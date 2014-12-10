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

#include "actiontrigger/controller/services/media/actionrecordpicture.hpp"

namespace actiontrigger
{

    std::string ActionRecordPicture::TYPE = "ACTION_ActionRecordPicture";

    std::vector<ParameterDefinition*>* ActionRecordPicture::parameterDefinitions = NULL;
    Logger* ActionRecordPicture::StatementLOG = LoggerFactory::getInstance("ActionRecordPicture");
    const StatementInfo ActionRecordPicture::info(false, "Take photo", ActionRecordPicture::TYPE,
            "Takes a camera picture and saves to a file in JPEG format.");

    const StatementInfo ActionRecordPicture::getInfo()
    {
        return ActionRecordPicture::info;
    }

    ActionRecordPicture::ActionRecordPicture(StatementModel* model) :
            Action(model)
    {
    }

    ActionRecordPicture::~ActionRecordPicture()
    {
    }

    std::vector<ParameterDefinition*>* ActionRecordPicture::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("OUTFILE", "File",
                            "Output filename to create",
                            bb::cascades::pickers::FilePickerMode::Saver,
                            bb::cascades::pickers::FileType::Video, "*.jpeg", ".jpeg"));
            ParameterDefinition* state = ParameterDefinition::getDropdown("CAMERA", "Camera",
                    "Front or back camera to use", "Camera");
            state->dropdownAddOption("Front", "FRONT");
            state->dropdownAddOption("Back", "BACK");
            parameterDefinitions->push_back(state);
            state = ParameterDefinition::getDropdown("FLASH", "Flash", "Setting when to use flash",
                    "Flash");
            state->dropdownAddOption("Auto", "AUTO");
            state->dropdownAddOption("On", "ON");
            state->dropdownAddOption("Off", "OFF");
            parameterDefinitions->push_back(state);
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("APPENDTIME", "Append date",
                            "Appends current date and time to filename. This is useful to ensure files are not overwritten if this is called multiple times."));
        }
        return parameterDefinitions;
    }

    std::string ActionRecordPicture::getDefaultParameter(std::string key)
    {
        if (key == "CAMERA") {
            return "BACK";
        } else if (key == "OUTFILE") {
            return "/accounts/1000/shared/photos/photo.jpeg";
        } else if (key == "APPENDTIME") {
            return "FALSE";
        } else if (key == "FLASH") {
            return "AUTO";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRecordPicture::getType() const
    {
        return ActionRecordPicture::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRecordPicture::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("PICTURE_FILENAME",
                        "Filename of last picture taken.", "STRING"));
        return vars;
    }

    void ActionRecordPicture::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        camera_unit_t cameraUnit =
                getParameter("CAMERA", state) == "BACK" ? CAMERA_UNIT_REAR : CAMERA_UNIT_FRONT;
        if (!state->getRuntimeResources()->isCameraOk(
                state->getRuntimeResources()->setupCamera(cameraUnit, CAMERA_MODE_RW))) {
            LOG->error("Failed to get camera");
            RUNLOG->error("Failed to get camera");
            return;
        }
        camera_handle_t camera = state->getRuntimeResources()->getCamera();
        if (!state->getRuntimeResources()->isCameraOk(
                _camera_set_photovf_property(camera, CAMERA_IMGPROP_END))) {
            LOG->error("Failed to set photo view finder properties");
            RUNLOG->error("Failed to set photo view finder properties");
            return;
        }
        if (!state->getRuntimeResources()->isCameraOk(
                _camera_set_photo_property(camera, CAMERA_IMGPROP_END))) {
            LOG->error("Failed to set photo properties");
            RUNLOG->error("Failed to set photo properties");
            return;
        }
        state->getRuntimeResources()->callbackReset();
        if (!state->getRuntimeResources()->isCameraOk(
                camera_start_photo_viewfinder(camera, NULL, viewFinderStatusCallback,
                        (void*) state->getRuntimeResources()))) {
            LOG->error("Failed to start video viewfinder");
            RUNLOG->error("Failed to start video viewfinder");
            return;
        }
        if (!state->getRuntimeResources()->callbackWait(5000)) {
            LOG->warning("Timed out waiting for view finder to start");
        }
        std::string flashModeStr = getParameter("FLASH", state);
        camera_flashmode_t flashMode;
        if (flashModeStr == "ON") {
            flashMode = CAMERA_FLASH_ON;
        } else if (flashModeStr == "OFF") {
            flashMode = CAMERA_FLASH_OFF;
        } else {
            flashMode = CAMERA_FLASH_AUTO;
        }
        if (!state->getRuntimeResources()->isCameraOk(camera_config_flash(camera, flashMode))) {
            LOG->error("Failed to set flash setting");
            RUNLOG->error("Failed to set flash setting");
            return;
        }
        std::string filename;
        if (isValueTrue(getParameter("APPENDTIME", state))) {
            filename = state->getRuntimeResources()->uniqueFilename(getParameter("OUTFILE", state));
        } else {
            filename = getParameter("OUTFILE", state);
        }
        state->setUserProperty("PICTURE_FILENAME", filename);
        state->setExecutionProperty("ACTION_ActionRecordPicture_OUTFILE", filename);
        state->getRuntimeResources()->callbackReset();
        if (!state->getRuntimeResources()->isCameraOk(
                camera_take_photo(camera, NULL, NULL, NULL, photoRecordingStatusCallback,
                        (void*) state, false))) {
            LOG->error("Failed to take photo");
            RUNLOG->error("Failed to take photo");
            return;
        }
        if (!state->getRuntimeResources()->callbackWait(3000)) {
            LOG->warning("Timed out waiting for view finder to start");
            return;
        }
        if (!state->getRuntimeResources()->isCameraOk(camera_stop_photo_viewfinder(camera))) {
            LOG->error("Failed to stop photo viewfinder");
            RUNLOG->error("Failed to stop photo viewfinder");
            return;
        }
        state->getRuntimeResources()->destroyCamera();
        RUNLOG->debug("Photo captured");
    }

    void ActionRecordPicture::viewFinderStatusCallback(camera_handle_t handle,
            camera_devstatus_t status, uint16_t value, void *context)
    {
        (void) handle;
        (void) status;
        (void) value;
        ((ExecutionResources*) context)->callbackFinished();
    }

    void ActionRecordPicture::photoRecordingStatusCallback(camera_handle_t handle,
            camera_buffer_t* buf, void *context)
    {
        (void) handle;
        ExecutionState* state = (ExecutionState*) context;
        std::string outfile = state->getExecutionProperty("ACTION_ActionRecordPicture_OUTFILE");
        DataModelLogger* RUNLOG = state->getLogger();

        if (buf->frametype != CAMERA_FRAMETYPE_JPEG) {
            RUNLOG->error("Failed to save image: Wrong format");
            return;
        }
        QFile file(QString::fromStdString(outfile));
        if (!file.open(QFile::WriteOnly)) {
            RUNLOG->error("Failed to save image: Cannot open file");
            return;
        }
        int fd = file.handle();
        int index = 0;
        while (index < (int) buf->framedesc.jpeg.bufsize) {
            int rc = write(fd, &buf->framebuf[index], buf->framedesc.jpeg.bufsize - index);
            if (rc > 0) {
                index += rc;
            } else if (rc == -1) {
                if ((errno == EAGAIN) || (errno == EINTR))
                    continue;
                RUNLOG->error(SSTR("Failed to save image: " << strerror(errno)));
                close(fd);
                return;
            }
        }
        close(fd);
        RUNLOG->trace(SSTR("Saved image with size " << buf->framedesc.jpeg.bufsize));
        state->getRuntimeResources()->callbackFinished();
    }

} /* namespace actiontrigger */
