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

#include "actiontrigger/model/database/sqlite/statementdbmodel.hpp"

namespace actiontrigger
{

    Logger* StatementDBModel::LOG = LoggerFactory::getInstance("StatementDBModel");

    StatementDBModel::StatementDBModel(DATABASE_MODE mode, int id, std::string type,
            StatementModel* previousStatement, std::map<std::string, StatementModel*>* branches,
            StatementModel* nextStatement, std::map<std::string, ParameterModel*>* parameters) :
            mode(mode), id(id), type(type), previousStatement(previousStatement), branches(
                    branches), nextStatement(nextStatement), parameters(parameters)
    {
        this->position = nextStatement ? nextStatement->getPosition() - 1 : 9000;
        if (NULL != nextStatement)
            nextStatement->setPreviousStatement(this);
    }

    StatementDBModel::~StatementDBModel()
    {
        for (std::map<std::string, ParameterModel*>::iterator it = parameters->begin();
                it != parameters->end(); ++it) {
            delete it->second;
        }
        delete parameters;
        for (std::map<std::string, StatementModel*>::iterator it = branches->begin();
                it != branches->end(); ++it) {
            if (it->second != NULL) {
                delete it->second;
            }
        }
        delete branches;
        delete nextStatement;
    }

    void StatementDBModel::remove()
    {
        {
            QSqlQuery query;
            query.prepare("DELETE FROM STATEMENT WHERE ID = :id");
            query.bindValue(":id", this->id);
            SqliteTools::queryExec(&query);
        }
        {
            QSqlQuery query;
            query.prepare("DELETE FROM PARAMETER_VALUE WHERE STATEMENT_ID = :id");
            query.bindValue(":id", this->id);
            SqliteTools::queryExec(&query);
        }
    }

    QVariant StatementDBModel::toQVariant(bool standalone) const
    {
        QVariantMap data;

        // Statement type
        data.insert("TYPE", QVariant(QString::fromStdString(getType())));

        // Parameters
        QVariantMap parameters;
        std::map<std::string, ParameterModel*>* parameterMap = getAllParameters();
        for (std::map<std::string, ParameterModel*>::iterator iter = parameterMap->begin();
                iter != parameterMap->end(); ++iter) {
            parameters.insert(QString::fromStdString(iter->second->getName()),
                    QVariant(QString::fromStdString(iter->second->getValue())));
        }
        data.insert("PARAMETERS", parameters);

        // Branches
        QVariantMap qBranches;
        for (std::map<std::string, StatementModel*>::iterator it = this->branches->begin();
                it != this->branches->end(); ++it) {
            if (it->second != NULL) {
                qBranches.insert(QString::fromStdString(it->first), it->second->toQVariant());
            }
        }
        data.insert("BRANCHES", qBranches);

        // Next statement
        if (!standalone && this->getNextStatement() != NULL) {
            data.insert("ACTIONS", this->getNextStatement()->toQVariant());
        }

        return QVariant(data);
    }

