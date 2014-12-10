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

#include "actiontrigger/controller/services/phone/triggercallupdated.hpp"

namespace actiontrigger
{

    std::string TriggerCallUpdated::TYPE = "TRIGGER_TriggerCallUpdated";
    Logger* TriggerCallUpdated::StatementLOG = LoggerFactory::getInstance("TriggerCallUpdated");
    const StatementInfo TriggerCallUpdated::info(true, "Call Updates", TriggerCallUpdated::TYPE,
            "Listens for phone call changes.");

    const StatementInfo TriggerCallUpdated::getInfo()
    {
        return TriggerCallUpdated::info;
    }

    TriggerCallUpdated::TriggerCallUpdated(StatementModel* model) :
            TriggerValue(model), parameterDefinitions(NULL)
    {
    }

    std::string TriggerCallUpdated::getType() const
    {
        return TriggerCallUpdated::TYPE;
    }

    TriggerCallUpdated::~TriggerCallUpdated()
    {
    }

    bool TriggerCallUpdated::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<statement_output_variable_definition*>* TriggerCallUpdated::getExposedVariableDefinitions()
    {
        // We don't need to expose INPUT variable
        return new std::vector<statement_output_variable_definition*>();
    }

    std::vector<ParameterDefinition*>* TriggerCallUpdated::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            // Get Parent's definitions for requirements
            parameterDefinitions = TriggerValue::getParameterDefinitions();

            parameterDefinitions->insert(parameterDefinitions->begin(),
                    ParameterDefinition::getCheckbox("ENABLE_QUEUE", "Queue updates",
                            RichText::html(
                                    "If checked, all the updates will be processed including updates received during other action(s) execution. Useful if you want to process ALL updates."
                                            + RichText::newline()
                                            + "If unchecked, all previous updates that occurred before this trigger is executed will not be considered.")));
        }
        return parameterDefinitions;
    }

    std::string TriggerCallUpdated::getDefaultParameter(std::string key)
    {
        if (key == "ENABLE_QUEUE") {
            return "TRUE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::vector<trigger_output_variable_definition*>* TriggerCallUpdated::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        // 10.3
        //definition->push_back(new trigger_output_variable_definition("CALL_LINE", "Call Line", "Call line. Value will be one of Invalid, Cellular, MVS or VideoChat.", "VARIABLE"));
        definition->push_back(
                new trigger_output_variable_definition("CALL_STATE", "Call State",
                        "State of the call. Value will be one of Invalid, Unknown, Incoming, Connecting, RemoteRinging, Connected, Disconnected, Failed, Held or MultiPartyMember.",
                        "VARIABLE"));
        definition->push_back(
                new trigger_output_variable_definition("CALL_TYPE", "Call Type",
                        "Type of call. Value will be one of Invalid, Incoming, Outgoing, MultiParty, Missed, Command or Emergency.",
                        "VARIABLE"));
        return definition;
    }

    bool TriggerCallUpdated::isActive(ExecutionState* state)
    {
        return trigger(state, false);
    }

    void TriggerCallUpdated::blockUntilActive(ExecutionState* state)
    {
        trigger(state, true);
    }

    bool TriggerCallUpdated::trigger(ExecutionState* state, bool block)
    {
        LOG->trace("trigger(state,block)");
        PhoneService* phoneService = state->getRuntimeResources()->getPhoneService();
        bool enableQueue = isValueTrue(getParameter("ENABLE_QUEUE", state));

        // Continuously check if there are new messages
        const bb::system::phone::Call* nextUpdate = NULL;

        if (block) {
            // Block until message is available
            while (true) {
                nextUpdate = phoneService->getNext();
                state->giveUpIfNotRunning();
                if (nextUpdate) {
                    if (checkCall(state, nextUpdate)) {
                        if (!enableQueue)
                            phoneService->disconnectReceiver();
                        return true;
                    }
                    nextUpdate = NULL;
                }
            }
        } else {
            // Check if there are new messages without waiting
            nextUpdate = phoneService->getNext(0);
            if (!enableQueue)
                phoneService->disconnectReceiver();
            return nextUpdate && checkCall(state, nextUpdate);
        }
    }

    bool TriggerCallUpdated::checkCall(ExecutionState* state, const bb::system::phone::Call* call)
    {
        LOG->trace("checkCall(state,call)");
        std::string callState = PhoneService::callStateToString(call->callState());
        std::string callType = PhoneService::callTypeToString(call->callType());
        state->getLogger()->debug(SSTR("Call update, type=" << callType << " state=" << callState));

        std::vector<std::string>* values = new std::vector<std::string>();
        // 10.3
        //values->push_back(PhoneService::lineTypeToString(call->callLine()));
        values->push_back(callState);
        values->push_back(callType);
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
