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

#ifndef LOGGERFACTORY_HPP_
#define LOGGERFACTORY_HPP_

#include "actiontrigger/model/logger/logger.hpp"
#include "actiontrigger/model/logger/blackberry/blackberrylogger.hpp"

namespace actiontrigger
{

    class LoggerFactory
    {
    public:
        static Logger* getInstance(std::string classname);
    private:
        LoggerFactory();
        virtual ~LoggerFactory();
    };

} /* namespace actiontrigger */
#endif /* LOGGERFACTORY_HPP_ */
