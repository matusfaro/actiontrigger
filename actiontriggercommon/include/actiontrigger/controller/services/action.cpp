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

#include "actiontrigger/controller/services/action.hpp"

namespace actiontrigger
{

    Logger* Action::LOG = LoggerFactory::getInstance("Action");

    Action::Action(StatementModel* model) :
            Statement(model)
    {
        LOG->trace("Action");
    }

    void Action::initBranches(std::map<std::string, Trigger*>* triggerBranches,
            std::map<std::string, Action*>* actionBranches)
    {
        (void) triggerBranches;
        (void) actionBranches;
    }

    Action::~Action()
    {
        LOG->trace("~Action");
    }

    Action* Action::getLastAction() const
    {
        LOG->trace("getLastAction");
        return (Action*) getLastStatement();
    }

    Action* Action::getNextAction() const
    {
        LOG->trace("getNextAction");
        return (Action*) getNextStatement();
    }

    bool Action::checkParameterValidityAndAllChildren(ExecutionState* state)
    {
        bool validity;
        if (NULL != state) {
            DataModelLogger* RUNLOG = state->getLogger();
            RUNLOG->pushLayer(this->getType());
            validity = checkParameterValidity(state);
            RUNLOG->popLayer();
        } else {
            validity = checkParameterValidity(state);
        }
        return validity
                && (!getNextStatement()
                        || getNextStatement()->checkParameterValidityAndAllChildren(state));
    }

    void Action::run(ExecutionState* state)
    {
        LOG->trace("run");
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->pushLayer(this->getType());
        // Execute this action
        RUNLOG->debug("Execute");
        LOG->trace("Execute action");
        this->execute(state);
        // Execute children
        if (getNextStatement()) {
            // Check if we should continue
            if (!state->isRunning())
                throw ExecutionAbortedException();
            // If action returns true, run the next action
            LOG->trace("execute child Action");
            RUNLOG->popLayer();
            getNextAction()->run(state);
        } else {
            RUNLOG->popLayer();
            LOG->trace("No Child action, returning");
        }
    }

} /* namespace actiontrigger */
