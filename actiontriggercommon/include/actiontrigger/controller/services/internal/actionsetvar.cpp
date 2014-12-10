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

#include "actiontrigger/controller/services/internal/actionsetvar.hpp"

namespace actiontrigger
{

    std::string ActionSetVar::TYPE = "ACTION_ActionSetVar";

    std::vector<ParameterDefinition*>* ActionSetVar::parameterDefinitions = NULL;
    Logger* ActionSetVar::StatementLOG = LoggerFactory::getInstance("ActionSetVar");
    const StatementInfo ActionSetVar::info(false, "Set variable", ActionSetVar::TYPE,
            "Sets a variable. Useful for creating new variable, adding extra text to variable or combining multiple variables into one.");

    const StatementInfo ActionSetVar::getInfo()
    {
        return ActionSetVar::info;
    }

    ActionSetVar::ActionSetVar(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetVar::~ActionSetVar()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetVar::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("NAME", "Name",
                            "Name of the variable to set. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$", false));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("CONTENT", "Content", "Content of the variable",
                            ""));
        }
        return parameterDefinitions;
    }

    std::string ActionSetVar::getType() const
    {
        return ActionSetVar::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSetVar::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        std::string name = getParameter("NAME");
        if (checkRegexMatch(name, "^[A-Za-z_]*$")) {
            vars->push_back(
                    new statement_output_variable_definition(name, "User defined variable",
                            "UNKNOWN"));
        }
        return vars;
    }

    void ActionSetVar::execute(ExecutionState* state)
    {
        state->setUserProperty(getParameter("NAME", state), getParameter("CONTENT", state));
    }

} /* namespace actiontrigger */
