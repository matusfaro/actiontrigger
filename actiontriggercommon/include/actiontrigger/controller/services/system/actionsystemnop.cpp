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

#include "actiontrigger/controller/services/system/actionsystemnop.hpp"

namespace actiontrigger
{

    std::vector<ParameterDefinition*>* ActionSystemNop::parameterDefinitions = NULL;
    std::string ActionSystemNop::TYPE = "ACTION_NOP";
    Logger* ActionSystemNop::StatementLOG = LoggerFactory::getInstance("SystemNop");
    const StatementInfo ActionSystemNop::info(false, "System Nop", ActionSystemNop::TYPE,
            "Does nothing");

    const StatementInfo ActionSystemNop::getInfo()
    {
        return ActionSystemNop::info;
    }

    ActionSystemNop::ActionSystemNop(StatementModel* model) :
            Action(model)
    {
        StatementLOG->trace("SystemNop(model,childAction)");
    }

    ActionSystemNop::~ActionSystemNop()
    {
        StatementLOG->trace("~SystemNop");
    }

    std::vector<ParameterDefinition*>* ActionSystemNop::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
        }
        return parameterDefinitions;
    }

    std::string ActionSystemNop::getType() const
    {
        return ActionSystemNop::TYPE;
    }

    void ActionSystemNop::execute(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        USERLOG->debug("Doing nothing");
    }

} /* namespace actiontrigger */
