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

#include "actiontrigger/model/database/sqlite/proceduredbmodel.hpp"

namespace actiontrigger
{

    Logger* ProcedureDBModel::LOG = LoggerFactory::getInstance("ProcedureDBModel");

    ProcedureDBModel::ProcedureDBModel(DATABASE_MODE mode, int id, std::string name,
            StatementModel* statement, std::map<std::string, std::string>* settings) :
            mode(mode), id(id), name(name), statement(statement)
    {
        LOG->trace("ProcedureDBModel");
        if (settings == NULL) {
            this->settings = new std::map<std::string, std::string>();
        } else {
            this->settings = settings;
        }
    }

    ProcedureDBModel::~ProcedureDBModel()
    {
        LOG->debug("~ProcedureDBModel");
        delete settings;
        delete statement;
    }

    QVariant ProcedureDBModel::toQVariant() const
    {
        QVariantMap data;

        // Name
        data.insert("NAME", QVariant(QString::fromStdString(getName())));

        // Settings
        QVariantMap qSettings;
        for (std::map<std::string, std::string>::iterator it = settings->begin();
                it != settings->end(); ++it) {
            qSettings.insert(QString::fromStdString(it->first),
                    QVariant(QString::fromStdString(it->second)));
        }
        data.insert("SETTINGS", qSettings);

        // Actions
        if (this->getStatement() != NULL) {
            data.insert("ACTIONS", this->getStatement()->toQVariant());
        }

        return QVariant(data);
    }

    ProcedureModel* ProcedureDBModel::create(DATABASE_MODE mode, QVariant dataVariant)
    {
        LOG->trace("create(qvariant)");

        std::string name = "-- CORRUPTED --";
        StatementModel* statement = NULL;

        if (dataVariant.isValid()) {
            if (dataVariant.type() == QVariant::Map) {
                QVariantMap data = dataVariant.toMap();

                // Load name
                if (data.contains("NAME")) {
                    QVariant qName = data.value("NAME");
                    if (qName.type() == QVariant::String) {
                        name = qName.toString().toStdString();
                    } else {
                        LOG->warning("Import: Skipping procedure NAME, not of type String");
                    }
                } else {
                    LOG->warning("Import: Skipping procedure NAME, not existent");
                }

                // Load next statement
                if (data.contains("ACTIONS")) {
                    statement = StatementDBModel::create(mode, data.value("ACTIONS"));
                }
            } else {
                LOG->warning("Import: Skipping procedure data, not of type Map");
            }
        } else {
            LOG->warning("Import: Skipping procedure data, not valid");
        }

        ProcedureModel* model = create(mode, // Database mode
                name, // Name
                statement // Statement Id
                );

        if (dataVariant.isValid()) {
            if (dataVariant.type() == QVariant::Map) {
                QVariantMap data = dataVariant.toMap();

                // Insert settings
                if (data.contains("SETTINGS")) {
                    QVariant qSettings = data.value("SETTINGS");
                    if (qSettings.type() == QVariant::Map) {
                        QVariantMap settings = qSettings.toMap();
                        for (QVariantMap::const_iterator it = settings.begin();
                                it != settings.end(); ++it) {
                            if (it.value().type() == QVariant::String) {
                                model->setSetting(it.key().toStdString(),
                                        it.value().toString().toStdString());
                            } else {
                                LOG->warning("Import: Skipping setting " + it.key().toStdString());
                            }
                        }
                    } else {
                        LOG->warning("Import: Skipping SETTINGS, not of type Map");
                    }
                } else {
                    LOG->warning("Import: Skipping SETTINGS, not existent");
                }
            }
        }

        return model;
    }

    ProcedureModel* ProcedureDBModel::create(DATABASE_MODE mode, std::string name,
            StatementModel* statement)
    {
        LOG->trace("create(new)");
        int id = -1;
        if (mode != IN_MEMORY) {
            QSqlQuery query;
            query.prepare("INSERT INTO PROCEDURE (NAME, STATEMENT_ID) VALUES (:name, :statement)");
            query.bindValue(":statement", statement ? statement->getId() : QVariant(QVariant::Int));
            query.bindValue(":name", QString::fromUtf8(name.c_str()));
            SqliteTools::queryExec(&query);
            id = query.lastInsertId().toInt();
        }
        return load(mode, // Database mode
                id, // Retrieve id
                name, // Name
                statement // Statement Id
                );
    }

