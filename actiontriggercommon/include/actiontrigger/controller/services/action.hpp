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

#ifndef ACTION_HPP_
#define ACTION_HPP_

#include "actiontrigger/controller/services/statement.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/exception.hpp"
#include "actiontrigger/controller/executionstate.hpp"

namespace actiontrigger
{

    class Action: public Statement
    {
    public:
        Action(StatementModel* model);
        virtual void initBranches(std::map<std::string, Trigger*>* triggerBranches,
                std::map<std::string, Action*>* actionBranches);
        virtual ~Action();

        bool checkParameterValidityAndAllChildren(ExecutionState* state = NULL);
        virtual Action* getNextAction() const;
        virtual Action* getLastAction() const;

        void run(ExecutionState* state);
    protected:
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* ACTION_HPP_ */
