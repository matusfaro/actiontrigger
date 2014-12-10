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

#include "actiontrigger/controller/services/advanced/actionboottime.hpp"

namespace actiontrigger
{

    std::string ActionBootTime::TYPE = "ACTION_ActionBootTime";

    std::vector<ParameterDefinition*>* ActionBootTime::parameterDefinitions = NULL;
    Logger* ActionBootTime::StatementLOG = LoggerFactory::getInstance("ActionBootTime");
    const StatementInfo ActionBootTime::info(false, "System boot time", ActionBootTime::TYPE,
            "Retrieves the time when device last started in seconds since the Unix Epoch");

    const StatementInfo ActionBootTime::getInfo()
    {
        return ActionBootTime::info;
    }

    ActionBootTime::ActionBootTime(StatementModel* model) :
            Action(model)
    {
    }

    ActionBootTime::~ActionBootTime()
    {
    }

    std::string ActionBootTime::getType() const
    {
        return ActionBootTime::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionBootTime::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("BOOT_TIME_EPOCH",
                        "Device boot time in seconds since Unix Epoch", "STRING"));
        return vars;
    }

    void ActionBootTime::execute(ExecutionState* state)
    {
        state->setUserProperty("BOOT_TIME_EPOCH", SSTR(AdvancedTools::getBootTime()));
    }

} /* namespace actiontrigger */
