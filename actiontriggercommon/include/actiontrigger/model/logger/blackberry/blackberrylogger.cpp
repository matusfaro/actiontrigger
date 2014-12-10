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

#include "actiontrigger/model/logger/blackberry/blackberrylogger.hpp"

namespace actiontrigger
{

    LogLevel BlackberryLogger::loglevel = DEBUG;
    std::map<std::string, BlackberryLogger*>* BlackberryLogger::instances = NULL;
    ConnectDataModel* BlackberryLogger::dataModelAll = NULL;
    ConnectDataModel* BlackberryLogger::dataModelLoading = NULL;

    void BlackberryLogger::trace(std::string msg)
    {
        if (loglevel >= TRACE) {
            lastMessage = getTimestamp() + " " + getTag("TRACE") + msg;
            std::cerr << lastMessage << std::endl;
            qDebug(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::debug(std::string msg)
    {
        if (loglevel >= DEBUG) {
            lastMessage = getTimestamp() + " " + getTag("DEBUG") + msg;
            std::cerr << lastMessage << std::endl;
            qDebug(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::info(std::string msg)
    {
        if (loglevel >= INFO) {
            lastMessage = getTimestamp() + " " + getTag("DEBUG") + msg;
            std::cerr << lastMessage << std::endl;
            qDebug(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::warning(std::string msg)
    {
        if (loglevel >= WARNING) {
            lastMessage = getTimestamp() + " " + getTag("WARNING") + msg;
            std::cerr << lastMessage << std::endl;
            qWarning(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::error(std::string msg)
    {
        if (loglevel >= ERROR) {
            lastMessage = getTimestamp() + " " + getTag("ERROR") + msg;
            std::cerr << lastMessage << std::endl;
            // Don't die on me... report it as warning instead
            qWarning(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::critical(std::string msg)
    {
        if (loglevel >= CRITICAL) {
            lastMessage = getTimestamp() + " " + getTag("CRITICAL") + msg;
            std::cerr << lastMessage << std::endl;
            // Don't die on me... report it as warning instead
            qWarning(lastMessage.c_str());
            if (NULL != this->dataModelAll) {
                BlackberryLogger::dataModelAll->send(lastMessage);
            }
        }
    }

    void BlackberryLogger::loading(std::string msg)
    {
        if (NULL != this->dataModelLoading) {
            lastMessage = msg;
            std::cerr << lastMessage << std::endl;
            BlackberryLogger::dataModelLoading->send(lastMessage);
        } else {
            lastMessage = "dataModelLoading is not set, omitting: " + msg;
            std::cerr << lastMessage << std::endl;
        }
    }

    BlackberryLogger* BlackberryLogger::getInstance(std::string classname)
    {
        std::map<std::string, BlackberryLogger*>* map = getMap();
        if (map->find(classname) == map->end()) {
            map->insert(
                    std::pair<std::string, BlackberryLogger*>(classname,
                            new BlackberryLogger(classname)));
        }
        return map->find(classname)->second;
    }

    void BlackberryLogger::setLogLevel(LogLevel newLevel)
    {
        loglevel = newLevel;
    }

    void BlackberryLogger::setContainerAll(bb::cascades::ArrayDataModel* dataModel)
    {
        BlackberryLogger::dataModelAll = new ConnectDataModel(dataModel);
    }

    void BlackberryLogger::setContainerLoading(bb::cascades::ArrayDataModel* splashLogDataModel)
    {
        BlackberryLogger::dataModelLoading = new ConnectDataModel(splashLogDataModel);
    }

    std::map<std::string, BlackberryLogger*>* BlackberryLogger::getMap()
    {
        if (!instances)
            instances = new std::map<std::string, BlackberryLogger*>();
        return instances;
    }

    BlackberryLogger::BlackberryLogger(const std::string& classname) :
            classname(classname)
    {
    }

    BlackberryLogger::~BlackberryLogger()
    {
    }

    std::string BlackberryLogger::getTag(std::string level)
    {
        return level + " " + SSTR(QThread::currentThreadId()) + " " + classname + " >>> ";
    }

} /* namespace actiontrigger */
