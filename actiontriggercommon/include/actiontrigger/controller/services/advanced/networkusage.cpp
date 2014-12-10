/*
 * ActionTrigger
 * Copyright (C) 2013  Matus Faro
 *
 * This file is part of ActionTrigger.
 *
 * ActionTrigger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ActionTrigger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ActionTrigger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "actiontrigger/controller/services/advanced/networkusage.hpp"

namespace actiontrigger
{

    Logger* NetworkUsage::LOG = LoggerFactory::getInstance("NetworkUsage");
    QMutex NetworkUsage::mutex(QMutex::Recursive);
    NetworkUsage* NetworkUsage::instance = NULL;

    NetworkUsage::NetworkUsage()
    {
        interfaces.insert(std::pair<std::string, std::string>("Wifi", "tiw_sta0"));
        interfaces.insert(std::pair<std::string, std::string>("Radio", "tiw_drv0")); //TODO find correct interface name

        // Has been inited before
        isInited = PersistentStorage::get()->getInternalValue("net_usage_init") == "TRUE";

        // Has been rebooted since last check
        std::string boot_time = SSTR(AdvancedTools::getBootTime());
        deviceRebooted = PersistentStorage::get()->getInternalValue("net_usage_boottime")
                != boot_time;
        if (deviceRebooted) {
            PersistentStorage::get()->setInternalValue("net_usage_boottime", boot_time);
        }
    }

    NetworkUsage* NetworkUsage::get()
    {
        QMutexLocker locker(&mutex);
        if (!instance) {
            instance = new NetworkUsage();
        }
        locker.unlock();
        return instance;
    }

    std::map<std::string, std::string>* NetworkUsage::supportedInterfaces()
    {
        return &interfaces;
    }

    errno_t NetworkUsage::update()
    {
        QMutexLocker locker(&mutex);

        // Iterate over all interfaces in question
        for (std::map<std::string, std::string>::iterator it = interfaces.begin();
                it != interfaces.end(); ++it) {
            // Loop twice for inbound and outbound
            Direction currDirection = INBOUND;
            for (int i = 0; i < 2; ++i) {
                do {
                    // Retrieve current reading
                    errno = EOK;
                    unsigned long bytesSinceBoot = getBytes(currDirection, it->second);
                    if (errno != EOK) {
                        LOG->error(SSTR("Could not get interface reading: " << strerror(errno)));
                        continue;
                    }

                    if (!isInited) {
                        LOG->trace("Detected reset or uninitialized network usage counter");
                        PersistentStorage::get()->setInternalValue(
                                SSTR("net_usage_bytes_" << currDirection << "_" << it->first), "0");
                        PersistentStorage::get()->setInternalValue(
                                SSTR(
                                        "net_usage_last_reading_" << currDirection << "_"
                                                << it->first), SSTR(bytesSinceBoot));
                        continue;
                    }

                    // Retrieve stored bytes count
                    std::string bytesString = PersistentStorage::get()->getInternalValue(
                            SSTR("net_usage_bytes_" << currDirection << "_" << it->first));
                    unsigned long bytes;
                    try {
                        bytes = Common::StringToNumber<unsigned long>(bytesString);
                    } catch (...) {
                        LOG->error(SSTR("Could not read bytes value: " << bytesString));
                        // Recover
                        bytes = 0;
                    }

                    if (deviceRebooted) {
                        // If device was rebooted, add the reading to current bytes count
                        PersistentStorage::get()->setInternalValue(
                                SSTR("net_usage_bytes_" << currDirection << "_" << it->first),
                                SSTR((bytes + bytesSinceBoot)));
                    } else {
                        std::string lastReadingString = PersistentStorage::get()->getInternalValue(
                                SSTR(
                                        "net_usage_last_reading_" << currDirection << "_"
                                                << it->first));
                        unsigned long lastReading;
                        try {
                            lastReading = Common::StringToNumber<unsigned long>(lastReadingString);
                        } catch (...) {
                            LOG->error(
                                    SSTR(
                                            "Could not read last reading value: "
                                                    << lastReadingString));
                            // Recover
                            lastReading = 0;
                        }
                        // If device was not rebooted, add the reading minus the last reading to current bytes count
                        PersistentStorage::get()->setInternalValue(
                                SSTR("net_usage_bytes_" << currDirection << "_" << it->first),
                                SSTR((bytes + (bytesSinceBoot - lastReading))));
                    }
                    PersistentStorage::get()->setInternalValue(
                            SSTR("net_usage_last_reading_" << currDirection << "_" << it->first),
                            SSTR(bytesSinceBoot));
                } while (false);
                // Change to outbound and loop again
                currDirection = OUTBOUND;
            }
        }

        if (!isInited) {
            PersistentStorage::get()->setInternalValue("net_usage_init", "TRUE");
            isInited = true;
        }
        deviceRebooted = false;

        locker.unlock();
        return EOK;
    }

    unsigned long NetworkUsage::getBytes(Direction direction, std::string interface)
    {
        LOG->trace("getBytes");
        std::string raw_data = AdvancedTools::invoke("ifconfig -v " + interface);
        std::string regex_string = SSTR(
                (direction == INBOUND ? "input" : "output") << ": +[0-9]+ +[^ ]+ +([0-9]+) +bytes");
        regex_t exp;
        if (0 != regcomp(&exp, regex_string.c_str(), REG_EXTENDED | REG_ICASE)) {
            LOG->error("Error compiling regular expression.");
            errno = EIO;
            return 0;
        }
        regmatch_t input_matches[2];
        if (0 != regexec(&exp, raw_data.c_str(), 2, input_matches, 0)) {
            LOG->error(SSTR("Could not match raw data: " << raw_data));
            errno = EAGAIN;
            return 0;
        }
        std::string bytes_string = raw_data.substr(input_matches[1].rm_so,
                input_matches[1].rm_eo - input_matches[1].rm_so);
        unsigned long bytes = 0;
        try {
            bytes = Common::StringToNumber<unsigned long>(bytes_string);
        } catch (...) {
            LOG->error(SSTR("Could not convert to unsigned long: " << bytes_string));
            errno = EAGAIN;
            return 0;
        }
        errno = EOK;
        return bytes;
    }

    std::map<std::string, std::string>* NetworkUsage::fetch()
    {
        std::map<std::string, std::string>* usage = new std::map<std::string, std::string>();
        QMutexLocker locker(&mutex);
        if (PersistentStorage::get()->getInternalValue("net_usage_init") == "TRUE") {
            for (std::map<std::string, std::string>::iterator it = interfaces.begin();
                    it != interfaces.end(); ++it) {
                // Loop twice for inbound and outbound
                Direction currDirection = INBOUND;
                unsigned long totalBytes = 0;
                for (int i = 0; i < 2; ++i) {
                    std::string bytesStr = PersistentStorage::get()->getInternalValue(
                            SSTR("net_usage_bytes_" << currDirection << "_" << it->first));
                    usage->insert(
                            std::pair<std::string, std::string>(
                                    it->first + (currDirection == INBOUND ? "_IN" : "_OUT"),
                                    bytesStr));
                    // Change to outbound and loop again
                    currDirection = OUTBOUND;
                    // Keep track of total per interface
                    totalBytes += Common::StringToNumber<unsigned long>(bytesStr);
                }
                usage->insert(
                        std::pair<std::string, std::string>(it->first + "_TOTAL",
                                SSTR(totalBytes)));
            }
        } else {
            for (std::map<std::string, std::string>::iterator it = interfaces.begin();
                    it != interfaces.end(); ++it) {
                usage->insert(std::pair<std::string, std::string>(it->first + "_IN", "0"));
                usage->insert(std::pair<std::string, std::string>(it->first + "_OUT", "0"));
                usage->insert(std::pair<std::string, std::string>(it->first + "_TOTAL", "0"));
            }
        }
        locker.unlock();
        return usage;
    }

    void NetworkUsage::reset()
    {
        QMutexLocker locker(&mutex);

        isInited = false;
        deviceRebooted = false;
        PersistentStorage::get()->setInternalValue("net_usage_init", "FALSE");

        locker.unlock();
    }

} /* namespace actiontrigger */
