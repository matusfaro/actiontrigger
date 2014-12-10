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

#ifndef DATABASESQLITE_HPP_
#define DATABASESQLITE_HPP_

#include "actiontrigger/model/loggerfactory.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtSql/QSqlDatabase>
#include <bb/data/SqlDataAccess>
#include "actiontrigger/model/database/sqlite/proceduredbmodel.hpp"
#include "actiontrigger/model/database/sqlite/statementdbmodel.hpp"
#include "actiontrigger/model/database/sqlite/parameterdbmodel.hpp"
#include "actiontrigger/model/filesystem/blackberry/blackberryfilesystem.hpp"
#include "actiontrigger/model/database/database.hpp"
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"
#include <QSqlError>
#include "actiontrigger/model/database/sqlite/permissiondbmodel.hpp"
#include "actiontrigger/model/database/permissionmodel.hpp"
#include "actiontrigger/model/database/sqlite/settingsdbmodel.hpp"
#include "actiontrigger/model/database/settingsmodel.hpp"

namespace actiontrigger
{

    class DatabaseSQLite: public Database
    {
    public:
        /*
         *  Get singleton instance.
         */
        static DatabaseSQLite* getInstance(bool forceDatabaseReload);
        /*
         *  Loads a vector of procedures.
         */
        std::vector<ProcedureModel*>* loadProcedures(int id = -1);
        /*
         *  Loads permission model.
         */
        PermissionModel* getPermissionModel(std::string permissionType);
        /*
         *  Loads settings model.
         */
        SettingsModel* getSettingsModel();
    private:
        /*
         * Given a statement id, loads a tree of statements. First statement is returned.
         */
        StatementDBModel* loadStatementTree(int id);
        /*
         * Given a statement id, loads only statement branches.
         */
        std::map<std::string, StatementModel*>* loadStatementBranches(int statementId);
        /*
         * Given a procedure id, loads all settings.
         */
        std::map<std::string, std::string>* loadProcedureSettings(int procedureId);
        /*
         *  Given statement id, loads a map of parameters.
         */
        std::map<std::string, ParameterModel*>* loadParameters(int id);

        static DatabaseSQLite* instance;
        QSqlDatabase database;
        static Logger* LOG;
        SettingsDBModel* settings;

        DatabaseSQLite(bool forceDatabaseReload = false);
        virtual ~DatabaseSQLite();
    };

} /* namespace actiontrigger */
#endif /* DATABASESQLITE_HPP_ */
