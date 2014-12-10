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

#ifndef PROCEDUREMODEL_HPP_
#define PROCEDUREMODEL_HPP_

#include <string>
#include "actiontrigger/model/database/statementmodel.hpp"
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/model/database/proceduremodel.hpp"

namespace actiontrigger
{

//enum {
//    NO_CACHE, CACHE_READ, CACHE_READ_WRITE
//};

    class ProcedureModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~ProcedureModel()
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

        virtual std::string getName() const = 0;
        virtual void setName(std::string name) = 0;

        virtual StatementModel* getStatement() const = 0;
        virtual void setStatement(StatementModel* statement) = 0;
        virtual void unsetStatement() = 0;

        virtual std::string getSetting(std::string name) const = 0;
        virtual void setSetting(std::string name, std::string value) = 0;

        virtual QVariant toQVariant() const = 0;
    };

} /* namespace actiontrigger */
#endif /* PROCEDUREMODEL_HPP_ */
