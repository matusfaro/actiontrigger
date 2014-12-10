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

#ifndef SERVICE_HPP_
#define SERVICE_HPP_

#include <string>
#include <map>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/statementfactory.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/branch.hpp"

namespace actiontrigger
{

    class UnknownStatementException: public std::runtime_error
    {
    public:
        UnknownStatementException(std::string msg) :
                std::runtime_error(msg)
        {
        }
    };

    /**
     * OBSOLETE
     */
    class Service
    {
    public:
        virtual ~Service();
        void setup();
        std::map<std::string, StatementInfo>* getTriggerInfos();
        std::map<std::string, StatementInfo>* getActionInfos();
        Trigger* getTriggerStatement(StatementModel* model);
        Action* getActionStatement(StatementModel* model);

        virtual int getId() const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getType() const = 0;
        virtual std::string getDescription() const = 0;
        int getTriggerCount() const;
        int getActionCount() const;
    protected:
        virtual std::map<std::string, StatementFactory<Trigger>*>* getTriggers() = 0;
        virtual std::map<std::string, StatementFactory<Action>*>* getActions() = 0;
        virtual std::map<std::string, StatementFactory<Branch>*>* getBranches();
    private:
        int id;
        std::string name;
        std::map<std::string, StatementFactory<Trigger>*>* triggers;
        std::map<std::string, StatementFactory<Action>*>* actions;
        std::map<std::string, StatementFactory<Branch>*>* branches;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* SERVICE_HPP_ */
