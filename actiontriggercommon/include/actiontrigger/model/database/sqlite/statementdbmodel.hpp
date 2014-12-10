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

#ifndef STATEMENTDBMODEL_HPP_
#define STATEMENTDBMODEL_HPP_

#include <QVariant>
#include <string>
#include <vector>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/database/sqlite/parameterdbmodel.hpp"
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/model/database/parametermodel.hpp"
#include "actiontrigger/model/database/databasecommon.hpp"
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"

namespace actiontrigger
{

    class StatementDBModel: public StatementModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~StatementDBModel();
        /*
         * Create Statement and save into database
         */
        static StatementDBModel* create(DATABASE_MODE mode, QVariant data);
        static StatementDBModel* create(DATABASE_MODE mode, std::string type,
                StatementModel* previousStatement = NULL,
                std::map<std::string, StatementModel*>* branches = NULL,
                StatementModel* nextStatement = NULL,
                std::map<std::string, ParameterModel*>* parameters = NULL);
        /*
         * Load Statement into memory
         */
        static StatementDBModel* load(DATABASE_MODE mode, int id, std::string type,
                StatementModel* previousStatement = NULL,
                std::map<std::string, StatementModel*>* branches = NULL,
                StatementModel* nextStatement = NULL,
                std::map<std::string, ParameterModel*>* parameters = NULL);
        /*
         * Delete permanently
         */
        void remove();
        /*
         * Getters and setters
         */
        int getId() const;
        int getPosition() const;
        std::string getType() const;
        void setType(std::string type);

        StatementModel* getPreviousStatement() const;
        void setPreviousStatement(StatementModel* previousStatement);
        void unsetPreviousStatement();

        StatementModel* getBranchStatement(std::string key) const;
        std::map<std::string, StatementModel*>* getBranches() const;
        void setBranchStatement(std::string key, StatementModel* statement);
        void unsetBranchStatement(std::string key);

        StatementModel* getNextStatement() const;
        void setNextStatement(StatementModel* nextStatement);
        void unsetNextStatement();

        bool hasParameter(std::string key) const;
        std::string getParameter(std::string key) const;
        void setParameter(std::string key, std::string value);
        std::map<std::string, ParameterModel*>* getAllParameters() const;

        virtual QVariant toQVariant(bool standalone = false) const;
    private:
        DATABASE_MODE mode;
        int id;
        int position;
        std::string type;
        StatementModel* previousStatement;
        std::map<std::string, StatementModel*>* branches;
        StatementModel* nextStatement;
        std::map<std::string, ParameterModel*>* parameters;
        StatementDBModel(DATABASE_MODE mode, int id, std::string type,
                StatementModel* previousStatement, std::map<std::string, StatementModel*>* branches,
                StatementModel* nextStatement, std::map<std::string, ParameterModel*>* parameters);
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* STATEMENTDBMODEL_HPP_ */
