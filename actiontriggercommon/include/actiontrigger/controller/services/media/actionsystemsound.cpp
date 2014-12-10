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

#include "actiontrigger/controller/services/media/actionsystemsound.hpp"

namespace actiontrigger
{

    std::string ActionSystemSound::TYPE = "ACTION_ActionSystemSound";

    std::vector<ParameterDefinition*>* ActionSystemSound::parameterDefinitions = NULL;
    Logger* ActionSystemSound::StatementLOG = LoggerFactory::getInstance("ActionSystemSound");
    const StatementInfo ActionSystemSound::info(false, "Play System Sound", ActionSystemSound::TYPE,
            "Play a default system sound. If you don't hear a sound, make sure you don't have a vibrate-only profile set.");

    const StatementInfo ActionSystemSound::getInfo()
    {
        return ActionSystemSound::info;
    }

    ActionSystemSound::ActionSystemSound(StatementModel* model) :
            Action(model)
    {
    }

    ActionSystemSound::~ActionSystemSound()
    {
    }

    std::vector<ParameterDefinition*>* ActionSystemSound::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            ParameterDefinition* state = ParameterDefinition::getDropdown("SYSTEMSOUND", "Sound",
                    "Sound to play", "Play");
            state->dropdownAddOption("Battery Alarm", "1");
            state->dropdownAddOption("Browser Start Event", "2");
            state->dropdownAddOption("Camera Shutter Event", "3");
            state->dropdownAddOption("Device Lock Event", "4");
            state->dropdownAddOption("Device Unlock Event", "5");
            state->dropdownAddOption("Device Tether Event", "6");
            state->dropdownAddOption("Device Untether Event", "7");
            state->dropdownAddOption("General Notification", "8");
            state->dropdownAddOption("Input Keypress", "9");
            state->dropdownAddOption("Recording Start Event", "10");
            state->dropdownAddOption("Recording Stop Event", "11");
            state->dropdownAddOption("Sapphire Notification", "12");
            state->dropdownAddOption("System Master Volume Reference", "13");
            state->dropdownAddOption("Video Call Event", "14");
            state->dropdownAddOption("Video Call Outgoing Event", "15");
            state->dropdownAddOption("Camera Burst Event", "16");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionSystemSound::getDefaultParameter(std::string key)
    {
        if (key == "SYSTEMSOUND") {
            return "8"; // GeneralNotification
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionSystemSound::getType() const
    {
        return ActionSystemSound::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSystemSound::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionSystemSound::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        bb::multimedia::SystemSound::Type melody = bb::multimedia::SystemSound::GeneralNotification;
        try {
            melody = static_cast<bb::multimedia::SystemSound::Type>(Common::StringToNumber<int>(
                    getParameter("SYSTEMSOUND", state)));
        } catch (...) {
            RUNLOG->error("Could not find sound to play");
        }
        bb::multimedia::SystemSound::play(melody);
    }

} /* namespace actiontrigger */
