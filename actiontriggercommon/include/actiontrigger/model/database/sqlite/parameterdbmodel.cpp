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

#include "actiontrigger/model/database/sqlite/parameterdbmodel.hpp"

namespace actiontrigger
{

    Logger* ParameterDBModel::LOG = LoggerFactory::getInstance("ParameterDBModel");

    ParameterDBModel::ParameterDBModel(DATABASE_MODE mode, int id, std::string name,
            std::string value, int statementId) :
            mode(mode), id(id), name(name), value(value), statementId(statementId)
    {
    }

    ParameterDBModel::~ParameterDBModel()
    {
    }

    /*
     * Delete permanently
     */
    void ParameterDBModel::remove()
    {
        LOG->trace(SSTR("remove " << name));
        QSqlQuery query;
        query.prepare("DELETE FROM PARAMETER_VALUE WHERE ID = :id");
        query.bindValue(":id", this->id);
        SqliteTools::queryExec(&query);
    }

    /*
     * Create parameter and save into database
     */
    ParameterDBModel* ParameterDBModel::create(DATABASE_MODE mode, std::string name,
            std::string value, int statementId)
    {
        LOG->trace(SSTR("setValue " << name << "=" << value));
        int id = -1;
        if (mode != IN_MEMORY) {
            QSqlQuery query;
            query.prepare(
                    "INSERT INTO PARAMETER_VALUE (NAME, VALUE, STATEMENT_ID) VALUES (:name, :value, :statementId)");
            query.bindValue(":name", QString::fromUtf8(name.c_str()));
            query.bindValue(":value", QString::fromUtf8(value.c_str()));
            query.bindValue(":statementId", statementId);
            SqliteTools::queryExec(&query);
            QSqlDatabase::database("ACTION_TRIGGER_MAIN_DATABASE");
            id = query.lastInsertId().toInt();
        }
        return load(mode, // Database mode
                id, // Retrieve id
                name, // Name
                value, // Value
                statementId // Statement Id
                );
    }

    /*
     * Load Parameter into memory
     */
    ParameterDBModel* ParameterDBModel::load(DATABASE_MODE mode, int id, std::string name,
            std::string value, int statementId)
    {
        return new ParameterDBModel(mode, id, name, value, statementId);
    }

    int ParameterDBModel::getId() const
    {
        return this->id;
    }

    std::string ParameterDBModel::getValue() const
    {
        LOG->trace(SSTR("getValue " << this->name << "=" << this->value));
        return this->value;
    }

    void ParameterDBModel::setValue(std::string value)
    {
        LOG->trace(SSTR("setValue " << this->name << "=" << value));
        if (mode != IN_MEMORY) {
            QSqlQuery query;
            query.prepare("UPDATE PARAMETER_VALUE SET VALUE = :value WHERE id = :id");
            query.bindValue(":id", this->id);
            query.bindValue(":value", QString::fromUtf8(value.c_str()));
            SqliteTools::queryExec(&query);
        }
        this->value = value;
    }

    std::string ParameterDBModel::getName() const
    {
        return this->name;
    }

    void ParameterDBModel::setName(std::string name)
    {
        if (mode != IN_MEMORY) {
            QSqlQuery query;
            query.prepare("UPDATE PARAMETER_VALUE SET NAME = :name WHERE id = :id");
            query.bindValue(":id", this->id);
            query.bindValue(":name", QString::fromUtf8(name.c_str()));
            SqliteTools::queryExec(&query);
        }
        this->name = name;
    }

} /* namespace actiontrigger */
