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

#include "actiontrigger/controller/services/media/actiongetvolume.hpp"

namespace actiontrigger
{

    std::string ActionGetVolume::TYPE = "ACTION_ActionGetVolume";

    std::vector<ParameterDefinition*>* ActionGetVolume::parameterDefinitions = NULL;
    Logger* ActionGetVolume::StatementLOG = LoggerFactory::getInstance("ActionGetVolume");
    const StatementInfo ActionGetVolume::info(false, "Get Audio Volume", ActionGetVolume::TYPE,
            "Retrieve the volume for audio input and output devices.");

    const StatementInfo ActionGetVolume::getInfo()
    {
        return ActionGetVolume::info;
    }

    ActionGetVolume::ActionGetVolume(StatementModel* model) :
            Action(model)
    {
    }

    ActionGetVolume::~ActionGetVolume()
    {
    }

    std::vector<ParameterDefinition*>* ActionGetVolume::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("DEVICE", "Audio device",
                    "Audio device to retrieve volume information for.'", "Device");
            state->dropdownAddOption("Default active device", "AUDIO_DEVICE_DEFAULT");
            state->dropdownAddOption("Main speaker", "AUDIO_DEVICE_SPEAKER");
            state->dropdownAddOption("Headset with microphone", "AUDIO_DEVICE_HEADSET");
            state->dropdownAddOption("Lineout headset jack", "AUDIO_DEVICE_LINEOUT");
            state->dropdownAddOption("Output only headphones", "AUDIO_DEVICE_HEADPHONE");
            state->dropdownAddOption("HDMI", "AUDIO_DEVICE_HDMI");
            state->dropdownAddOption("Bluetooth A2DP", "AUDIO_DEVICE_A2DP");
            state->dropdownAddOption("Phone receiver handset", "AUDIO_DEVICE_HANDSET");
            state->dropdownAddOption("Bluetooth hands-free", "AUDIO_DEVICE_BT_SCO");
            state->dropdownAddOption("TOSLINK", "AUDIO_DEVICE_TOSLINK");
            state->dropdownAddOption("Hearing aid device", "AUDIO_DEVICE_HAC");
            state->dropdownAddOption("TTY", "AUDIO_DEVICE_TTY");
            state->dropdownAddOption("USB Audio", "AUDIO_DEVICE_USB");
            state->dropdownAddOption("System Tones", "AUDIO_DEVICE_TONES");
            state->dropdownAddOption("VoIP", "AUDIO_DEVICE_VOICE");
            state->dropdownAddOption("Wifi Audio", "AUDIO_DEVICE_WIFI_DISPLAY");
            state->dropdownAddOption("MirrorLink", "AUDIO_DEVICE_MIRRORLINK");
            state->dropdownAddOption("Video share", "AUDIO_DEVICE_AUDIO_SHARE");
//        state->dropdownAddOption("The total number of devices supported.", "AUDIO_DEVICE_COUNT");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionGetVolume::getDefaultParameter(std::string key)
    {
        if (key == "DEVICE") {
            return "AUDIO_DEVICE_DEFAULT";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionGetVolume::getType() const
    {
        return ActionGetVolume::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGetVolume::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("VOLUME_OUT",
                        "Output volume level of audio device from 0 to 100", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("VOLUME_IN",
                        "Input volume level of audio device from 0 to 100", "STRING"));
        return vars;
    }

    void ActionGetVolume::execute(ExecutionState* state)
    {
        (void) state;
        audio_manager_device_t device = VolumeManager::stringToAudioDevice(
                getParameter("DEVICE", state));
        double level;
        audio_manager_get_output_level(device, &level);
        state->setUserProperty("VOLUME_OUT", SSTR(level));
        audio_manager_get_input_level(device, &level);
        state->setUserProperty("VOLUME_IN", SSTR(level));
    }

} /* namespace actiontrigger */
