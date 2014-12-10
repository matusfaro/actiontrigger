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

#ifndef PERMISSION_HPP_
#define PERMISSION_HPP_

#include <string>
#include "actiontrigger/model/database/permissionmodel.hpp"
#include "actiontrigger/model/database/sqlite/permissiondbmodel.hpp"

namespace actiontrigger
{

    class Permission
    {
    public:
        Permission(std::string permissionType);
        virtual ~Permission();

        // Get human readable name of the permission
        virtual std::string getName() const = 0;
        // Get permission description
        virtual std::string getDescription() const = 0;
        // Get type identifier of permission
        std::string getType();
        // Get icon path for permission
        virtual std::string getImagePath() const = 0;

        // Checks whether access has been granted.
        bool hasAccessGranted();
        // Checks whether access has been granted on OS
        virtual bool hasAccessGrantedSystem() = 0;
        // Checks whether access has been granted within the application
        bool hasAccessGrantedApplication();

        // Allow permission within application
        void setAccessGrantedApplication();
        // Disallow permission within application
        void setAccessDeniedApplication();
    protected:
        PermissionModel* model;
        std::string type;
    };

} /* namespace actiontrigger */
#endif /* PERMISSION_HPP_ */
