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

#include "actiontrigger/controller/services/advanced/actionjavascript.hpp"

namespace actiontrigger
{

    std::string ActionJavascript::TYPE = "ACTION_ActionJavascript";

    std::vector<ParameterDefinition*>* ActionJavascript::parameterDefinitions = NULL;
    Logger* ActionJavascript::StatementLOG = LoggerFactory::getInstance("ActionJavascript");
    const StatementInfo ActionJavascript::info(false, "Javascript Eval", ActionJavascript::TYPE,
            "Executes code in JavaScriptCore. To print output, use the debug() method. The variable OUTPUT will contain the result.");

    const StatementInfo ActionJavascript::getInfo()
    {
        return ActionJavascript::info;
    }

    ActionJavascript::ActionJavascript(StatementModel* model) :
            Action(model)
    {
    }

    ActionJavascript::~ActionJavascript()
    {
    }

    std::vector<ParameterDefinition*>* ActionJavascript::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CODE", "Javascript", "Javascript to execute"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("TIMEOUT", "Timeout",
                            "Maximum amount of milliseconds to wait for execution to finish.",
                            "1000", "60000", "1000"));
        }
        return parameterDefinitions;
    }

    std::string ActionJavascript::getDefaultParameter(std::string key)
    {
        if (key == "CODE") {
            return "debug('Hello!')";
        } else if (key == "TIMEOUT") {
            return "5000";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionJavascript::getType() const
    {
        return ActionJavascript::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionJavascript::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("OUTPUT",
                        "Output of the invoked javascript", "STRING"));
        return vars;
    }

    void ActionJavascript::execute(ExecutionState* state)
    {
        //QString::fromStdString(AdvancedTools::invoke("`ls -r /usr/bin/python* | tail -1`"))
        AdvancedTools::execute("/usr/bin/jsc", QStringList(), getParameter("CODE", state), state,
                Common::StringToNumber<int>(getParameter("TIMEOUT", state)), true);
    }

} /* namespace actiontrigger */
