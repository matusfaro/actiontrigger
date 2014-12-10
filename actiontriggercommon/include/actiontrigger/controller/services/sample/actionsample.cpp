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

#include "actiontrigger/controller/services/sample/actionsample.hpp"

namespace actiontrigger
{

    std::string ActionSample::TYPE = "ACTION_ActionSample";

    std::vector<ParameterDefinition*>* ActionSample::parameterDefinitions = NULL;
    Logger* ActionSample::StatementLOG = LoggerFactory::getInstance("ActionSample");
    const StatementInfo ActionSample::info(false, "ActionSample", ActionSample::TYPE,
            "Set the sample value of sample");

    const StatementInfo ActionSample::getInfo()
    {
        return ActionSample::info;
    }

    ActionSample::ActionSample(StatementModel* model) :
            Action(model)
    {
    }

    ActionSample::~ActionSample()
    {
    }

    std::vector<ParameterDefinition*>* ActionSample::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Attempt to turn Wifi on or off", "Desired state");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionSample::getType() const
    {
        return ActionSample::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSample::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("Sample", "Sample value", "STRING"));
        return vars;
    }

    std::string ActionSample::getDefaultParameter(std::string key)
    {
        if (key == "Sample") {
            return "asdf";
        } else if (key == "Sample") {
            return "fdsa";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    void ActionSample::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        std::string sampleVar = getParameter("Sample", state);
        RUNLOG->debug("Sample" + sampleVar);
        std::string paramState = getParameter("STATE", state);
        bool desiredState;
        if (paramState.compare("on") == 0) {
            desiredState = true;
        } else if (paramState.compare("off") == 0) {
            desiredState = false;
        } else {
            RUNLOG->error("Parameter STATE is incorrect");
            StatementLOG->debug("Parameter is incorrect");
            return;
        }
        (void) desiredState;
    }

} /* namespace actiontrigger */
