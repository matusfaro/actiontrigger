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

#include "actiontrigger/controller/services/flow/actionbreak.hpp"

namespace actiontrigger
{

    std::string ActionBreak::TYPE = "ACTION_ActionBreak";

    std::vector<ParameterDefinition*>* ActionBreak::parameterDefinitions = NULL;
    Logger* ActionBreak::StatementLOG = LoggerFactory::getInstance("ActionBreak");
    const StatementInfo ActionBreak::info(false, "Loop break", ActionBreak::TYPE,
            "Breaks out of the loop. Execution of the next item after the loop will continue. If no loops are present, execution will stop");

    const StatementInfo ActionBreak::getInfo()
    {
        return ActionBreak::info;
    }

    ActionBreak::ActionBreak(StatementModel* model) :
            Action(model)
    {
    }

    ActionBreak::~ActionBreak()
    {
    }

    std::string ActionBreak::getType() const
    {
        return ActionBreak::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionBreak::getExposedVariableDefinitions()
    {
        return new std::vector<statement_output_variable_definition*>();
    }

    void ActionBreak::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        RUNLOG->debug("Sending loop break..");
        throw LoopBreakException();
    }

} /* namespace actiontrigger */
