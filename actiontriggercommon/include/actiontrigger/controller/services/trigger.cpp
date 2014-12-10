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

#include "actiontrigger/controller/services/trigger.hpp"

namespace actiontrigger
{

    Logger* Trigger::LOG = LoggerFactory::getInstance("Trigger");

    Trigger::Trigger(StatementModel* model) :
            Action(model)
    {
        LOG->trace("Trigger");
    }

    Trigger::~Trigger()
    {
        LOG->trace("~Trigger");
        // No-op
    }

    Trigger* Trigger::getLastTrigger() const
    {
        LOG->trace("getLastTrigger");
        return (Trigger*) getLastStatement();
    }

    Trigger* Trigger::getNextTrigger() const
    {
        LOG->trace("getNextTrigger");
        return (Trigger*) getNextStatement();
    }

    bool Trigger::checkParameterValidityAndAllChildren(ExecutionState* state)
    {
        bool validity;
        if (state)
            state->getLogger()->pushLayer(this->getType());
        validity = checkParameterValidity(state);
        if (state)
            state->getLogger()->popLayer();
        return validity
                && (!getNextStatement()
                        || getNextStatement()->checkParameterValidityAndAllChildren(state));
    }

// Ran as action instead
    void Trigger::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->trace("Running trigger as action, wait until trigger is active");
        blockUntilActive(state);
        RUNLOG->debug("Trigger is active");
    }

    void Trigger::blockUntilActive(ExecutionState* state)
    {
        useconds_t granularity = getGranularity(state);
        // Assuming granularity is in milliseconds, convert to microseconds
        while (true) {
            state->giveUpIfNotRunning();
            if (isActive(state))
                return;
            state->giveUpIfNotRunning();
            if (granularity != 0)
                usleep(granularity);
        }
    }

// Non-blocking check if all triggers are in active state
    bool Trigger::checkTriggerStateOperatorAnd(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->pushLayer(this->getType());
        bool isActive = this->isActive(state);
        RUNLOG->popLayer();
        return isActive && areActiveAllChildren(state);
    }

// Non-blocking check if at least one trigger is in active state
    bool Trigger::checkTriggerStateOperatorOr(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->pushLayer(this->getType());
        bool isActive = this->isActive(state);
        RUNLOG->popLayer();
        return isActive || isActiveOneChild(state);
    }

    trigger_return_status Trigger::blockTriggerState(ExecutionState* state)
    {
        return this->blockTriggerStateAlgoTwoPointOh(state);
    }

// Very sophisticated algorithm to make sure all triggers are active at the same time.
// This has support for both instantenous triggers and state triggers.
// It ensures the instantenous triggers are not checked for activity rapidly.
    void Trigger::blockTriggerStateAlgoSimple(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->pushLayer(this->getType());
        LOG->trace(
                SSTR(
                        "id" << this->model->getId() << ": " << this->getInfo().name
                                << ": blockTriggerState()"));
        while (true) {
            // Wait until our trigger is active
            RUNLOG->debug("Block until active");
            LOG->trace(
                    SSTR(
                            "id" << this->model->getId() << ": " << this->getInfo().name
                                    << ": Wait until our trigger is active"));
            blockUntilActive(state);
            // Check if we should continue
            if (!state->isRunning())
                throw ExecutionAbortedException();
            // Our trigger was activated, let's check our child trigger state
            RUNLOG->debug("Active!");
            LOG->trace(
                    SSTR(
                            "id" << this->model->getId() << ": " << this->getInfo().name
                                    << ": Our trigger is activated, let's check our child trigger state"));
            RUNLOG->popLayer();
            if (this->areActiveAllChildren(state)) {
                RUNLOG->pushLayer(this->getType());
                // Our child and all its descendants are active
                RUNLOG->debug("All Triggers active!");
                LOG->trace(
                        SSTR(
                                "id" << this->model->getId() << ": " << this->getInfo().name
                                        << ": Our child and all its descendants are active"));
                RUNLOG->popLayer();
                return;
            } else {
                // One or more of our descendants are not active
                // Block until all our descendants are active
                LOG->trace(
                        SSTR(
                                "id" << this->model->getId() << ": " << this->getInfo().name
                                        << ": One or more of our descendants are not active. Block until all our descendants are active"));
                // TODO If some children are already active, we should block on those children instead of blocking on the immediate child which may have returned is active already.
                getNextTrigger()->blockTriggerState(state);
                RUNLOG->pushLayer(this->getType());
                // Check if we should continue
                if (!state->isRunning())
                    throw ExecutionAbortedException();
                // All our descendants are active
                // let's double check that our state is still active
                LOG->trace(
                        SSTR(
                                "id" << this->model->getId() << ": " << this->getInfo().name
                                        << ": All our descendants are active. let's double check that our state is still active"));
                if (isActive(state)) {
                    // Our state is active, return from method
                    RUNLOG->debug(SSTR("TRIGGER: " << this->getType() << ": Active!"));
                    LOG->trace(
                            SSTR(
                                    "id" << this->model->getId() << ": " << this->getInfo().name
                                            << ": Our state is active, return from method."));
                    RUNLOG->popLayer();
                    return;
                }
                RUNLOG->debug(SSTR("TRIGGER: " << this->getType() << ": Not active!"));
                // Our state has indeed changed while blocked for our descendants
                // Let's do this all over again in next iteration of the loop
                LOG->trace(
                        SSTR(
                                "id" << this->model->getId() << ": " << this->getInfo().name
                                        << ": Our state has indeed changed while blocked for our descendants. Let's do this all over again in next iteration of the loop"));
            }
        }
    }

