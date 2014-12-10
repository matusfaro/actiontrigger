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

#include "actiontrigger/controller/services/internal/actionsetglobalvar.hpp"

namespace actiontrigger
{

    std::string ActionSetGlobalVar::TYPE = "ACTION_ActionSetGlobalVar";

    std::vector<ParameterDefinition*>* ActionSetGlobalVar::parameterDefinitions = NULL;
    Logger* ActionSetGlobalVar::StatementLOG = LoggerFactory::getInstance("ActionSetGlobalVar");
    const StatementInfo ActionSetGlobalVar::info(false, "Set global variable",
            ActionSetGlobalVar::TYPE,
            "Sets a global variable from an execution variable. Global variables are accessible from all other procedures and persist as long as this application is running.");

    const StatementInfo ActionSetGlobalVar::getInfo()
    {
        return ActionSetGlobalVar::info;
    }

    ActionSetGlobalVar::ActionSetGlobalVar(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetGlobalVar::~ActionSetGlobalVar()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetGlobalVar::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("NAME", "Name",
                            "Name of the global variable to set. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$", false));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("CONTENT", "Content", "Content of the variable",
                            ""));
        }
        return parameterDefinitions;
    }

    std::string ActionSetGlobalVar::getType() const
    {
        return ActionSetGlobalVar::TYPE;
    }

    void ActionSetGlobalVar::execute(ExecutionState* state)
    {
        state->getGlobalResources()->setUserProperty(getParameter("NAME", state),
                getParameter("CONTENT", state));
    }

} /* namespace actiontrigger */
