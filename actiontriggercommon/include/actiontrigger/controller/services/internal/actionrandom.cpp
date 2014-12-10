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

#include "actiontrigger/controller/services/internal/actionrandom.hpp"

namespace actiontrigger
{

    std::string ActionRandom::TYPE = "ACTION_ActionRandom";

    std::vector<ParameterDefinition*>* ActionRandom::parameterDefinitions = NULL;
    Logger* ActionRandom::StatementLOG = LoggerFactory::getInstance("ActionRandom");
    const StatementInfo ActionRandom::info(false, "Random number", ActionRandom::TYPE,
            "Sets a variable to a pseudo-random number within the requested range.");

    const StatementInfo ActionRandom::getInfo()
    {
        return ActionRandom::info;
    }

    ActionRandom::ActionRandom(StatementModel* model) :
            Action(model)
    {
    }

    ActionRandom::~ActionRandom()
    {
    }

    std::vector<ParameterDefinition*>* ActionRandom::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("FROM", "From", "Start of range inclusive."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("TO", "To", "End of range inclusive."));
        }
        return parameterDefinitions;
    }

    std::string ActionRandom::getDefaultParameter(std::string key)
    {
        if (key == "FROM") {
            return "0";
        } else if (key == "TO") {
            return "10";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionRandom::getType() const
    {
        return ActionRandom::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRandom::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("MATH_RANDOM",
                        SSTR(
                                "Random number in the given range. [" << getParameter("MIN") << ","
                                        << getParameter("MAX") << "]"), "NUMBER"));
        return vars;
    }

    void ActionRandom::execute(ExecutionState* state)
    {
        std::string op = getParameter("OPERATION");

        double numOne, numTwo;
        try {
            numOne = Common::StringToNumber<double>(getParameter("FROM", state));
        } catch (...) {
            state->getLogger()->error(
                    "Invalid minimum value of range: " + getParameter("FROM", state));
            return;
        }
        try {
            numTwo = Common::StringToNumber<double>(getParameter("TO", state));
        } catch (...) {
            state->getLogger()->error(
                    "Invalid maximum value of range: " + getParameter("TO", state));
            return;
        }

        double max = numOne > numTwo ? numOne : numTwo;
        double min = numOne > numTwo ? numTwo : numOne;

        state->setUserProperty("MATH_RANDOM", SSTR(min + (rand() % (int )(max - min))));
    }

} /* namespace actiontrigger */