    StatementDBModel* StatementDBModel::create(DATABASE_MODE mode, QVariant dataVariant)
    {
        std::string type = "";

        if (dataVariant.type() == QVariant::Map) {
            QVariantMap data = dataVariant.toMap();
            // Load type
            if (data.contains("TYPE")) {
                QVariant qType = data.value("TYPE");
                if (qType.type() == QVariant::String) {
                    type = qType.toString().toStdString();
                } else {
                    LOG->warning("Import: Skipping statement TYPE, not of type String");
                }
            } else {
                LOG->warning("Import: Skipping statement TYPE, not existent");
            }
        } else {
            LOG->warning("Import: Skipping statement data, not of type map");
        }

        StatementDBModel* model = create(mode, type, NULL, NULL, NULL, NULL);

        if (dataVariant.type() == QVariant::Map) {
            QVariantMap data = dataVariant.toMap();

            // Insert parameters
            if (data.contains("PARAMETERS")) {
                QVariant qParams = data.value("PARAMETERS");
                if (qParams.type() == QVariant::Map) {
                    QVariantMap params = qParams.toMap();
                    for (QVariantMap::const_iterator it = params.begin(); it != params.end();
                            ++it) {
                        if (it.value().type() == QVariant::String) {
                            model->setParameter(it.key().toStdString(),
                                    it.value().toString().toStdString());
                        } else {
                            LOG->warning("Import: Skipping parameter " + it.key().toStdString());
                        }
                    }
                } else {
                    LOG->warning("Import: Skipping PARAMETERS, not of type QVariantMap");
                }
            } else {
                LOG->warning("Import: Skipping PARAMETERS, not existent");
            }

            // Load branches
            if (data.contains("BRANCHES")) {
                QVariant qBranches = data.value("BRANCHES");
                if (qBranches.type() == QVariant::Map) {
                    QVariantMap branches = qBranches.toMap();
                    for (QVariantMap::const_iterator it = branches.begin(); it != branches.end();
                            ++it) {
                        model->setBranchStatement(it.key().toStdString(), create(mode, it.value()));
                    }
                } else {
                    LOG->warning("Import: Skipping BRANCHES, not of type QVariantMap");
                }
            } else {
                LOG->warning("Import: Skipping BRANCHES, not existent");
            }

            // Load next statement
            if (data.contains("ACTIONS")) {
                StatementModel* nextStatement = create(mode, data.value("ACTIONS"));
                model->setNextStatement(nextStatement);
            }
        }

        return model;
    }

    StatementDBModel* StatementDBModel::create(DATABASE_MODE mode, std::string type,
            StatementModel* previousStatement, std::map<std::string, StatementModel*>* branches,
            StatementModel* nextStatement, std::map<std::string, ParameterModel*>* parameters)
    {
        int id = -1;
        if (mode != IN_MEMORY) {
            QSqlQuery query;
            query.prepare(
                    "INSERT INTO STATEMENT (NEXT_STATEMENT, TYPE) VALUES (:nextStatement, :type)");
            query.bindValue(":nextStatement",
                    nextStatement ? nextStatement->getId() : QVariant(QVariant::Int));
            query.bindValue(":type", QString::fromUtf8(type.c_str()));
            SqliteTools::queryExec(&query);
            id = query.lastInsertId().toInt();
        }
        return load(mode, // Database mode
                id, // Retrieve id
                type, // Statement type
                previousStatement, // Next statement Id
                branches, // Statement branches
                nextStatement, // Next statement Id
                parameters // Parameters map
                );
    }

    StatementDBModel* StatementDBModel::load(DATABASE_MODE mode, int id, std::string type,
            StatementModel* previousStatement, std::map<std::string, StatementModel*>* branches,
            StatementModel* nextStatement, std::map<std::string, ParameterModel*>* parameters)
    {
        if (NULL == branches) {
            branches = new std::map<std::string, StatementModel*>();
        }
        if (NULL == parameters) {
            parameters = new std::map<std::string, ParameterModel*>();
        }
        return new StatementDBModel(mode, id, type, previousStatement, branches, nextStatement,
                parameters);
    }

    int StatementDBModel::getId() const
    {
        return this->id;
    }

    int StatementDBModel::getPosition() const
    {
        return NULL == previousStatement ? 1 : previousStatement->getPosition() + 1;
    }

    std::string StatementDBModel::getType() const
    {
        return this->type;
    }

    void StatementDBModel::setType(std::string type)
    {
        LOG->trace("setType");
        QSqlQuery query;
        query.prepare("UPDATE STATEMENT SET TYPE = :StatementType WHERE id = :id");
        query.bindValue(":id", this->id);
        query.bindValue(":statementType", QString::fromUtf8(type.c_str()));
        SqliteTools::queryExec(&query);
        this->type = type;
    }

