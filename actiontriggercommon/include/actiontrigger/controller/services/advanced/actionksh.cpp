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

#include "actiontrigger/controller/services/advanced/actionksh.hpp"

namespace actiontrigger
{

    std::string ActionKsh::TYPE = "ACTION_ActionKsh";

    std::vector<ParameterDefinition*>* ActionKsh::parameterDefinitions = NULL;
    Logger* ActionKsh::StatementLOG = LoggerFactory::getInstance("ActionKsh");
    const StatementInfo ActionKsh::info(false, "Korn shell", ActionKsh::TYPE,
            "Executes commands in Korn shell (ksh). The variable OUTPUT will contain the STDOUT output from the execution.");

    const StatementInfo ActionKsh::getInfo()
    {
        return ActionKsh::info;
    }

    ActionKsh::ActionKsh(StatementModel* model) :
            Action(model)
    {
    }

    ActionKsh::~ActionKsh()
    {
    }

    std::vector<ParameterDefinition*>* ActionKsh::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CODE", "Code", "Commands to execute"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("TIMEOUT", "Timeout",
                            "Maximum amount of milliseconds to wait for execution to finish.",
                            "1000", "60000", "1000"));
        }
        return parameterDefinitions;
    }

    std::string ActionKsh::getDefaultParameter(std::string key)
    {
        if (key == "CODE") {
            return "print 'Hello!'";
        } else if (key == "TIMEOUT") {
            return "5000";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionKsh::getType() const
    {
        return ActionKsh::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionKsh::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("OUTPUT",
                        "Output of the invoked shell command", "STRING"));
        return vars;
    }

    void ActionKsh::execute(ExecutionState* state)
    {
        AdvancedTools::execute("/bin/ksh", QStringList(), getParameter("CODE", state), state,
                Common::StringToNumber<int>(getParameter("TIMEOUT", state)));
    }

} /* namespace actiontrigger */
