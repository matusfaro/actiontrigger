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

#include "actiontrigger/controller/permissions/permissionlocation.hpp"

namespace actiontrigger
{

    std::string PermissionLocation::TYPE = "LOCATION";

    PermissionLocation::PermissionLocation() :
            Permission(this->TYPE)
    {
    }

    PermissionLocation::~PermissionLocation()
    {
    }

    std::string PermissionLocation::getName() const
    {
        return "Location";
    }

    std::string PermissionLocation::getDescription() const
    {
        return "Allows the app to access the device's current location and access locations saved on the device. This permission includes access to geolocation, geofencing, cell tower information, Wi-Fi data, and Cascades Places.";
    }

    std::string PermissionLocation::getImagePath() const
    {
        return NULL;
    }

    bool PermissionLocation::hasAccessGrantedSystem()
    {
        return true;
    }

} /* namespace actiontrigger */
