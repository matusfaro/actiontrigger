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

#include "actiontrigger/controller/services/system/triggersystemtrue.hpp"

namespace actiontrigger
{

    std::vector<ParameterDefinition*>* TriggerSystemTrue::parameterDefinitions = NULL;
    std::string TriggerSystemTrue::TYPE = "TRIGGER_TRUE";
    Logger* TriggerSystemTrue::StatementLOG = LoggerFactory::getInstance("SystemTrue");
    const StatementInfo TriggerSystemTrue::info(true, "True", TriggerSystemTrue::TYPE,
            "Always triggers.");

    const StatementInfo TriggerSystemTrue::getInfo()
    {
        return TriggerSystemTrue::info;
    }

    TriggerSystemTrue::TriggerSystemTrue(StatementModel* model) :
            Trigger(model)
    {
        StatementLOG->trace("ConsoleOut(model,childAction)");
    }

    std::string TriggerSystemTrue::getType() const
    {
        return TriggerSystemTrue::TYPE;
    }

    TriggerSystemTrue::~TriggerSystemTrue()
    {
    }

    std::vector<ParameterDefinition*>* TriggerSystemTrue::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
        }
        return parameterDefinitions;
    }

    bool TriggerSystemTrue::isActive(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        USERLOG->debug("Returning true as always");
        return true;
    }
    bool TriggerSystemTrue::isEventOnlyTrigger()
    {
        return false;
    }
    void TriggerSystemTrue::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        USERLOG->debug("Returning true as always");
        return;
    }

} /* namespace actiontrigger */
