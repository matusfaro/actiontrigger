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

#include "actiontrigger/controller/services/internal/actiondisplaytoast.hpp"

namespace actiontrigger
{

    std::string ActionDisplayToast::TYPE = "ACTION_ActionDisplayToast";

    std::vector<ParameterDefinition*>* ActionDisplayToast::parameterDefinitions = NULL;
    Logger* ActionDisplayToast::StatementLOG = LoggerFactory::getInstance("ActionDisplayToast");
    const StatementInfo ActionDisplayToast::info(false, "Display Message Bubble",
            ActionDisplayToast::TYPE, "Display a custom message within the application.");

    const StatementInfo ActionDisplayToast::getInfo()
    {
        return ActionDisplayToast::info;
    }

    ActionDisplayToast::ActionDisplayToast(StatementModel* model) :
            Action(model)
    {
    }

    ActionDisplayToast::~ActionDisplayToast()
    {
    }

    std::vector<ParameterDefinition*>* ActionDisplayToast::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CONTENT", "Content", "Message content"));
        }
        return parameterDefinitions;
    }

    std::string ActionDisplayToast::getDefaultParameter(std::string key)
    {
        if (key == "CONTENT") {
            return "Hello!";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionDisplayToast::getType() const
    {
        return ActionDisplayToast::TYPE;
    }

    void ActionDisplayToast::execute(ExecutionState* state)
    {
        GlobalResources::get()->sendUserFacingMessage(
                QString::fromStdString(getParameter("CONTENT", state)));
    }

} /* namespace actiontrigger */
