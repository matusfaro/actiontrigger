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

#include "actiontrigger/controller/services/device/actiongetclipboard.hpp"

namespace actiontrigger
{

    std::string ActionGetClipboard::TYPE = "ACTION_ActionGetClipboard";

    std::vector<ParameterDefinition*>* ActionGetClipboard::parameterDefinitions = NULL;
    Logger* ActionGetClipboard::StatementLOG = LoggerFactory::getInstance("ActionGetClipboard");
    const StatementInfo ActionGetClipboard::info(false, "Get Clipboard", ActionGetClipboard::TYPE,
            "Retrieve content of device clipboard. Common format types: 'text/plain', 'text/html', 'image/png', 'image/jpeg' and 'application/x-color'");

    const StatementInfo ActionGetClipboard::getInfo()
    {
        return ActionGetClipboard::info;
    }

    ActionGetClipboard::ActionGetClipboard(StatementModel* model) :
            Action(model)
    {
    }

    ActionGetClipboard::~ActionGetClipboard()
    {
    }

    std::vector<ParameterDefinition*>* ActionGetClipboard::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("TYPE", "Type",
                            "Format type of data to retrieve from clipboard"));
        }
        return parameterDefinitions;
    }

    std::string ActionGetClipboard::getType() const
    {
        return ActionGetClipboard::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGetClipboard::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("CLIPBOARD_CONTENT",
                        "Content of clipboard", "STRING"));
        return vars;
    }

    std::string ActionGetClipboard::getDefaultParameter(std::string key)
    {
        if (key == "TYPE") {
            return "text/plain";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    void ActionGetClipboard::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        std::string type = getParameter("TYPE", state);
        bb::system::Clipboard clipboard(0);
        std::string content = QString::fromUtf8(
                clipboard.value(QString::fromStdString(type), NULL).constData()).toStdString();

        state->setUserProperty("CLIPBOARD_CONTENT", content);
        RUNLOG->debug("Clipboard has been retrieved");
    }

} /* namespace actiontrigger */
