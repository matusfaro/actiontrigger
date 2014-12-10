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

#include "actiontrigger/controller/services/media/actionrecordaudio.hpp"

namespace actiontrigger
{

    std::string ActionRecordAudio::TYPE = "ACTION_ActionRecordAudio";

    std::vector<ParameterDefinition*>* ActionRecordAudio::parameterDefinitions = NULL;
    Logger* ActionRecordAudio::StatementLOG = LoggerFactory::getInstance("ActionRecordAudio");
    const StatementInfo ActionRecordAudio::info(false, "Capture Audio", ActionRecordAudio::TYPE,
            "Records audio asynchronously to a file in MPEG 4 format. Recording can be paused and resumed. Recording is stopped either by calling the Stop action or when the procedure ends.");

    const StatementInfo ActionRecordAudio::getInfo()
    {
        return ActionRecordAudio::info;
    }

    ActionRecordAudio::ActionRecordAudio(StatementModel* model) :
            Action(model)
    {
    }

    ActionRecordAudio::~ActionRecordAudio()
    {
    }

    std::vector<ParameterDefinition*>* ActionRecordAudio::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION", "Action",
                    "Recording action");
            parameterDefinitions->push_back(filter);

            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("OUTFILE", "File",
                            "Output filename to create",
                            bb::cascades::pickers::FilePickerMode::Saver,
                            bb::cascades::pickers::FileType::Music, "*.m4a", ".m4a"));
            int INDEX_OUTFILE = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("APPENDTIME", "Append date",
                            "Appends current date and time to filename. This is useful to ensure files are not overwritten if this is called multiple times."));
            int INDEX_APPENDTIME = parameterDefinitions->size() - 1;

            filter->createFilter("Start", "START");
            filter->addToFilter(INDEX_OUTFILE);
            filter->addToFilter(INDEX_APPENDTIME);
            filter->createFilter("Pause", "PAUSE");
            filter->createFilter("Resume", "RESUME");
            filter->createFilter("Stop", "STOP");
        }
        return parameterDefinitions;
    }

    std::string ActionRecordAudio::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "START";
        } else if (key == "OUTFILE") {
            return "/accounts/1000/shared/voice/recording.m4a";
        } else if (key == "APPENDTIME") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRecordAudio::getType() const
    {
        return ActionRecordAudio::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRecordAudio::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("PICTURE_FILENAME",
                        "Filename of last audio recorded.", "STRING"));
        return vars;
    }

    void ActionRecordAudio::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string action = getParameter("ACTION", state);
        if (action == "START") {
            bb::multimedia::AudioRecorder* recorder =
                    state->getRuntimeResources()->getAudioRecorder();
            if (recorder->mediaState() != bb::multimedia::MediaState::Unprepared) {
                RUNLOG->warning("Stopping previous recording before starting");
                recorder->reset();
            }
            std::string filename;
            if (isValueTrue(getParameter("APPENDTIME", state))) {
                filename = state->getRuntimeResources()->uniqueFilename(
                        getParameter("OUTFILE", state));
            } else {
                filename = getParameter("OUTFILE", state);
            }
            state->setUserProperty("AUDIO_FILENAME", filename);
            recorder->setOutputUrl(QString::fromStdString(filename));
            recorder->record();
            RUNLOG->debug("Audio recording started");
        } else if (action == "PAUSE") {
            bb::multimedia::AudioRecorder* recorder =
                    state->getRuntimeResources()->getAudioRecorder();
            if (recorder->mediaState() != bb::multimedia::MediaState::Started) {
                RUNLOG->error("Recording is not running, cannot pause");
            } else {
                RUNLOG->debug("Audio recording paused");
                recorder->pause();
            }
        } else if (action == "RESUME") {
            bb::multimedia::AudioRecorder* recorder =
                    state->getRuntimeResources()->getAudioRecorder();
            if (recorder->mediaState() != bb::multimedia::MediaState::Paused) {
                RUNLOG->error("Recording is not running, cannot resume");
            } else {
                RUNLOG->debug("Audio recording resumed");
                recorder->record();
            }
        } else if (action == "STOP") {
            bb::multimedia::AudioRecorder* recorder =
                    state->getRuntimeResources()->getAudioRecorder();
            recorder->reset();
            RUNLOG->debug("Audio recording stopped");
        } else {
            RUNLOG->error("Unknown action: " + action);
        }
    }

} /* namespace actiontrigger */
