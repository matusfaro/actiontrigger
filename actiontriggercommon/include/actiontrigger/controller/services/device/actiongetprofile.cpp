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

#include "actiontrigger/controller/services/device/actiongetprofile.hpp"

namespace actiontrigger
{

    std::string ActionGetProfile::TYPE = "ACTION_ActionGetProfile";

    std::vector<ParameterDefinition*>* ActionGetProfile::parameterDefinitions = NULL;
    Logger* ActionGetProfile::StatementLOG = LoggerFactory::getInstance("ActionGetProfile");
    const StatementInfo ActionGetProfile::info(false, "Get Notification Profile",
            ActionGetProfile::TYPE, "Retrieve the global notification profile for the device.");

    const StatementInfo ActionGetProfile::getInfo()
    {
        return ActionGetProfile::info;
    }

    ActionGetProfile::ActionGetProfile(StatementModel* model) :
            Action(model)
    {
    }

    ActionGetProfile::~ActionGetProfile()
    {
    }

    std::vector<ParameterDefinition*>* ActionGetProfile::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
        }
        return parameterDefinitions;
    }

    std::string ActionGetProfile::getType() const
    {
        return ActionGetProfile::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGetProfile::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("PROFILE",
                        "Current notification profile. Value will be one of NORMAL, CALLS, VIBRATE, SILENT, OFF or UNKNOWN.",
                        "STRING"));
        return vars;
    }

    void ActionGetProfile::execute(ExecutionState* state)
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
        state->setUserProperty("PROFILE", currentModeString);
    }

} /* namespace actiontrigger */
