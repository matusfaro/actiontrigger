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

#include "actiontrigger/controller/services/clock/actionhalt.hpp"

namespace actiontrigger
{

    std::string ActionHalt::TYPE = "ACTION_HALT";
    std::vector<ParameterDefinition*>* ActionHalt::parameterDefinitions = NULL;
    Logger* ActionHalt::StatementLOG = LoggerFactory::getInstance("ActionHalt");
    const StatementInfo ActionHalt::info(false, "Halt", ActionHalt::TYPE,
            "Halts this execution forever.");

    const StatementInfo ActionHalt::getInfo()
    {
        return ActionHalt::info;
    }

    ActionHalt::ActionHalt(StatementModel* model) :
            Action(model)
    {
    }

    ActionHalt::~ActionHalt()
    {
    }

    std::string ActionHalt::getType() const
    {
        return ActionHalt::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionHalt::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionHalt::execute(ExecutionState* state)
    {
        QDateTime datetime;
        state->getLogger()->debug("Halting execution");
        // Sleep forever... Until woken up due to execution stop.
        state->getRuntimeResources()->sleepSafe();
    }

} /* namespace actiontrigger */