    StatementModel* StatementDBModel::getPreviousStatement() const
    {
        return this->previousStatement;
    }

    void StatementDBModel::setPreviousStatement(StatementModel* previousStatement)
    {
        this->previousStatement = previousStatement;
    }

    void StatementDBModel::unsetPreviousStatement()
    {
        this->previousStatement = NULL;
    }

    StatementModel* StatementDBModel::getBranchStatement(std::string key) const
    {
        if (this->branches->find(key) == this->branches->end()) {
            return NULL;
        }
        return this->branches->find(key)->second;
    }

    std::map<std::string, StatementModel*>* StatementDBModel::getBranches() const
    {
        return this->branches;
    }

    void StatementDBModel::setBranchStatement(std::string key, StatementModel* statement)
    {
        if (this->branches->find(key) != this->branches->end()) {
            unsetBranchStatement(key);
        }
        QSqlQuery query;
        query.prepare("INSERT INTO branch (parent, name, child) VALUES (:id, :name, :child)");
        query.bindValue(":id", this->id);
        query.bindValue(":name", QString::fromUtf8(key.c_str()));
        query.bindValue(":child", statement->getId());
        SqliteTools::queryExec(&query);
        this->branches->insert(std::pair<std::string, StatementModel*>(key, statement));
    }

    void StatementDBModel::unsetBranchStatement(std::string key)
    {
        if (this->branches->find(key) == this->branches->end()) {
            LOG->critical("Could not unset branch statement: doesn't exist in memory");
        }
        QSqlQuery query;
        query.prepare("DELETE FROM branch WHERE PARENT = :id AND NAME = :key");
        query.bindValue(":id", this->id);
        query.bindValue(":key", QString::fromUtf8(key.c_str()));
        SqliteTools::queryExec(&query);
        StatementModel* branchStatement = branches->find(key)->second;
        if (branchStatement)
            branchStatement->remove();
        branches->erase(key);
    }

    StatementModel* StatementDBModel::getNextStatement() const
    {
        return this->nextStatement;
    }

    void StatementDBModel::setNextStatement(StatementModel* nextStatement)
    {
        nextStatement->setPreviousStatement(this);
        QSqlQuery query;
        query.prepare("UPDATE STATEMENT SET NEXT_STATEMENT = :nextStatement WHERE id = :id");
        query.bindValue(":id", this->id);
        query.bindValue(":nextStatement", nextStatement->getId());
        SqliteTools::queryExec(&query);
        this->nextStatement = nextStatement;
    }

    void StatementDBModel::unsetNextStatement()
    {
        if (nextStatement != NULL) {
            nextStatement->unsetPreviousStatement();
        }
        QSqlQuery query;
        query.prepare("UPDATE STATEMENT SET NEXT_STATEMENT = NULL WHERE id = :id");
        query.bindValue(":id", this->id);
        SqliteTools::queryExec(&query);
        this->nextStatement = NULL;
    }

    bool StatementDBModel::hasParameter(std::string key) const
    {
        return this->parameters->find(key) != this->parameters->end();
    }

    std::string StatementDBModel::getParameter(std::string key) const
    {
        std::string val = "";
        if (this->parameters->find(key) != this->parameters->end()) {
            val = this->parameters->find(key)->second->getValue();
        }
        return val;
    }

    void StatementDBModel::setParameter(std::string key, std::string value)
    {
        if (this->parameters->find(key) == this->parameters->end()) {
            this->parameters->insert(std::pair<std::string, ParameterModel*>(key, // Name
                    (ParameterModel*) ParameterDBModel::create(READ_CACHE, // Database mode
                            key, // Name
                            value, // Value
                            this->id // Statement Id
                            )));
        } else {
            this->parameters->find(key)->second->setValue(value);
        }
    }

    std::map<std::string, ParameterModel*>* StatementDBModel::getAllParameters() const
    {
        return this->parameters;
    }

} /* namespace actiontrigger */
