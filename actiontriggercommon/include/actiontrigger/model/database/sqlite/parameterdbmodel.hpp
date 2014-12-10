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

#ifndef PARAMETERDBMODEL_HPP_
#define PARAMETERDBMODEL_HPP_

#include <string>
#include "actiontrigger/model/loggerfactory.hpp"
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>
#include "actiontrigger/model/database/parametermodel.hpp"
#include "actiontrigger/model/database/databasecommon.hpp"
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"
#include <QSqlError>

namespace actiontrigger
{

    class ParameterDBModel: ParameterModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~ParameterDBModel();
        /*
         * Create parameter and save into database
         */
        static ParameterDBModel* create(DATABASE_MODE mode, std::string name, std::string value,
                int statementId);
        /*
         * Load Parameter into memory
         */
        static ParameterDBModel* load(DATABASE_MODE mode, int id, std::string name,
                std::string value, int statementId);
        /*
         * Delete permanently
         */
        void remove();
        /*
         * Getters and setters
         */
        int getId() const;
        std::string getValue() const;
        void setValue(std::string value);
        std::string getName() const;
        void setName(std::string name);
    private:
        DATABASE_MODE mode;
        int id;
        std::string name;
        std::string value;
        int statementId;
        ParameterDBModel(DATABASE_MODE mode, int id, std::string name, std::string value,
                int statementId);
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PARAMETERDBMODEL_HPP_ */
