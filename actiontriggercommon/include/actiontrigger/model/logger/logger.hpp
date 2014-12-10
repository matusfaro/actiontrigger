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

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <QDateTime>

// Default procedure/statement run loglevel
// NOT THE APPLICATION LOGLEVEL
#define DEFAULT_LOGLEVEL WARNING

// Infamous string concatenator
#include <sstream>
#define SSTR( x ) dynamic_cast< std::ostringstream & > ( ( std::ostringstream() << std::dec << x ) ).str()

namespace actiontrigger
{

    enum LogLevel
    {
        NONE, CRITICAL, ERROR, WARNING, INFO, DEBUG, TRACE
    };

    class Logger
    {
    public:
        virtual ~Logger()
        {
        }
        ;
        virtual void trace(std::string) = 0;
        virtual void debug(std::string) = 0;
        virtual void info(std::string) = 0;
        virtual void warning(std::string) = 0;
        virtual void error(std::string) = 0;
        virtual void critical(std::string) = 0;
        virtual void loading(std::string)
        {
        }
        ;
        static LogLevel parseLogLevel(std::string loglevel)
        {
            if (loglevel == "NONE")
                return NONE;
            else if (loglevel == "CRITICAL")
                return CRITICAL;
            else if (loglevel == "ERROR")
                return ERROR;
            else if (loglevel == "WARNING")
                return WARNING;
            else if (loglevel == "INFO")
                return INFO;
            else if (loglevel == "DEBUG")
                return DEBUG;
            else if (loglevel == "TRACE")
                return TRACE;
            else
                return NONE;
        }
        ;
        static std::string parseLogLevelName(LogLevel loglevel)
        {
            switch (loglevel) {
                case NONE:
                    return "NONE";
                case CRITICAL:
                    return "CRITICAL";
                case ERROR:
                    return "ERROR";
                case WARNING:
                    return "WARNING";
                case INFO:
                    return "INFO";
                case DEBUG:
                    return "DEBUG";
                case TRACE:
                    return "TRACE";
                default:
                    return "ERROR";
            }
        }
        ;

        static std::string getTimestamp()
        {
            QDateTime datetime = QDateTime::currentDateTime();
            return SSTR(
                    datetime.date().year() << "/" << datetime.date().month() << "/" << datetime.date().day() << " " << datetime.time().toString().toStdString() << "." << datetime.time().msec());
        }
        ;

    };

} /* namespace actiontrigger */
#endif /* LOGGER_HPP_ */
