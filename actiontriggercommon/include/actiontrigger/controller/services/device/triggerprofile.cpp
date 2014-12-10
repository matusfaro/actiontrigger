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

#include "actiontrigger/controller/services/device/triggerprofile.hpp"

namespace actiontrigger
{

    std::string TriggerProfile::TYPE = "TRIGGER_TriggerProfile";
    Logger* TriggerProfile::StatementLOG = LoggerFactory::getInstance("TriggerProfile");
    const StatementInfo TriggerProfile::info(true, "Notification Profile", TriggerProfile::TYPE,
            "Triggers on the global notification profile for the device.");

    const StatementInfo TriggerProfile::getInfo()
    {
        return TriggerProfile::info;
    }

    TriggerProfile::TriggerProfile(StatementModel* model) :
            TriggerValue(model), parameterDefinitions(NULL)
    {
    }

    std::string TriggerProfile::getType() const
    {
        return TriggerProfile::TYPE;
    }

    TriggerProfile::~TriggerProfile()
    {
    }

    bool TriggerProfile::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<statement_output_variable_definition*>* TriggerProfile::getExposedVariableDefinitions()
    {
        // We don't need to expose INPUT variable
        return new std::vector<statement_output_variable_definition*>();
    }

    std::vector<trigger_output_variable_definition*>* TriggerProfile::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("PROFILE", "Profile",
                        "Current notification profile. Value will be one of NORMAL, CALLS, VIBRATE, SILENT, OFF or UNKNOWN.",
                        "VARIABLE"));
        return definition;
    }

    bool TriggerProfile::isActive(ExecutionState* state)
    {
        bb::platform::NotificationGlobalSettings settings;
        std::string currentModeString;
        switch (settings.mode()) {
            case bb::platform::NotificationMode::Normal:
                currentModeString = "NORMAL";
                break;
            case bb::platform::NotificationMode::PhoneOnly:
                currentModeString = "CALLS";
                break;
            case bb::platform::NotificationMode::Vibrate:
                currentModeString = "VIBRATE";
                break;
            case bb::platform::NotificationMode::Silent:
                currentModeString = "SILENT";
                break;
            case bb::platform::NotificationMode::AlertsOff:
                currentModeString = "OFF";
                break;
            default:
                currentModeString = "UNKNOWN";
                break;
        }
        std::vector<std::string>* values = new std::vector<std::string>();
        values->push_back(currentModeString);
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
