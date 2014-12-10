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

#include "actiontrigger/controller/services/media/actioncustomsound.hpp"

namespace actiontrigger
{

    std::string ActionCustomSound::TYPE = "ACTION_ActionCustomSound";

    std::vector<ParameterDefinition*>* ActionCustomSound::parameterDefinitions = NULL;
    Logger* ActionCustomSound::StatementLOG = LoggerFactory::getInstance("ActionCustomSound");
    const StatementInfo ActionCustomSound::info(false, "Play Custom Sound", ActionCustomSound::TYPE,
            "Plays a custom sound asynchronously from a file. Current playback will be stopped when the Stop action is called or when the procedure ends.");

    const StatementInfo ActionCustomSound::getInfo()
    {
        return ActionCustomSound::info;
    }

    ActionCustomSound::ActionCustomSound(StatementModel* model) :
            Action(model)
    {
    }

    ActionCustomSound::~ActionCustomSound()
    {
    }

    std::vector<ParameterDefinition*>* ActionCustomSound::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION", "Action",
                    "Action to perform");
            parameterDefinitions->push_back(filter);

            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("AUDIOFILE", "Audio file",
                            "Audio file to play.", bb::cascades::pickers::FilePickerMode::Picker,
                            bb::cascades::pickers::FileType::Music));
            int INDEX_AUDIOFILE = parameterDefinitions->size() - 1;

            filter->createFilter("Play", "PLAY");
            filter->addToFilter(INDEX_AUDIOFILE);
            filter->createFilter("Stop", "STOP");
        }
        return parameterDefinitions;
    }

    std::string ActionCustomSound::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "PLAY"; // GeneralNotification
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionCustomSound::getType() const
    {
        return ActionCustomSound::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionCustomSound::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionCustomSound::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string action = getParameter("ACTION", state);
        if (action == "PLAY") {
            std::string file = getParameter("AUDIOFILE", state);
            bb::multimedia::MediaPlayer* mp = state->getRuntimeResources()->getMediaPlayer();
            mp->stop();
            mp->setSourceUrl(QString::fromStdString(file));
            mp->play();
        } else if (action == "STOP") {
            bb::multimedia::MediaPlayer* mp = state->getRuntimeResources()->getMediaPlayer();
            mp->stop();
        } else {
            RUNLOG->error("Unknown action: " + action);
        }
    }

} /* namespace actiontrigger */
