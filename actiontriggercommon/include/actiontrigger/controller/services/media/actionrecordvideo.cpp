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

#include "actiontrigger/controller/services/media/actionrecordvideo.hpp"

namespace actiontrigger
{

    std::string ActionRecordVideo::TYPE = "ACTION_ActionRecordVideo";

    std::vector<ParameterDefinition*>* ActionRecordVideo::parameterDefinitions = NULL;
    Logger* ActionRecordVideo::StatementLOG = LoggerFactory::getInstance("ActionRecordVideo");
    const StatementInfo ActionRecordVideo::info(false, "Record Video", ActionRecordVideo::TYPE,
            "Records video asynchronously to a file in MPEG 4 format. Recording is stopped either by calling the Stop action or when the procedure ends.");

    const StatementInfo ActionRecordVideo::getInfo()
    {
        return ActionRecordVideo::info;
    }

    ActionRecordVideo::ActionRecordVideo(StatementModel* model) :
            Action(model)
    {
    }

    ActionRecordVideo::~ActionRecordVideo()
    {
    }

    std::vector<ParameterDefinition*>* ActionRecordVideo::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            ParameterDefinition* cameraOpt = ParameterDefinition::getDropdown("CAMERA", "Camera",
                    "Front or back camera to use", "Camera");
            cameraOpt->dropdownAddOption("Front", "FRONT");
            cameraOpt->dropdownAddOption("Back", "BACK");

