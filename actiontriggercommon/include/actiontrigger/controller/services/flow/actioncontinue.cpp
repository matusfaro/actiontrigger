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

#include "actiontrigger/controller/services/flow/actioncontinue.hpp"

namespace actiontrigger
{

    std::string ActionContinue::TYPE = "ACTION_ActionContinue";

    std::vector<ParameterDefinition*>* ActionContinue::parameterDefinitions = NULL;
    Logger* ActionContinue::StatementLOG = LoggerFactory::getInstance("ActionContinue");
    const StatementInfo ActionContinue::info(false, "Loop continue", ActionContinue::TYPE,
            "Continues with the next iteration of the loop. If no loops are present, execution will stop");

    const StatementInfo ActionContinue::getInfo()
    {
        return ActionContinue::info;
    }

    ActionContinue::ActionContinue(StatementModel* model) :
            Action(model)
    {
    }

    ActionContinue::~ActionContinue()
    {
    }

    std::string ActionContinue::getType() const
    {
        return ActionContinue::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionContinue::getExposedVariableDefinitions()
    {
        return new std::vector<statement_output_variable_definition*>();
    }

    void ActionContinue::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->debug("Sending loop continue..");
        throw LoopContinueException();
    }

} /* namespace actiontrigger */
