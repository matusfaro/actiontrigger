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

#include "actiontrigger/controller/services/device/triggercustom.hpp"

namespace actiontrigger
{

    std::string TriggerCustom::TYPE = "TRIGGER_TriggerCustom";
    Logger* TriggerCustom::StatementLOG = LoggerFactory::getInstance("TriggerCustom");
    const StatementInfo TriggerCustom::info(true, "Variable Trigger", TriggerCustom::TYPE,
            "Triggers on input text hopefully containing variable(s).");

    const StatementInfo TriggerCustom::getInfo()
    {
        return TriggerCustom::info;
    }

    TriggerCustom::TriggerCustom(StatementModel* model) :
            TriggerValue(model), parameterDefinitions(NULL)
    {
    }

    std::string TriggerCustom::getType() const
    {
        return TriggerCustom::TYPE;
    }

    TriggerCustom::~TriggerCustom()
    {
    }

    bool TriggerCustom::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<statement_output_variable_definition*>* TriggerCustom::getExposedVariableDefinitions()
    {
        // We don't need to expose INPUT variable
        return new std::vector<statement_output_variable_definition*>();
    }

    std::vector<ParameterDefinition*>* TriggerCustom::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            // Get Parent's definitions for requirements
            parameterDefinitions = TriggerValue::getParameterDefinitions();

            parameterDefinitions->insert(parameterDefinitions->begin(),
                    ParameterDefinition::getText("INPUT", "Input",
                            "Input text to match. Should contain variables."));
        }
        return parameterDefinitions;
    }

    std::vector<trigger_output_variable_definition*>* TriggerCustom::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("INPUT", "Requirement",
                        "Requirement for input text.", "VARIABLE"));
        return definition;
    }

    bool TriggerCustom::isActive(ExecutionState* state)
    {
        std::vector<std::string>* values = new std::vector<std::string>();
        values->push_back(/*INPUT*/getParameter("INPUT", state));
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
