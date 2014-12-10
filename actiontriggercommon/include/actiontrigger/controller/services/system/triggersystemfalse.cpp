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

#include "actiontrigger/controller/services/system/triggersystemfalse.hpp"

namespace actiontrigger
{

    std::vector<ParameterDefinition*>* TriggerSystemFalse::parameterDefinitions = NULL;
    std::string TriggerSystemFalse::TYPE = "TRIGGER_TriggerSystemFalse";
    Logger* TriggerSystemFalse::StatementLOG = LoggerFactory::getInstance("TriggerSystemFalse");
    const StatementInfo TriggerSystemFalse::info(true, "False", TriggerSystemFalse::TYPE,
            "Never triggers.");

    const StatementInfo TriggerSystemFalse::getInfo()
    {
        return TriggerSystemFalse::info;
    }

    TriggerSystemFalse::TriggerSystemFalse(StatementModel* model) :
            Trigger(model)
    {
        StatementLOG->trace("ConsoleOut(model,childAction)");
    }

    std::string TriggerSystemFalse::getType() const
    {
        return TriggerSystemFalse::TYPE;
    }

    TriggerSystemFalse::~TriggerSystemFalse()
    {
    }

    std::vector<ParameterDefinition*>* TriggerSystemFalse::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
        }
        return parameterDefinitions;
    }

    bool TriggerSystemFalse::isActive(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        USERLOG->debug("Triggered");
        return false;
    }
    bool TriggerSystemFalse::isEventOnlyTrigger()
    {
        return false;
    }
    void TriggerSystemFalse::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        USERLOG->warning("Blocked forever");
        useconds_t granularity = getGranularity(state);
        for (;;) {
            state->giveUpIfNotRunning(); // Check if we should continue
            if (granularity != 0)
                usleep(granularity);
        }
    }

} /* namespace actiontrigger */
