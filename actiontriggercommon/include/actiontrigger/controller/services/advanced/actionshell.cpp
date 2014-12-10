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

#include "actiontrigger/controller/services/advanced/actionshell.hpp"

namespace actiontrigger
{

    std::string ActionShell::TYPE = "ACTION_ActionShell";

    std::vector<ParameterDefinition*>* ActionShell::parameterDefinitions = NULL;
    Logger* ActionShell::StatementLOG = LoggerFactory::getInstance("ActionShell");
    const StatementInfo ActionShell::info(false, "Invoke shell command", ActionShell::TYPE,
            "Invokes a shell command via popen() by creating a pipe, forking, and invoking the shell.");

    const StatementInfo ActionShell::getInfo()
    {
        return ActionShell::info;
    }

    ActionShell::ActionShell(StatementModel* model) :
            Action(model)
    {
    }

    ActionShell::~ActionShell()
    {
    }

    std::vector<ParameterDefinition*>* ActionShell::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("COMMAND", "Command", "Command to invoke", ""));
        }
        return parameterDefinitions;
    }

    std::string ActionShell::getDefaultParameter(std::string key)
    {
        if (key == "COMMAND") {
            return "uname -a";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionShell::getType() const
    {
        return ActionShell::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionShell::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("COMMAND_OUTPUT",
                        "Output of the invoked shell command", "STRING"));
        return vars;
    }

    void ActionShell::execute(ExecutionState* state)
    {
        state->setUserProperty("COMMAND_OUTPUT",
                AdvancedTools::invoke(getParameter("COMMAND", state)));
    }

} /* namespace actiontrigger */
