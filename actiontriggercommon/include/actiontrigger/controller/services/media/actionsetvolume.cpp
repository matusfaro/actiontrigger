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

#include "actiontrigger/controller/services/media/actionsetvolume.hpp"

namespace actiontrigger
{

    std::string ActionSetVolume::TYPE = "ACTION_ActionSetVolume";

    std::vector<ParameterDefinition*>* ActionSetVolume::parameterDefinitions = NULL;
    Logger* ActionSetVolume::StatementLOG = LoggerFactory::getInstance("ActionSetVolume");
    const StatementInfo ActionSetVolume::info(false, "Set Audio Volume", ActionSetVolume::TYPE,
            "Set the volume for audio input and output devices.");

    const StatementInfo ActionSetVolume::getInfo()
    {
        return ActionSetVolume::info;
    }

    ActionSetVolume::ActionSetVolume(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetVolume::~ActionSetVolume()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetVolume::getParameterDefinitions()
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

            ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION",
                    "Adjustment", "Volume adjustment to make.");
            parameterDefinitions->push_back(filter);

            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("SLIDER_VOLUME", "Volume Level",
                            "Volume level to set for the device", "0", "100", "1"));
            int INDEX_VOLUME_SLIDER_VOLUME = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("SLIDER_ADJUST", "Volume Level",
                            "Volume level adjustment. If set to 10, the volume will be raised by 10%.",
                            "-100", "100", "1"));
            int INDEX_VOLUME_SLIDER_ADJUST = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("MUTE_BOOL", "Mute/Unmute",
                            "Check to mute, uncheck for unmute"));
            int INDEX_VOLUME_MUTE_UNMUTE_CHECKBOX = parameterDefinitions->size() - 1;

            filter->createFilter("Set output volume", "SET_VOLUME_OUT");
            filter->addToFilter(INDEX_VOLUME_SLIDER_VOLUME);
            filter->createFilter("Adjust output volume", "ADJUST_VOLUME_OUT");
            filter->addToFilter(INDEX_VOLUME_SLIDER_ADJUST);
            filter->createFilter("Mute/Unmute output volume", "MUTE_UNMUTE_VOLUME_OUT");
            filter->addToFilter(INDEX_VOLUME_MUTE_UNMUTE_CHECKBOX);
            filter->createFilter("Toggle mute output volume", "TOGGLE_MUTE_VOLUME_OUT");

            filter->createFilter("Set input volume", "SET_VOLUME_IN");
            filter->addToFilter(INDEX_VOLUME_SLIDER_VOLUME);
            filter->createFilter("Adjust input volume", "ADJUST_VOLUME_IN");
            filter->addToFilter(INDEX_VOLUME_SLIDER_ADJUST);
            filter->createFilter("Mute/Unmute input volume", "MUTE_UNMUTE_VOLUME_IN");
            filter->addToFilter(INDEX_VOLUME_MUTE_UNMUTE_CHECKBOX);
            filter->createFilter("Toggle mute input volume", "TOGGLE_MUTE_VOLUME_IN");
        }
        return parameterDefinitions;
    }

    std::string ActionSetVolume::getDefaultParameter(std::string key)
    {
        if (key == "DEVICE") {
            return "AUDIO_DEVICE_DEFAULT";
        } else if (key == "ACTION") {
            return "SET_VOLUME_OUT";
        } else if (key == "SLIDER_VOLUME") {
            return "100";
        } else if (key == "SLIDER_ADJUST") {
            return "0";
        } else if (key == "MUTE_BOOL") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionSetVolume::getType() const
    {
        return ActionSetVolume::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSetVolume::getExposedVariableDefinitions()
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

    void ActionSetVolume::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        double level;
        try {
            level = Common::StringToNumber<double>(getParameter("SLIDER_VOLUME", state));
        } catch (std::exception& e) {
            LOG->trace(
                    SSTR(
                            "Failed to convert level string to number '"
                                    << getParameter("SLIDER_VOLUME", state) << "'"));
            level = -1000;
        }
        level = (level <= 100 && level >= 0) ? level : -1000;

        double adjust;
        try {
            adjust = Common::StringToNumber<double>(getParameter("SLIDER_ADJUST", state));
        } catch (std::exception& e) {
            LOG->trace(
                    SSTR(
                            "Failed to convert adjust string to number '"
                                    << getParameter("SLIDER_ADJUST", state) << "'"));
            adjust = -1000;
        }
        adjust = (adjust <= 100 && adjust >= -100) ? adjust : -1000;

        audio_manager_device_t device = VolumeManager::stringToAudioDevice(
                getParameter("DEVICE", state));

        bool muteUnmute = isValueTrue(getParameter("MUTE_BOOL", state));

        std::string action = getParameter("ACTION", state);

        if (action == "SET_VOLUME_OUT") {
            if (level == -1000) {
                RUNLOG->warning(
                        SSTR("Volume out of range: " << getParameter("SLIDER_VOLUME", state)));
            } else {
                audio_manager_set_output_level(device, level);
            }
        } else if (action == "SET_VOLUME_IN") {
            if (level == -1000) {
                RUNLOG->warning(
                        SSTR("Volume out of range: " << getParameter("SLIDER_VOLUME", state)));
            } else {
                audio_manager_set_input_level(device, level);
            }
        } else if (action == "ADJUST_VOLUME_OUT") {
            if (adjust == -1000) {
                RUNLOG->warning(
                        SSTR("Adjustment out of range: " << getParameter("SLIDER_ADJUST", state)));
            } else {
                audio_manager_adjust_output_level(device, adjust);
            }
        } else if (action == "ADJUST_VOLUME_IN") {
            if (adjust == -1000) {
                RUNLOG->warning(
                        SSTR("Adjustment out of range: " << getParameter("SLIDER_ADJUST", state)));
            } else {
                audio_manager_adjust_input_level(device, adjust);
            }
        } else if (action == "MUTE_UNMUTE_VOLUME_OUT") {
            audio_manager_set_output_mute(device, muteUnmute);
        } else if (action == "TOGGLE_MUTE_VOLUME_OUT") {
            audio_manager_toggle_output_mute(device);
        } else if (action == "MUTE_UNMUTE_VOLUME_IN") {
            audio_manager_set_input_mute(device, muteUnmute);
        } else if (action == "TOGGLE_MUTE_VOLUME_IN") {
            audio_manager_toggle_input_mute(device);
        } else {
            RUNLOG->error(SSTR("Unknown action: " << action));
        }
    }

} /* namespace actiontrigger */
