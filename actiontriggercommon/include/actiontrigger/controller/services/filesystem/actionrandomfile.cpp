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

#include "actiontrigger/controller/services/filesystem/actionrandomfile.hpp"

namespace actiontrigger
{

    std::string ActionRandomFile::TYPE = "ACTION_ActionRandomFile";

    std::vector<ParameterDefinition*>* ActionRandomFile::parameterDefinitions = NULL;
    Logger* ActionRandomFile::StatementLOG = LoggerFactory::getInstance("ActionRandomFile");
    const StatementInfo ActionRandomFile::info(false, "Random file", ActionRandomFile::TYPE,
            "Get the filename of a random file within a directory");

    const StatementInfo ActionRandomFile::getInfo()
    {
        return ActionRandomFile::info;
    }

    ActionRandomFile::ActionRandomFile(StatementModel* model) :
            Action(model)
    {
    }

    ActionRandomFile::~ActionRandomFile()
    {
    }

    std::vector<ParameterDefinition*>* ActionRandomFile::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("FILTER", "Filter",
                            "Only select files that match this filter. Wildcard * is acceptable. Example: *.png"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getDirectoryDialog("DIR", "Directory",
                            "Path to the directory"));
        }
        return parameterDefinitions;
    }

    std::string ActionRandomFile::getDefaultParameter(std::string key)
    {
        if (key == "FILTER") {
            return "*.png";
        } else if (key == "DIR") {
            return "/accounts/1000/shared/protected_media/wallpaper";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRandomFile::getType() const
    {
        return ActionRandomFile::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRandomFile::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("RAND_FILE",
                        "Randomly picked file from given directory", "STRING"));
        return vars;
    }

    void ActionRandomFile::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        QStringList filenames = QDir(QString::fromStdString(getParameter("DIR", state)),
                QString::fromStdString(getParameter("FILTER", state))).entryList(
                QDir::NoDotAndDotDot | QDir::Files, QDir::NoSort);
        if (filenames.length() == 0) {
            RUNLOG->error(
                    "No files found in directory \"" + getParameter("DIR", state)
                            + "\" with filter \"" + getParameter("FILTER", state) + "\"");
            state->setUserProperty("RAND_FILE", "");
            return;
        }
        std::string filename = filenames.at(rand() % filenames.length()).toStdString();
        state->setUserProperty("RAND_FILE", getParameter("DIR", state) + "/" + filename);
    }

} /* namespace actiontrigger */
