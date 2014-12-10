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

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include "actiontrigger/model/database/permissionmodel.hpp"
#include "actiontrigger/model/database/settingsmodel.hpp"

namespace actiontrigger
{

    class Database
    {
    public:
        /*
         *  Loads a vector of procedures. Optionally specify db id to load a single procedure.
         */
        virtual std::vector<ProcedureModel*>* loadProcedures(int id = -1) = 0;
        /*
         *  Loads permission model.
         */
        virtual PermissionModel* getPermissionModel(std::string permissionType) = 0;
        /*
         *  Loads settings model.
         */
        virtual SettingsModel* getSettingsModel() = 0;

        virtual ~Database()
        {
        }
        ;
    };

} /* namespace actiontrigger */
#endif /* DATABASE_HPP_ */
