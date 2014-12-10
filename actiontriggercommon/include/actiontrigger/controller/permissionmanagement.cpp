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

#include "actiontrigger/controller/permissionmanagement.hpp"

namespace actiontrigger
{

    Logger* PermissionManagement::LOG = LoggerFactory::getInstance("PermissionManagement");

    PermissionManagement* PermissionManagement::instance = NULL;

    PermissionManagement* PermissionManagement::getInstance()
    {
        if (!instance) {
            instance = new PermissionManagement();
        }
        return instance;
    }

    PermissionManagement::PermissionManagement()
    {
        this->permissions = new std::map<std::string, Permission*>();
        // List all possible permissions
        this->permissions->insert(
                std::pair<std::string, Permission*>(PermissionLocation::TYPE,
                        (Permission*) new PermissionLocation()));
    }

    PermissionManagement::~PermissionManagement()
    {
    }

    Permission* PermissionManagement::getPermission(std::string type)
    {
        if (permissions->find(type) == permissions->end()) {
            LOG->warning("Cannot find permission");
            return NULL;
        }
        return permissions->find(type)->second;
    }

    std::vector<Permission*>* PermissionManagement::getPermissions()
    {
        std::vector<Permission*>* permissionVector = new std::vector<Permission*>();
        for (std::map<std::string, Permission*>::iterator it = this->permissions->begin();
                it != this->permissions->end(); ++it) {
            permissionVector->push_back(it->second);
        }
        return permissionVector;
    }

} /* namespace actiontrigger */
