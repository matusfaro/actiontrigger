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

#ifndef PERMISSIONLOCATION_HPP_
#define PERMISSIONLOCATION_HPP_

#include "actiontrigger/controller/permissions/permission.hpp"
#include <bps/geolocation.h>

namespace actiontrigger
{

    class PermissionLocation: public Permission
    {
    public:
        PermissionLocation();
        virtual ~PermissionLocation();

        // Unique permission type
        static std::string TYPE;

        // Get human readable name of the permission
        std::string getName() const;
        // Get permission description
        std::string getDescription() const;
        // Get icon path for permission
        std::string getImagePath() const;
        // Checks whether access has been granted on OS
        bool hasAccessGrantedSystem();
    };

} /* namespace actiontrigger */
#endif /* PERMISSIONLOCATION_HPP_ */