            parameterDefinitions = getParameterDefinitionsCamera(cameraOpt);
        }
        return parameterDefinitions;
    }

    std::vector<ParameterDefinition*>* ActionRecordVideo::getParameterDefinitionsCamera(
            ParameterDefinition* cameraOpt)
    {
        std::vector<ParameterDefinition*>* parameterDefinitionsNew = new std::vector<
                ParameterDefinition*>();

        ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION", "Action",
                "Recording action");
        parameterDefinitionsNew->push_back(filter);

        parameterDefinitionsNew->push_back(
                ParameterDefinition::getFileDialog("OUTFILE", "File", "Output filename to create",
                        bb::cascades::pickers::FilePickerMode::Saver,
                        bb::cascades::pickers::FileType::Video, "*.mp4", ".mp4"));
        int INDEX_OUTFILE = parameterDefinitionsNew->size() - 1;
        parameterDefinitionsNew->push_back(cameraOpt);
        int INDEX_CAMERA = parameterDefinitionsNew->size() - 1;
        parameterDefinitionsNew->push_back(
                ParameterDefinition::getCheckbox("APPENDTIME", "Append date",
                        "Appends current date and time to filename. This is useful to ensure files are not overwritten if this is called multiple times."));
        int INDEX_APPENDTIME = parameterDefinitionsNew->size() - 1;

        filter->createFilter("Start", "START");
        filter->addToFilter(INDEX_OUTFILE);
        filter->addToFilter(INDEX_CAMERA);
        filter->addToFilter(INDEX_APPENDTIME);
        filter->createFilter("Stop", "STOP");

        return parameterDefinitionsNew;
    }

    std::string ActionRecordVideo::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "START";
        } else if (key == "CAMERA") {
            return "BACK";
        } else if (key == "OUTFILE") {
            return "/accounts/1000/shared/videos/recording.mp4";
        } else if (key == "APPENDTIME") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRecordVideo::getType() const
    {
        return ActionRecordVideo::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRecordVideo::getExposedVariableDefinitions()
    {
        return getExposedVariableDefinitionsCamera();
    }

    std::vector<statement_output_variable_definition*>* ActionRecordVideo::getExposedVariableDefinitionsCamera()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("VIDEO_FILENAME",
                        "Filename of last video recorded.", "STRING"));
        return vars;
    }

    void ActionRecordVideo::execute(ExecutionState* state)
    {
        executeCameraAction(state, getParameter("ACTION", state), getParameter("CAMERA", state),
                getParameter("APPENDTIME", state), getParameter("OUTFILE", state));
    }

    void ActionRecordVideo::executeCameraAction(ExecutionState* state, std::string action,
            std::string cameraUnitString, std::string appendTime, std::string outFile)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        camera_unit_t cameraUnit;
        if (cameraUnitString == "BACK") {
            cameraUnit = CAMERA_UNIT_REAR;
        } else if (cameraUnitString == "FRONT") {
            cameraUnit = CAMERA_UNIT_FRONT;
        } else if (cameraUnitString == "DESKTOP_PRIMARY") {
            cameraUnit = CAMERA_UNIT_DESKTOP;
        } else if (cameraUnitString == "DESKTOP_SECONDARY") {
            cameraUnit = CAMERA_UNIT_SECONDARY_DESKTOP;
        } else {
            LOG->warning(SSTR("Unknown source selected: " << cameraUnitString));
            RUNLOG->error("Unknown source selected");
            return;
        }

        if (action == "START") {
            if (!state->getRuntimeResources()->isCameraOk(
                    state->getRuntimeResources()->setupCamera(cameraUnit, CAMERA_MODE_RW))) {
                LOG->error("Failed to get camera");
                RUNLOG->error("Failed to get camera");
                return;
            }
            camera_handle_t camera = state->getRuntimeResources()->getCamera();
            if (!state->getRuntimeResources()->isCameraOk(
                    _camera_set_videovf_property(camera, CAMERA_IMGPROP_END))) {
                LOG->error("Failed to set video frame properties");
                RUNLOG->error("Failed to set video frame properties");
                return;
            }
            if (!state->getRuntimeResources()->isCameraOk(
                    _camera_set_video_property(camera, CAMERA_IMGPROP_END))) {
                LOG->error("Failed to set video properties");
                RUNLOG->error("Failed to set video properties");
                return;
            }
            state->getRuntimeResources()->callbackReset();
            if (!state->getRuntimeResources()->isCameraOk(
                    camera_start_video_viewfinder(camera, NULL, viewFinderStatusCallback,
                            (void*) state->getRuntimeResources()))) {
                LOG->error("Failed to start video viewfinder");
                RUNLOG->error("Failed to start video viewfinder");
                return;
            }
            if (!state->getRuntimeResources()->callbackWait(5000)) {
                LOG->warning("Timed out waiting for view finder to start");
            }
            std::string filename;
            if (Statement::isValueTrue(appendTime)) {
                filename = state->getRuntimeResources()->uniqueFilename(outFile);
            } else {
                filename = outFile;
            }
            state->setUserProperty("VIDEO_FILENAME", filename);
            state->getRuntimeResources()->callbackReset();
            if (!state->getRuntimeResources()->isCameraOk(
                    camera_start_video(camera, filename.c_str(), NULL, videoRecordingStatusCallback,
                            (void*) state->getRuntimeResources()))) {
                LOG->error("Failed to start video recording");
                RUNLOG->error("Failed to start video recording");
                return;
            }
            if (!state->getRuntimeResources()->callbackWait(5000)) {
                LOG->warning("Timed out waiting for recording to start");
            }
            RUNLOG->debug("Started video recording");
        } else if (action == "STOP") {
            camera_handle_t camera = state->getRuntimeResources()->getCamera();
            if (!state->getRuntimeResources()->isCameraOk(camera_stop_video(camera))) {
                LOG->error("Failed to stop video recording");
                RUNLOG->error("Failed to stop video recording");
                return;
            }
            if (!state->getRuntimeResources()->isCameraOk(camera_stop_video_viewfinder(camera))) {
                LOG->error("Failed to stop video viewfinder");
                RUNLOG->error("Failed to stop video viewfinder");
                return;
            }
            state->getRuntimeResources()->destroyCamera();
            RUNLOG->debug("Stopped video recording");
        } else {
            RUNLOG->error("Unknown action: " + action);
        }
    }

    void ActionRecordVideo::viewFinderStatusCallback(camera_handle_t handle,
            camera_devstatus_t status, uint16_t value, void *context)
    {
        (void) handle;
        (void) status;
        (void) value;
        ((ExecutionResources*) context)->callbackFinished();
    }

    void ActionRecordVideo::videoRecordingStatusCallback(camera_handle_t handle,
            camera_devstatus_t status, uint16_t value, void *context)
    {
        (void) handle;
        (void) status;
        (void) value;
        ((ExecutionResources*) context)->callbackFinished();
    }

} /* namespace actiontrigger */
