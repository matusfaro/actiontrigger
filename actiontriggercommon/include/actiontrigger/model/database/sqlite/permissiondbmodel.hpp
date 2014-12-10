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

#ifndef PERMISSIONDBMODEL_HPP_
#define PERMISSIONDBMODEL_HPP_

#include "actiontrigger/model/database/permissionmodel.hpp"
#include <string>
#include <QSqlQuery>
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"

namespace actiontrigger
{

    class PermissionDBModel: public PermissionModel
    {
    public:
        PermissionDBModel(std::string permissionType);
        virtual ~PermissionDBModel();

        bool hasAccessGranted();
        void setAccessGranted();
        void setAccessDenied();
    private:
        void setAccess(bool newAccessGranted);
        std::string permissionType;
        bool accessGranted;
    };

} /* namespace actiontrigger */
#endif /* PERMISSIONDBMODEL_HPP_ */
