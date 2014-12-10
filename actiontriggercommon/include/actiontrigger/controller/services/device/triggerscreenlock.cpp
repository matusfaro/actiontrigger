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

#include "actiontrigger/controller/services/device/triggerscreenlock.hpp"

namespace actiontrigger
{

    std::string TriggerScreenLock::TYPE = "TRIGGER_SCREEN_LOCK";
    Logger* TriggerScreenLock::StatementLOG = LoggerFactory::getInstance("TriggerScreenLock");
    const StatementInfo TriggerScreenLock::info(true, "Screen lock", TriggerScreenLock::TYPE,
            "Trigger based on device lock state.");

    const StatementInfo TriggerScreenLock::getInfo()
    {
        return TriggerScreenLock::info;
    }

    TriggerScreenLock::TriggerScreenLock(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerScreenLock::getType() const
    {
        return TriggerScreenLock::TYPE;
    }

    TriggerScreenLock::~TriggerScreenLock()
    {
    }

    bool TriggerScreenLock::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerScreenLock::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("LOCK_STATE", "Lock state",
                        "Retrieve the device lock state. One of UNLOCKED, SCREEN_LOCKED, and PASSWORD_LOCKED.",
                        "STRING"));
        return definition;
    }

    bool TriggerScreenLock::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        state->getRuntimeResources()->bpsInitialize();
        navigator_request_events(NAVIGATOR_DEVICE_LOCK_STATE);
        bps_event_t* event = state->getRuntimeResources()->bpsGetEvent(navigator_get_domain(),
                NAVIGATOR_DEVICE_LOCK_STATE);
        navigator_stop_events(NAVIGATOR_DEVICE_LOCK_STATE);
        if (event) {
            RUNLOG->debug(SSTR("Retrieved lock state event"));
            values->push_back(
                    /*LOCK_STATE*/SSTR(
                            lockStateToString(navigator_event_get_device_lock_state(event))));
            return checkRequirements(state, values);
        } else {
            RUNLOG->error("Did not receive lock state information from system");
            return false;
        }
    }

    std::string TriggerScreenLock::lockStateToString(int state)
    {
        switch (state) {
            case NAVIGATOR_DEVICE_LOCK_STATE_UNLOCKED:
                return "UNLOCKED";
            case NAVIGATOR_DEVICE_LOCK_STATE_SCREEN_LOCKED:
                return "SCREEN_LOCKED";
            case NAVIGATOR_DEVICE_LOCK_STATE_PASSWORD_LOCKED:
                return "PASSWORD_LOCKED";
            default:
                return "UNKNOWN";
        }
    }

} /* namespace actiontrigger */
