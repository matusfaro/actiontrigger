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

#include "actiontrigger/controller/services/sample/triggersample.hpp"

namespace actiontrigger
{

    std::string TriggerSample::TYPE = "TRIGGER_TriggerSample";
    std::vector<ParameterDefinition*>* TriggerSample::parameterDefinitions = NULL;
    Logger* TriggerSample::StatementLOG = LoggerFactory::getInstance("TriggerSample");
    const StatementInfo TriggerSample::info(true, "Sample", TriggerSample::TYPE,
            "Check current sample status");

    const StatementInfo TriggerSample::getInfo()
    {
        return TriggerSample::info;
    }

    TriggerSample::TriggerSample(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerSample::getType() const
    {
        return TriggerSample::TYPE;
    }

    TriggerSample::~TriggerSample()
    {
    }

    std::vector<ParameterDefinition*>* TriggerSample::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Trigger when Wifi status is 'on' or 'off'", "Wait until");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("GRANULARITY", "Granularity",
                            "Amount of time (milliseconds) to wait between checking again", "500",
                            "10000", "500"));
        }
        return parameterDefinitions;
    }

    std::string TriggerSample::getDefaultParameter(std::string key)
    {
        if (key == "Sample") {
            return "asdf";
        } else if (key == "Sample") {
            return "fdsa";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::vector<statement_output_variable_definition*>* TriggerSample::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("Sample", "Sample value", "STRING"));
        return vars;
    }

    bool TriggerSample::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        RUNLOG->debug("Sample");
        std::string paramState = getParameter("STATE", state);
        return true;
    }

    bool TriggerSample::isEventOnlyTrigger()
    {
        return false;
    }

} /* namespace actiontrigger */
