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

    LogLevel BlackberryLogger::loglevel = NONE;
    std::map<std::string, BlackberryLogger*>* BlackberryLogger::instances = NULL;
    QMutex BlackberryLogger::logFileLock;
    QFile* BlackberryLogger::logFile = NULL;
    QTextStream* BlackberryLogger::logStream = NULL;
    ConnectDataModel* BlackberryLogger::dataModelAll = NULL;
    ConnectDataModel* BlackberryLogger::dataModelLoading = NULL;

    void BlackberryLogger::trace(std::string msg)
    {
        if (loglevel >= TRACE) {
            lastMessage = getTimestamp() + " " + getTag("TRACE") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::debug(std::string msg)
    {
        if (loglevel >= DEBUG) {
            lastMessage = getTimestamp() + " " + getTag("DEBUG") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::info(std::string msg)
    {
        if (loglevel >= INFO) {
            lastMessage = getTimestamp() + " " + getTag("INFO") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::warning(std::string msg)
    {
        if (loglevel >= WARNING) {
            lastMessage = getTimestamp() + " " + getTag("WARNING") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::error(std::string msg)
    {
        if (loglevel >= ERROR) {
            lastMessage = getTimestamp() + " " + getTag("ERROR") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::critical(std::string msg)
    {
        if (loglevel >= CRITICAL) {
            lastMessage = getTimestamp() + " " + getTag("CRITICAL") + msg;
            log(lastMessage);
        }
    }

    void BlackberryLogger::loading(std::string msg)
    {
        if (NULL != this->dataModelLoading) {
            lastMessage = msg;
            log(lastMessage);
            BlackberryLogger::dataModelLoading->send(lastMessage);
        } else {
            lastMessage = "dataModelLoading is not set, omitting: " + msg;
            log(lastMessage);
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

    void BlackberryLogger::log(std::string msg)
    {
//        {
//            QMutexLocker lock(&logFileLock);
//            // Open log file
//            if (NULL == logFile) {
//                logFile = new QFile(
//                        QDir::currentPath() + QString("/shared/misc/actiontrigger.log.")
//                                + QString::number(QCoreApplication::applicationPid()));
//                if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
//                    logFile->setPermissions(QFile::ReadOther | QFile::ReadUser | QFile::ReadGroup);
//                } else {
//                    std::cerr << "Cannot open log file" << std::endl;
//                    delete logFile;
//                    logFile = NULL;
//                }
//            }
//            // Open stream
//            if (NULL != logFile && NULL == logStream) {
//                logStream = new QTextStream(logFile);
//            }
//            // Append log line to file
//            if (NULL != logStream) {
//                if (logStream->status() == QTextStream::Ok) {
//                    logStream->operator <<(QString::fromStdString(msg));
//                    logStream->operator <<("\n");
//                    logStream->flush();
//                } else {
//                    std::cerr << "log file stream is closed " << logStream->status() << std::endl;
//                    delete logStream;
//                    logStream = NULL;
//                }
//            }
//        }

        std::cerr << msg << std::endl;
        qWarning(msg.c_str());
        if (NULL != dataModelAll) {
            BlackberryLogger::dataModelAll->send(msg);
        }
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
