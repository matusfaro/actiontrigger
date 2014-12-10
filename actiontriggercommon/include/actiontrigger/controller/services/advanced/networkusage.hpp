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

#ifndef NETWORKUSAGE_HPP_
#define NETWORKUSAGE_HPP_

#include "actiontrigger/model/database/sqlite/persistentstorage.hpp"
#include <QMutex>
#include <QMutexLocker>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/advanced/advancedtools.hpp"
#include <regex.h>
#include <string>
#include "errno.h"
#include <devctl.h>
#include <sys/procfs.h>

namespace actiontrigger
{

    typedef enum
    {
        INBOUND, OUTBOUND
    } Direction;

    class NetworkUsage
    {
    public:
        static NetworkUsage* get();
        std::map<std::string, std::string>* supportedInterfaces();
        errno_t update();
        std::map<std::string, std::string>* fetch();
        void reset();
    private:
        static QMutex mutex;
        NetworkUsage();
        static NetworkUsage* instance;
        std::map<std::string, std::string> interfaces;
        static Logger* LOG;
        unsigned long getBytes(Direction direction, std::string interface);
        bool isInited;
        bool deviceRebooted;
    };

} /* namespace actiontrigger */
#endif /* NETWORKUSAGE_HPP_ */
