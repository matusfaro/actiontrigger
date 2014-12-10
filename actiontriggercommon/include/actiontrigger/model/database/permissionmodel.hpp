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

#ifndef PERMISSIONMODEL_HPP_
#define PERMISSIONMODEL_HPP_

namespace actiontrigger
{

#define DEFAULT_ACCESS_STATE true

    class PermissionModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~PermissionModel()
        {
        }
        ;
        /*
         * Getters and setters
         */
        virtual bool hasAccessGranted() = 0;
        virtual void setAccessGranted() = 0;
        virtual void setAccessDenied() = 0;
    };

} /* namespace actiontrigger */

#endif /* PERMISSIONMODEL_HPP_ */
