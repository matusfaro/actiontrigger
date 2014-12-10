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

#include "actiontrigger/controller/services/device/actionsetclipboard.hpp"

namespace actiontrigger
{

    std::string ActionSetClipboard::TYPE = "ACTION_ActionSetClipboard";

    std::vector<ParameterDefinition*>* ActionSetClipboard::parameterDefinitions = NULL;
    Logger* ActionSetClipboard::StatementLOG = LoggerFactory::getInstance("ActionSetClipboard");
    const StatementInfo ActionSetClipboard::info(false, "Set Clipboard", ActionSetClipboard::TYPE,
            "Set content of device clipboard or empty clipboard. Common format types: 'text/html', 'text/plain', 'image/png', 'image/jpeg' and 'application/x-color'");

    const StatementInfo ActionSetClipboard::getInfo()
    {
        return ActionSetClipboard::info;
    }

    ActionSetClipboard::ActionSetClipboard(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetClipboard::~ActionSetClipboard()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetClipboard::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("TYPE", "Type",
                            "Type of data to be written to clipboard. If Content AND Type is empty, clipboard for all types will be emptied."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CONTENT", "Content",
                            "Content to be written to clipboard. If empty, clipboard will be emptied."));
        }
        return parameterDefinitions;
    }

    std::string ActionSetClipboard::getType() const
    {
        return ActionSetClipboard::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSetClipboard::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    std::string ActionSetClipboard::getDefaultParameter(std::string key)
    {
        if (key == "TYPE") {
            return "text/plain";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    void ActionSetClipboard::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        std::string type = getParameter("TYPE", state);
        std::string content = getParameter("CONTENT", state);
        bb::system::Clipboard clipboard(0);

        if (content.empty()) {
            if (type.empty()) {
                // Empty clipboard
                clipboard.clear();
                RUNLOG->debug("Clipboard has been completely cleared");
            } else {
                // Empty clipboard only for specific type
                clipboard.remove(QString::fromStdString(type));
                RUNLOG->debug(SSTR("Clipboard has been emptied for type: " << type));
            }
        } else {
            // Set clipboard
            clipboard.insert(QString::fromStdString(type),
                    QString::fromStdString(content).toUtf8());
            RUNLOG->debug(SSTR("Clipboard has been set"));
        }
    }

} /* namespace actiontrigger */
