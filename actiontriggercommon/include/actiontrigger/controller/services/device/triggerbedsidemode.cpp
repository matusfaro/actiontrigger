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

#include "actiontrigger/controller/services/device/triggerbedsidemode.hpp"

namespace actiontrigger
{

    std::string TriggerBedsideMode::TYPE = "TRIGGER_BEDSIDE_MODE";
    Logger* TriggerBedsideMode::StatementLOG = LoggerFactory::getInstance("TriggerBedsideMode");
    const StatementInfo TriggerBedsideMode::info(true, "Bedside Mode", TriggerBedsideMode::TYPE,
            "Trigger on bedside mode.");

    const StatementInfo TriggerBedsideMode::getInfo()
    {
        return TriggerBedsideMode::info;
    }

    TriggerBedsideMode::TriggerBedsideMode(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerBedsideMode::getType() const
    {
        return TriggerBedsideMode::TYPE;
    }

    TriggerBedsideMode::~TriggerBedsideMode()
    {
    }

    bool TriggerBedsideMode::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerBedsideMode::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("BEDSIDE_MODE", "Bedside Mode",
                        "Is bedside mode active. Either TRUE or FALSE.", "BOOL"));
        return definition;
    }

    bool TriggerBedsideMode::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        bb::platform::HomeScreen homeScreen;
        bool isBedsideModeActive = homeScreen.isBedsideModeActive();
        RUNLOG->debug(SSTR("Retrieved bedside mode: " << (isBedsideModeActive ? "TRUE" : "FALSE")));
        values->push_back(/*BEDSIDE_MODE*/homeScreen.isBedsideModeActive() ? "TRUE" : "FALSE");
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
