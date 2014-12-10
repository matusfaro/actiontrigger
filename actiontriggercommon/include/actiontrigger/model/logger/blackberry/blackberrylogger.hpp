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

#ifndef BLACKBERRYLOGGER_HPP_
#define BLACKBERRYLOGGER_HPP_

#include "actiontrigger/model/logger/blackberry/connectdatamodel.hpp"
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <qt4/QtCore/qDebug>
#include "actiontrigger/model/logger/logger.hpp"
#include <bb/cascades/ArrayDataModel>

namespace actiontrigger
{

    class BlackberryLogger: public Logger
    {
    public:
        static BlackberryLogger* getInstance(std::string classname);
        static void setLogLevel(LogLevel newLevel);
        static void setContainerAll(bb::cascades::ArrayDataModel* dataModel);
        static void setContainerLoading(bb::cascades::ArrayDataModel* splashLogDataModel);
        inline void trace(std::string);
        inline void debug(std::string);
        inline void info(std::string);
        inline void warning(std::string);
        inline void error(std::string);
        inline void critical(std::string);
        inline void loading(std::string);
    private:
        static LogLevel loglevel;
        BlackberryLogger(const std::string& classname);
        virtual ~BlackberryLogger();

        const std::string classname;
        std::string lastMessage;
        std::string getTag(std::string level);

        static std::map<std::string, BlackberryLogger*>* instances;
        static std::map<std::string, BlackberryLogger*>* getMap();

        static ConnectDataModel* dataModelAll;
        static ConnectDataModel* dataModelLoading;
    };

} /* namespace actiontrigger */
#endif /* BLACKBERRYLOGGER_HPP_ */
