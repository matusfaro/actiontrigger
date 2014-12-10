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

#include "actiontrigger/controller/services/device/actionbedsidemode.hpp"

namespace actiontrigger
{

    std::string ActionBedsideMode::TYPE = "ACTION_BEDSIDE_MODE";
    std::vector<ParameterDefinition*>* ActionBedsideMode::parameterDefinitions = NULL;
    Logger* ActionBedsideMode::StatementLOG = LoggerFactory::getInstance("ActionBedsideMode");
    const StatementInfo ActionBedsideMode::info(false, "Bedside Mode", ActionBedsideMode::TYPE,
            "Check device bedside mode.");

    const StatementInfo ActionBedsideMode::getInfo()
    {
        return ActionBedsideMode::info;
    }

    ActionBedsideMode::ActionBedsideMode(StatementModel* model) :
            Action(model)
    {
    }

    ActionBedsideMode::~ActionBedsideMode()
    {
    }

    std::string ActionBedsideMode::getType() const
    {
        return ActionBedsideMode::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionBedsideMode::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("BEDSIDE_MODE",
                        "Is bedside mode active. Either TRUE or FALSE.", "BOOL"));
        return vars;
    }

    void ActionBedsideMode::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        state->getRuntimeResources()->bpsInitialize();
        navigator_request_events(NAVIGATOR_DEVICE_LOCK_STATE);
        bps_event_t* event = state->getRuntimeResources()->bpsGetEvent(navigator_get_domain(),
                NAVIGATOR_DEVICE_LOCK_STATE);
        navigator_stop_events(NAVIGATOR_DEVICE_LOCK_STATE);
        if (event) {
            RUNLOG->debug(SSTR("Retrieved lock state event"));
            state->setUserProperty("LOCK_STATE",
                    SSTR(
                            TriggerScreenLock::lockStateToString(
                                    navigator_event_get_device_lock_state(event))));
        } else {
            RUNLOG->error("Did not receive lock state information");
        }
    }

} /* namespace actiontrigger */