// Non-blocking check for active state of all children
    bool Trigger::areActiveAllChildren(ExecutionState* state)
    {
        LOG->trace(
                SSTR(
                        "id" << this->model->getId() << ": " << this->getInfo().name
                                << ": areActiveAllChildren()"));
        DataModelLogger* RUNLOG = state->getLogger();
        Trigger* childTrigger = getNextTrigger();
        while (childTrigger != NULL) {
            RUNLOG->pushLayer(childTrigger->getType());
            if (!childTrigger->isActive(state)) {
                RUNLOG->popLayer();
                return false;
            }
            RUNLOG->popLayer();
            childTrigger = childTrigger->getNextTrigger();
            // Check if we should continue
            if (!state->isRunning())
                throw ExecutionAbortedException();
        }
        return true;
    }

// Non-blocking check for active state of all children
    bool Trigger::isActiveOneChild(ExecutionState* state)
    {
        LOG->trace(
                SSTR(
                        "id" << this->model->getId() << ": " << this->getInfo().name
                                << ": isActiveOneChild()"));
        DataModelLogger* RUNLOG = state->getLogger();
        Trigger* childTrigger = getNextTrigger();
        while (childTrigger != NULL) {
            RUNLOG->pushLayer(childTrigger->getType());
            if (childTrigger->isActive(state)) {
                RUNLOG->popLayer();
                return true;
            }
            RUNLOG->popLayer();
            childTrigger = childTrigger->getNextTrigger();
            // Check if we should continue
            if (!state->isRunning())
                throw ExecutionAbortedException();
        }
        return false;
    }

// Even more sophisticated algorithm than the previous one that checks triggers more
// efficiently without unnecessary calls
    trigger_return_status Trigger::blockTriggerStateAlgoTwoPointOh(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->pushLayer(this->getType());
        LOG->trace(
                SSTR(
                        "id" << this->model->getId() << ": " << this->getInfo().name
                                << ": blockTriggerState()"));
        bool hasBeenBlocked = false;
        bool hasChild = getNextStatement() != NULL;
        bool childIsActive = false;
        while (true) {
            state->giveUpIfNotRunning(); // Check if we should continue
            RUNLOG->trace("Checking if trigger is already active");
            if (isActive(state)) {
                RUNLOG->debug("Trigger is active");
                // This only happens on subsequent iterations of while loop
                if (childIsActive) {
                    // If our child is active and we haven't returned yet it
                    // implies we have been blocked and we don't need to check here
                    RUNLOG->popLayer();
                    return ALL_ACTIVE;
                }
            } else {
                // For event only triggers, isActive will almost always return false. Let's not print anything to the user.
                if (!isEventOnlyTrigger())
                    RUNLOG->debug("Trigger is not active, waiting...");
                this->blockUntilActive(state);
                state->giveUpIfNotRunning(); // Check if we should continue
                RUNLOG->debug("Trigger activated");
                hasBeenBlocked = true;
            }
            if (!hasChild) {
                RUNLOG->trace("Trigger has no child triggers to check, let's successfully return");
                // If no children, return right away
                RUNLOG->popLayer();
                if (hasBeenBlocked)
                    return ALL_ACTIVE;
                else
                    return ALL_ACTIVE_WITHOUT_BLOCK;
            }

            RUNLOG->trace("Trigger has child triggers, let's check them...");
            RUNLOG->popLayer();
            trigger_return_status childStatus = getNextTrigger()->blockTriggerStateAlgoTwoPointOh(
                    state);
            state->giveUpIfNotRunning(); // Check if we should continue
            RUNLOG->pushLayer(this->getType());

            if (childStatus == ALL_ACTIVE_WITHOUT_BLOCK) {
                RUNLOG->trace("Child trigger is active right away, let's successfully return");
                RUNLOG->popLayer();
                if (hasBeenBlocked)
                    return ALL_ACTIVE;
                else
                    return ALL_ACTIVE_WITHOUT_BLOCK;
            } else if (childStatus == ALL_ACTIVE) {
                RUNLOG->trace(
                        "Child trigger is active, but our state may have changed, let's check...");
                childIsActive = true;
                hasBeenBlocked = true;
            }
        }
        // Never happens
        return ALL_ACTIVE;
    }

}
/* namespace actiontrigger */
