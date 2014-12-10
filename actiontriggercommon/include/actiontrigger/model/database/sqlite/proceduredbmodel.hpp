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

#ifndef PROCEDUREDBMODEL_HPP_
#define PROCEDUREDBMODEL_HPP_

#include <string>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/database/sqlite/statementdbmodel.hpp"
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>
#include "actiontrigger/model/database/proceduremodel.hpp"
#include "actiontrigger/model/database/databasecommon.hpp"
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"

namespace actiontrigger
{

    class ProcedureDBModel: public ProcedureModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~ProcedureDBModel();
        /*
         * Create Procedure and save into database
         */
        static ProcedureModel* create(DATABASE_MODE mode, std::string name,
                StatementModel* statement);
        static ProcedureModel* create(DATABASE_MODE mode, QVariant data);
        /*
         * Load Procedure into memory
         */
        static ProcedureModel* load(DATABASE_MODE mode, int id, std::string name,
                StatementModel* statement, std::map<std::string, std::string>* settings = NULL);
        /*
         * Delete permanently
         */
        void remove();
        /*
         * Getters and setters
         */
        int getId() const;

        std::string getName() const;
        void setName(std::string name);

        StatementModel* getStatement() const;
        void setStatement(StatementModel* statement);
        void unsetStatement();

        std::string getSetting(std::string name) const;
        void setSetting(std::string name, std::string value);

        virtual QVariant toQVariant() const;
    private:
        DATABASE_MODE mode;
        int id;
        std::string name;
        // First statement
        StatementModel* statement;
        std::map<std::string, std::string>* settings;
        ProcedureDBModel(DATABASE_MODE mode, int id, std::string name, StatementModel* statement,
                std::map<std::string, std::string>* settings = NULL);
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PROCEDUREDBMODEL_HPP_ */
