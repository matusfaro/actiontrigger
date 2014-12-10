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

#include "actiontrigger/controller/services/clock/actionsleep.hpp"

namespace actiontrigger
{

    std::string ActionSleep::TYPE = "ACTION_SLEEP";
    std::vector<ParameterDefinition*>* ActionSleep::parameterDefinitions = NULL;
    Logger* ActionSleep::StatementLOG = LoggerFactory::getInstance("ActionSleep");
    const StatementInfo ActionSleep::info(false, "Sleep", ActionSleep::TYPE,
            "Pauses execution for the specified amount of seconds");

    const StatementInfo ActionSleep::getInfo()
    {
        return ActionSleep::info;
    }

    ActionSleep::ActionSleep(StatementModel* model) :
            Action(model)
    {
    }

    ActionSleep::~ActionSleep()
    {
    }

    std::vector<ParameterDefinition*>* ActionSleep::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("DURATION", "Sleep duration",
                            "Amount of seconds to pause execution."));
        }
        return parameterDefinitions;
    }

    std::string ActionSleep::getDefaultParameter(std::string key)
    {
        if (key == "DURATION") {
            return "1";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionSleep::getType() const
    {
        return ActionSleep::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSleep::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionSleep::execute(ExecutionState* state)
    {
        QDateTime datetime;
        int duration = atoi(getParameter("DURATION").c_str());
        if (duration <= 0) {
            return;
        }
        state->getLogger()->debug(SSTR("Sleeping for " << duration << " seconds"));
        if (state->getRuntimeResources()->sleepSafe(((unsigned long) duration * 1000))) {
            LOG->trace("Forcefully woken up");
        }
        state->getLogger()->debug("Woke up");
    }

} /* namespace actiontrigger */
