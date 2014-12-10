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

#ifndef TRIGGER_HPP_
#define TRIGGER_HPP_

#include <vector>
#include <map>
#include <stdexcept>
#include <string>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/exception.hpp"
#include "actiontrigger/controller/services/statement.hpp"
#include <unistd.h>
#include "actiontrigger/controller/executionstate.hpp"
#include "actiontrigger/controller/exception.hpp"
#include "actiontrigger/controller/services/action.hpp"

namespace actiontrigger
{

    enum trigger_return_status
    {
        ALL_ACTIVE, /* Returned when trigger and all children are active. */
        ALL_ACTIVE_WITHOUT_BLOCK /* Returned when trigger and all children are active. Also implies the triggers have been active right away so the callee trigger does not have to check its status again. */
//TODO    NEVER_ACTIVE /* Used when a trigger cannot possibly become active ever. */
    };

    class Trigger: public Action
    {
    public:
        Trigger(StatementModel* model);
        virtual ~Trigger();

        bool checkParameterValidityAndAllChildren(ExecutionState* state = NULL);
        virtual Trigger* getNextTrigger() const;
        virtual Trigger* getLastTrigger() const;

        // Non-blocking check if all triggers are in active state
        bool checkTriggerStateOperatorAnd(ExecutionState* state);

        // Non-blocking check if at least one trigger is in active state
        bool checkTriggerStateOperatorOr(ExecutionState* state);

        // Block until this trigger and all descendant triggers are in active state
        trigger_return_status blockTriggerState(ExecutionState* state);
    protected:
        // Every trigger can be an action as well
        virtual void execute(ExecutionState* state);
        // Block Trigger state algorithms to determine if all triggers are active
        void blockTriggerStateAlgoSimple(ExecutionState* state);
        trigger_return_status blockTriggerStateAlgoTwoPointOh(ExecutionState* state);
        // Non-blocking check for active state
        virtual bool isActive(ExecutionState* state) = 0;
        // Event only triggers fire only at an event
        // isActive() method for these triggers usually always return false
        virtual bool isEventOnlyTrigger() = 0;
        // Block until state is active
        // Non overloaded method continuously checks for activeness using isActive
        // It waits GRANULARITY time in milliseconds in between checks
        virtual void blockUntilActive(ExecutionState* state);
        static Logger* LOG;
    private:
        // Non-blocking check for active state and all children
        bool areActiveAllChildren(ExecutionState* state);
        // Non-blocking check for active state of at least one child
        bool isActiveOneChild(ExecutionState* state);
    };

} /* namespace actiontrigger */
#endif /* TRIGGER_HPP_ */
