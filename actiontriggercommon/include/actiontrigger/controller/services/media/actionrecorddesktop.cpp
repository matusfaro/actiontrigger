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

#include "actiontrigger/controller/services/media/actionrecorddesktop.hpp"

namespace actiontrigger
{

    std::string ActionRecordDesktop::TYPE = "ACTION_ActionRecordDesktop";

    std::vector<ParameterDefinition*>* ActionRecordDesktop::parameterDefinitions = NULL;
    Logger* ActionRecordDesktop::StatementLOG = LoggerFactory::getInstance("ActionRecordDesktop");
    const StatementInfo ActionRecordDesktop::info(false, "Record Desktop",
            ActionRecordDesktop::TYPE,
            "Records desktop screen asynchronously to a file in MPEG 4 format. Recording is stopped either by calling the Stop action or when the procedure ends.");

    const StatementInfo ActionRecordDesktop::getInfo()
    {
        return ActionRecordDesktop::info;
    }

    ActionRecordDesktop::ActionRecordDesktop(StatementModel* model) :
            Action(model)
    {
    }

    ActionRecordDesktop::~ActionRecordDesktop()
    {
    }

    std::vector<ParameterDefinition*>* ActionRecordDesktop::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            ParameterDefinition* cameraOpt =
                    ParameterDefinition::getDropdown("CAMERA", "Source",
                            "Desktop to capture. Either capture the device desktop or an attached desktop if available.",
                            "Camera");
            cameraOpt->dropdownAddOption("Device desktop", "DESKTOP_PRIMARY");
            cameraOpt->dropdownAddOption("Attached desktop", "DESKTOP_SECONDARY");

            parameterDefinitions = ActionRecordVideo::getParameterDefinitionsCamera(cameraOpt);
        }
        return parameterDefinitions;
    }

    std::string ActionRecordDesktop::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "START";
        } else if (key == "CAMERA") {
            return "DESKTOP_PRIMARY";
        } else if (key == "OUTFILE") {
            return "/accounts/1000/shared/videos/recording.mp4";
        } else if (key == "APPENDTIME") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRecordDesktop::getType() const
    {
        return ActionRecordDesktop::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRecordDesktop::getExposedVariableDefinitions()
    {
        return ActionRecordVideo::getExposedVariableDefinitionsCamera();
    }

    void ActionRecordDesktop::execute(ExecutionState* state)
    {
        // Logic is located in ActionRecordVideo
        ActionRecordVideo::executeCameraAction(state, getParameter("ACTION", state),
                getParameter("CAMERA", state), getParameter("APPENDTIME", state),
                getParameter("OUTFILE", state));
    }

} /* namespace actiontrigger */
