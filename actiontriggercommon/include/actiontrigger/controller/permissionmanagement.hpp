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

#ifndef PERMISSIONMANAGEMENT_HPP_
#define PERMISSIONMANAGEMENT_HPP_

#include <string>
#include <vector>
#include <map>
#include "actiontrigger/controller/permissions/permission.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/permissions/permissionlocation.hpp"

namespace actiontrigger
{

    class PermissionManagement
    {
    public:
        static PermissionManagement* getInstance();
        Permission* getPermission(std::string type);
        std::vector<Permission*>* getPermissions();
    private:
        PermissionManagement();
        virtual ~PermissionManagement();
        std::map<std::string, Permission*>* permissions;
        static Logger* LOG;
        static PermissionManagement* instance;
    };

} /* namespace actiontrigger */
#endif /* PERMISSIONMANAGEMENT_HPP_ */
