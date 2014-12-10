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

#include "actiontrigger/controller/services/internal/actiongetglobalvar.hpp"

namespace actiontrigger
{

    std::string ActionGetGlobalVar::TYPE = "ACTION_ActionGetGlobalVar";

    std::vector<ParameterDefinition*>* ActionGetGlobalVar::parameterDefinitions = NULL;
    Logger* ActionGetGlobalVar::StatementLOG = LoggerFactory::getInstance("ActionGetGlobalVar");
    const StatementInfo ActionGetGlobalVar::info(false, "Get global variable",
            ActionGetGlobalVar::TYPE,
            "Gets a global variable and stores value in a local variable. Global variables are accessible from all other procedures and persist as long as this application is running.");

    const StatementInfo ActionGetGlobalVar::getInfo()
    {
        return ActionGetGlobalVar::info;
    }

    ActionGetGlobalVar::ActionGetGlobalVar(StatementModel* model) :
            Action(model)
    {
    }

    ActionGetGlobalVar::~ActionGetGlobalVar()
    {
    }

    std::vector<ParameterDefinition*>* ActionGetGlobalVar::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("GLOBAL_NAME", "Global name",
                            "Name of the global variable to get. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$", false));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("LOCAL_NAME", "Local name",
                            "Name of the local variable to set. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("DEFAULT_VALUE", "Default value",
                            "If variable was not previously set, use this default value instead. A default value will not be used if the variable was previously set with an empty string."));
        }
        return parameterDefinitions;
    }

    std::string ActionGetGlobalVar::getType() const
    {
        return ActionGetGlobalVar::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGetGlobalVar::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        std::string name = getParameter("LOCAL_NAME");
        if (checkRegexMatch(name, "^[A-Za-z_]*$")) {
            vars->push_back(
                    new statement_output_variable_definition(name, "User defined variable",
                            "UNKNOWN"));
        }
        return vars;
    }

    void ActionGetGlobalVar::execute(ExecutionState* state)
    {
        state->setUserProperty(getParameter("LOCAL_NAME", state),
                state->getGlobalResources()->getUserProperty(getParameter("GLOBAL_NAME", state),
                        getParameter("DEFAULT_VALUE", state)));
    }

} /* namespace actiontrigger */
