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

#include "actiontrigger/model/database/sqlite/databasesqlite.hpp"

namespace actiontrigger
{

    Logger* DatabaseSQLite::LOG = LoggerFactory::getInstance("DatabaseSQLite");

    DatabaseSQLite* DatabaseSQLite::instance = NULL;

    DatabaseSQLite* DatabaseSQLite::getInstance(bool forceDatabaseReload)
    {
        LOG->trace("getInstance");
        if (!instance) {
            LOG->trace("Instantiating instance...");
            instance = new DatabaseSQLite(forceDatabaseReload);
        }
        return instance;
    }

    DatabaseSQLite::DatabaseSQLite(bool forceDatabaseReload) :
            settings(NULL)
    {
        LOG->trace("DatabaseSQLite");
        this->database = QSqlDatabase::addDatabase("QSQLITE");
        this->database.setDatabaseName(
                BlackberryFilesystem::loadFile("actiontrigger.db", forceDatabaseReload).c_str());
        if (database.open()) {
            LOG->trace("Database open");
        } else {
            LOG->trace("Database open");
            std::cerr << "Error opening connection to the database: ";
        }
    }

    DatabaseSQLite::~DatabaseSQLite()
    {
        LOG->trace("~DatabaseSQLite");
        delete this->settings;
        this->database.close();
    }

    PermissionModel* DatabaseSQLite::getPermissionModel(std::string permissionType)
    {
        return (PermissionModel*) new PermissionDBModel(permissionType);
    }

    SettingsModel* DatabaseSQLite::getSettingsModel()
    {
        if (NULL == this->settings) {
            this->settings = new SettingsDBModel();
        }
        return (SettingsModel*) this->settings;
    }

    std::vector<ProcedureModel*>* DatabaseSQLite::loadProcedures(int id)
    {
        LOG->trace("loadProcedures");
        QSqlQuery query;
        if (id == -1) {
            query.prepare("SELECT ID, NAME, STATEMENT_ID FROM PROCEDURE");
        } else {
            query.prepare("SELECT ID, NAME, STATEMENT_ID FROM PROCEDURE WHERE ID = :id");
            query.bindValue(":id", id);
        }
        SqliteTools::queryExec(&query);
        std::vector<ProcedureModel*>* procedures = new std::vector<ProcedureModel*>();
        while (query.next()) {
            LOG->trace(
                    "Loading.. Procedure: name=\""
                            + std::string(query.value(1).toString().toStdString()) + "\"");
            LOG->loading("     " + std::string(query.value(1).toString().toStdString()));
            StatementDBModel* nextStatement = NULL;
            if (!query.value(2).isNull() || query.value(2).toInt() != 0) {
                nextStatement = loadStatementTree(query.value(2).toInt());
            }
            procedures->push_back(ProcedureDBModel::load(READ_CACHE, // Database mode
                    query.value(0).toInt(), // ID
                    query.value(1).toString().toStdString(), // Name
                    nextStatement, // Statement
                    loadProcedureSettings(query.value(0).toInt()) // Procedure settings
                            ));
        }
        return procedures;
    }

    std::map<std::string, std::string>* DatabaseSQLite::loadProcedureSettings(int procedureId)
    {
        LOG->trace("loadProcedureSettings");
        QSqlQuery query;
        query.prepare(
                "SELECT NAME, VALUE FROM PROCEDURE_SETTINGS WHERE PROCEDURE_ID = :procedureid");
        query.bindValue(":procedureid", procedureId);
        SqliteTools::queryExec(&query);
        std::map<std::string, std::string>* settings = new std::map<std::string, std::string>();
        while (query.next()) {
            LOG->trace(
                    " Setting: name=\"" + std::string(query.value(0).toString().toStdString())
                            + "\"");
            settings->operator [](query.value(0).toString().toStdString()) =
                    query.value(1).toString().toStdString();
        }
        return settings;
    }

    StatementDBModel* DatabaseSQLite::loadStatementTree(int statementId)
    {
        LOG->trace("loadStatementTree");
        StatementDBModel* nextStatement;
        StatementDBModel* myStatement = NULL;
        QSqlQuery query;
        query.prepare("SELECT NEXT_STATEMENT, TYPE FROM STATEMENT WHERE ID = :id");
        query.bindValue(":id", statementId);
        SqliteTools::queryExec(&query);
        if (query.next()) {

            LOG->trace(
                    "Loading.. Statement:  type=\""
                            + std::string(query.value(2).toString().toStdString()) + "\"");
            if (query.value(0).isNull())
                nextStatement = NULL;
            else
                nextStatement = this->loadStatementTree(query.value(0).toInt());
            myStatement = StatementDBModel::load(READ_CACHE, // Database mode
                    statementId, // ID
                    query.value(1).toString().toStdString(), // Type
                    NULL, // Previous statement, will set later because chicken and egg
                    this->loadStatementBranches(statementId), // Statement branches
                    nextStatement, // Next statement
                    loadParameters(statementId) // Parameters
                            );
        } else {
            LOG->critical(
                    SSTR(
                            "Could not load statement. Database corrupted. StatementId: "
                                    << statementId));
            return NULL;
        }
        return myStatement;
    }

    std::map<std::string, StatementModel*>* DatabaseSQLite::loadStatementBranches(int statementId)
    {
        LOG->trace("loadStatementBranches");
        std::map<std::string, StatementModel*>* branches =
                new std::map<std::string, StatementModel*>();
        QSqlQuery query;
        query.prepare("SELECT name, child FROM branch WHERE parent = :id");
        query.bindValue(":id", statementId);
        SqliteTools::queryExec(&query);
        while (query.next()) {
            LOG->trace(
                    "Loading.. Branch: name=\""
                            + std::string(query.value(0).toString().toStdString()) + "\"");
            branches->insert(
                    std::pair<std::string, StatementModel*>(
                            std::string(query.value(0).toString().toStdString()),
                            this->loadStatementTree(query.value(1).toInt())));
        }
        return branches;
    }

    std::map<std::string, ParameterModel*>* DatabaseSQLite::loadParameters(int statementId)
    {
        LOG->trace("loadParameters");
        QSqlQuery query;
        query.prepare(
                "SELECT ID, NAME, VALUE FROM PARAMETER_VALUE WHERE STATEMENT_ID = :statementId");
        query.bindValue(":statementId", statementId);
        SqliteTools::queryExec(&query);
        std::map<std::string, ParameterModel*>* parameters = new std::map<std::string,
                ParameterModel*>();
        while (query.next()) {
            LOG->trace(
                    "Loading.. Parameter: key=\""
                            + std::string(query.value(1).toString().toStdString()) + "\" value=\""
                            + std::string(query.value(2).toString().toStdString()) + "\"");
//        LOG->loading("   Parameter: " + std::string(query.value(1).toString().toStdString()));
            parameters->insert(
                    std::pair<std::string, ParameterModel*>(query.value(1).toString().toStdString(), // Name
                            (ParameterModel*) ParameterDBModel::load(READ_CACHE, // Database mode
                                    query.value(0).toInt(), // Id
                                    query.value(1).toString().toStdString(), // Name
                                    query.value(2).toString().toStdString(), // Value
                                    statementId // Statement Id
                                    )));
        }
        return parameters;
    }

} /* namespace actiontrigger */
