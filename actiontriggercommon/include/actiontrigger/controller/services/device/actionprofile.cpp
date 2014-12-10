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

#include "actiontrigger/controller/services/device/actionprofile.hpp"

namespace actiontrigger
{

    std::string ActionProfile::TYPE = "ACTION_ActionProfile";

    std::vector<ParameterDefinition*>* ActionProfile::parameterDefinitions = NULL;
    Logger* ActionProfile::StatementLOG = LoggerFactory::getInstance("ActionProfile");
    const StatementInfo ActionProfile::info(false, "Set Notification Profile", ActionProfile::TYPE,
            "Change the global notification profile for the device.");

    const StatementInfo ActionProfile::getInfo()
    {
        return ActionProfile::info;
    }

    ActionProfile::ActionProfile(StatementModel* model) :
            Action(model)
    {
    }

    ActionProfile::~ActionProfile()
    {
    }

    std::vector<ParameterDefinition*>* ActionProfile::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("PROFILE", "Profile",
                    "Profile mode to switch to.", "Desired Profile");
            state->dropdownAddOption("Normal", "NORMAL");
            state->dropdownAddOption("Phone Calls Only", "CALLS");
            state->dropdownAddOption("Vibrate Only", "VIBRATE");
            state->dropdownAddOption("Silent", "SILENT");
            state->dropdownAddOption("All Alerts Off", "OFF");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionProfile::getDefaultParameter(std::string key)
    {
        if (key == "PROFILE") {
            return "NORMAL";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionProfile::getType() const
    {
        return ActionProfile::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionProfile::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionProfile::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string paramProfile = getParameter("PROFILE", state);
        bb::platform::NotificationMode::Type desiredProfile;
        if (paramProfile.compare("NORMAL") == 0) {
            desiredProfile = bb::platform::NotificationMode::Normal;
        } else if (paramProfile.compare("CALLS") == 0) {
            desiredProfile = bb::platform::NotificationMode::PhoneOnly;
        } else if (paramProfile.compare("VIBRATE") == 0) {
            desiredProfile = bb::platform::NotificationMode::Vibrate;
        } else if (paramProfile.compare("SILENT") == 0) {
            desiredProfile = bb::platform::NotificationMode::Silent;
        } else if (paramProfile.compare("OFF") == 0) {
            desiredProfile = bb::platform::NotificationMode::AlertsOff;
        } else {
            RUNLOG->error("Parameter PROFILE is incorrect");
            return;
        }
        bb::platform::NotificationGlobalSettings settings;
        bb::platform::NotificationSettingsError::Type retval = settings.setMode(desiredProfile);
        switch (retval) {
            case bb::platform::NotificationSettingsError::None:
                RUNLOG->debug(SSTR("Global notification profile set to: " << paramProfile));
                break;
            case bb::platform::NotificationSettingsError::InsufficientPermissions:
                RUNLOG->error("Permission denied");
                break;
            default:
                RUNLOG->error(
                        SSTR("Failed to set profile mode, NotificationSettingsError: " << retval));
                break;
        }
    }

} /* namespace actiontrigger */
