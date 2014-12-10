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

#ifndef STATEMENTMODEL_HPP_
#define STATEMENTMODEL_HPP_

#include <string>
#include <vector>
#include "actiontrigger/model/database/parametermodel.hpp"
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>

namespace actiontrigger
{

    class StatementModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~StatementModel()
        {
        }
        ;
        /*
         * Delete permanently
         */
        virtual void remove() = 0;
        /*
         * Getters and setters
         */
        virtual int getId() const = 0;
        virtual int getPosition() const = 0;
        virtual std::string getType() const = 0;
        virtual void setType(std::string type) = 0;

        virtual StatementModel* getPreviousStatement() const = 0;
        virtual void setPreviousStatement(StatementModel* previousStatement) = 0;
        virtual void unsetPreviousStatement() = 0;

        virtual StatementModel* getBranchStatement(std::string key) const = 0;
        virtual std::map<std::string, StatementModel*>* getBranches() const = 0;
        virtual void setBranchStatement(std::string key, StatementModel* statement) = 0;
        virtual void unsetBranchStatement(std::string key) = 0;

        virtual StatementModel* getNextStatement() const = 0;
        virtual void setNextStatement(StatementModel*) = 0;
        virtual void unsetNextStatement() = 0;

        virtual bool hasParameter(std::string key) const = 0;
        virtual std::string getParameter(std::string key) const = 0;
        virtual void setParameter(std::string key, std::string value) = 0;
        virtual std::map<std::string, ParameterModel*>* getAllParameters() const = 0;

        virtual QVariant toQVariant(bool standalone = false) const = 0;
    };

} /* namespace actiontrigger */
#endif /* STATEMENTMODEL_HPP_ */
