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

#include "actiontrigger/controller/services/camera/actionflash.hpp"

namespace actiontrigger
{

    std::string ActionFlash::TYPE = "ACTION_ActionFlash";

    std::vector<ParameterDefinition*>* ActionFlash::parameterDefinitions = NULL;
    Logger* ActionFlash::StatementLOG = LoggerFactory::getInstance("ActionFlash");
    const StatementInfo ActionFlash::info(false, "Flash", ActionFlash::TYPE,
            "Control the camera flash. Once procedure is stopped, flash will be turned off.");

    const StatementInfo ActionFlash::getInfo()
    {
        return ActionFlash::info;
    }

    ActionFlash::ActionFlash(StatementModel* model) :
            Action(model)
    {
    }

    ActionFlash::~ActionFlash()
    {
    }

    std::vector<ParameterDefinition*>* ActionFlash::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Attempt to turn Flash on or off", "Desired state");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionFlash::getDefaultParameter(std::string key)
    {
        if (key == "STATE") {
            return "on";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionFlash::getType() const
    {
        return ActionFlash::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionFlash::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionFlash::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string paramState = getParameter("STATE", state);
        bool desiredState;
        if (paramState.compare("on") == 0) {
            desiredState = true;
        } else if (paramState.compare("off") == 0) {
            desiredState = false;
        } else {
            RUNLOG->error("Parameter STATE is incorrect");
            return;
        }
        if (desiredState) {
            if (!state->getRuntimeResources()->isCameraOk(
                    state->getRuntimeResources()->setupCamera())) {
                RUNLOG->error("Failed to get camera");
                return;
            }
            camera_handle_t camera = state->getRuntimeResources()->getCamera();
            if (!state->getRuntimeResources()->isCameraOk(
                    camera_config_videolight(camera, CAMERA_VIDEOLIGHT_ON))) {
                RUNLOG->error("Failed to start the flash on the camera");
                return;
            }
        } else {
            state->getRuntimeResources()->destroyCamera();
        }
    }

} /* namespace actiontrigger */
