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

#include "actiontrigger/controller/services/noop/statementcorrupt.hpp"

namespace actiontrigger
{

    std::string StatementCorrupt::TYPE = "TRIGGER_StatementCorrupt";
    std::vector<ParameterDefinition*>* StatementCorrupt::parameterDefinitions = NULL;
    Logger* StatementCorrupt::StatementLOG = LoggerFactory::getInstance("StatementCorrupt");
    const StatementInfo StatementCorrupt::info(true, "-- Corrupted --", StatementCorrupt::TYPE,
            "This action could not be loaded due to data corruption. This is acting as a placeholder. Please contact the developer if this is not expected.");

    const StatementInfo StatementCorrupt::getInfo()
    {
        return StatementCorrupt::info;
    }

    StatementCorrupt::StatementCorrupt(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string StatementCorrupt::getType() const
    {
        return StatementCorrupt::TYPE;
    }

    StatementCorrupt::~StatementCorrupt()
    {
    }

    std::vector<ParameterDefinition*>* StatementCorrupt::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
        }
        return parameterDefinitions;
    }

    std::vector<statement_output_variable_definition*>* StatementCorrupt::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    bool StatementCorrupt::isActive(ExecutionState* state)
    {
        state->getLogger()->critical("Corrupted data, fix me please.");
        return true;
    }

    bool StatementCorrupt::isEventOnlyTrigger()
    {
        return false;
    }

} /* namespace actiontrigger */
