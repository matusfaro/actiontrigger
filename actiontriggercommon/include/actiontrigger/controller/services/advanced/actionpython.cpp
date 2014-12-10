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

#include "actiontrigger/controller/services/advanced/actionpython.hpp"

namespace actiontrigger
{

    std::string ActionPython::TYPE = "ACTION_ActionPython";

    std::vector<ParameterDefinition*>* ActionPython::parameterDefinitions = NULL;
    Logger* ActionPython::StatementLOG = LoggerFactory::getInstance("ActionPython");
    const StatementInfo ActionPython::info(false, "Python Interpreter", ActionPython::TYPE,
            "Interprets Python3.2 code. The variable OUTPUT will contain the STDOUT output from the execution.");

    const StatementInfo ActionPython::getInfo()
    {
        return ActionPython::info;
    }

    ActionPython::ActionPython(StatementModel* model) :
            Action(model)
    {
    }

    ActionPython::~ActionPython()
    {
    }

    std::vector<ParameterDefinition*>* ActionPython::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CODE", "Code", "Python code to execute"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("TIMEOUT", "Timeout",
                            "Maximum amount of milliseconds to wait for execution to finish.",
                            "1000", "60000", "1000"));
        }
        return parameterDefinitions;
    }

    std::string ActionPython::getDefaultParameter(std::string key)
    {
        if (key == "CODE") {
            return "print('Hello!')";
        } else if (key == "TIMEOUT") {
            return "5000";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionPython::getType() const
    {
        return ActionPython::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionPython::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("OUTPUT",
                        "Output of the invoked python script", "STRING"));
        return vars;
    }

    void ActionPython::execute(ExecutionState* state)
    {
        //QString::fromStdString(AdvancedTools::invoke("`ls -r /usr/bin/python* | tail -1`"))
        AdvancedTools::execute("/usr/bin/python3.2", QStringList(), getParameter("CODE", state),
                state, Common::StringToNumber<int>(getParameter("TIMEOUT", state)));
    }

} /* namespace actiontrigger */