    ProcedureModel* ProcedureDBModel::load(DATABASE_MODE mode, int id, std::string name,
            StatementModel* statement, std::map<std::string, std::string>* settings)
    {
        LOG->trace("load");
        LOG->trace("name=" + name);
        return (ProcedureModel*) new ProcedureDBModel(mode, id, name, statement, settings);
    }

    /*
     * Delete permanently
     */
    void ProcedureDBModel::remove()
    {
        LOG->trace("remove");
        {
            QSqlQuery query;
            query.prepare("DELETE FROM PROCEDURE WHERE ID = :id");
            query.bindValue(":id", this->id);
            SqliteTools::queryExec(&query);
        }
        {
            QSqlQuery query;
            query.prepare("DELETE FROM PROCEDURE_SETTINGS WHERE PROCEDURE_ID = :id");
            query.bindValue(":id", this->id);
            SqliteTools::queryExec(&query);
        }
    }

    int ProcedureDBModel::getId() const
    {
        LOG->trace("getId");
        return this->id;
    }

    std::string ProcedureDBModel::getName() const
    {
        LOG->trace("getName");
        return this->name;
    }

    void ProcedureDBModel::setName(std::string name)
    {
        LOG->trace("setName");
        QSqlQuery query;
        query.prepare("UPDATE PROCEDURE SET NAME = :name WHERE id = :id");
        query.bindValue(":id", this->id);
        query.bindValue(":name", QString::fromUtf8(name.c_str()));
        SqliteTools::queryExec(&query);
        this->name = name;
    }

    StatementModel* ProcedureDBModel::getStatement() const
    {
        LOG->trace("getTrigger");
        return this->statement;
    }

    void ProcedureDBModel::setStatement(StatementModel* statement)
    {
        LOG->trace("setTrigger");
        statement->unsetPreviousStatement();
        QSqlQuery query;
        query.prepare("UPDATE PROCEDURE SET statement_id = :statement WHERE id = :id");
        query.bindValue(":id", this->id);
        query.bindValue(":statement", statement->getId());
        SqliteTools::queryExec(&query);
        this->statement = statement;
    }

    void ProcedureDBModel::unsetStatement()
    {
        LOG->trace("unsetTrigger");
        if (statement) {
            QSqlQuery query;
            query.prepare("UPDATE PROCEDURE SET statement_id = NULL WHERE id = :id");
            query.bindValue(":id", this->id);
            SqliteTools::queryExec(&query);
            this->statement = NULL;
        } else {
            LOG->warning("Unset statement called when no statement exists");
        }
    }

    std::string ProcedureDBModel::getSetting(std::string name) const
    {
        if (this->settings->find(name) == this->settings->end()) {
            return "";
        }
        return this->settings->find(name)->second;
    }

    void ProcedureDBModel::setSetting(std::string name, std::string value)
    {
        if (this->settings->find(name) == this->settings->end()) {
            QSqlQuery query;
            query.prepare(
                    "INSERT INTO PROCEDURE_SETTINGS (PROCEDURE_ID, NAME, VALUE) VALUES (:procedureid, :name, :value)");
            query.bindValue(":procedureid", this->id);
            query.bindValue(":name", QString::fromUtf8(name.c_str()));
            query.bindValue(":value", QString::fromUtf8(value.c_str()));
            SqliteTools::queryExec(&query);
        } else {
            QSqlQuery query;
            query.prepare(
                    "UPDATE PROCEDURE_SETTINGS SET VALUE = :value WHERE PROCEDURE_ID = :procedureid AND NAME = :name");
            query.bindValue(":value", QString::fromUtf8(value.c_str()));
            query.bindValue(":procedureid", this->id);
            query.bindValue(":name", QString::fromUtf8(name.c_str()));
            SqliteTools::queryExec(&query);
        }
        this->settings->operator[](name) = value;
    }

} /* namespace actiontrigger */
