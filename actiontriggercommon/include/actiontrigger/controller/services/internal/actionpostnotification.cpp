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

#include "actiontrigger/controller/services/internal/actionpostnotification.hpp"

namespace actiontrigger
{

    std::string ActionPostNotification::TYPE = "ACTION_ActionPostNotification";

    std::vector<ParameterDefinition*>* ActionPostNotification::parameterDefinitions = NULL;
    Logger* ActionPostNotification::StatementLOG = LoggerFactory::getInstance(
            "ActionPostNotification");
    const StatementInfo ActionPostNotification::info(false, "Display External Dialog",
            ActionPostNotification::TYPE,
            "Display a dialog outside of this application. Vibration and sound will be played according to your notification settings for this application and your current notification profile.");

    const StatementInfo ActionPostNotification::getInfo()
    {
        return ActionPostNotification::info;
    }

    ActionPostNotification::ActionPostNotification(StatementModel* model) :
            Action(model)
    {
    }

    ActionPostNotification::~ActionPostNotification()
    {
    }

    std::vector<ParameterDefinition*>* ActionPostNotification::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("TITLE", "Title", "Title of the dialog."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("BODY", "Message",
                            "Message content to display in dialog."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("BUTTONLABEL", "Button label",
                            "Button Label to display in the dialog."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("REPEAT", "Repeat sound",
                            "Repeat notification sound in a loop."));
        }
        return parameterDefinitions;
    }

    std::string ActionPostNotification::getDefaultParameter(std::string key)
    {
        if (key == "TITLE") {
            return "Hello";
        } else if (key == "BUTTONLABEL") {
            return "Dismiss";
        } else if (key == "REPEAT") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionPostNotification::getType() const
    {
        return ActionPostNotification::TYPE;
    }

    void ActionPostNotification::execute(ExecutionState* state)
    {
        QVariantMap data;
        data.insert("title", QString::fromStdString(getParameter("TITLE", state)));
        data.insert("body", QString::fromStdString(getParameter("BODY", state)));
        data.insert("buttonLabel", QString::fromStdString(getParameter("BUTTONLABEL", state)));
        data.insert("repeat", isValueTrue(getParameter("REPEAT", state)));
        GlobalResources::get()->requestNotification(data);
    }

} /* namespace actiontrigger */
