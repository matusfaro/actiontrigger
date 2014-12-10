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

#include "actiontrigger/controller/services/device/triggerholster.hpp"

namespace actiontrigger
{

    std::string TriggerHolster::TYPE = "TRIGGER_HOLSTER";
    Logger* TriggerHolster::StatementLOG = LoggerFactory::getInstance("TriggerHolster");
    const StatementInfo TriggerHolster::info(true, "Holster", TriggerHolster::TYPE,
            "Triggers on device's holster state. Returns TRUE if holstered, otherwise FALSE.");

    bool TriggerHolster::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_HOLSTER);
    }

    const StatementInfo TriggerHolster::getInfo()
    {
        return TriggerHolster::info;
    }

    TriggerHolster::TriggerHolster(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerHolster::getType() const
    {
        return TriggerHolster::TYPE;
    }

    TriggerHolster::~TriggerHolster()
    {
    }

    bool TriggerHolster::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerHolster::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("HOLSTERED", "Holstered",
                        "Device in a holster", "BOOL"));
        return definition;
    }

    bool TriggerHolster::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_HOLSTER, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: HOLSTER="
                                << (event->holster_s.holstered == 1 ? "TRUE" : "FALSE")));
        values->push_back(/*HOLSTER*/event->holster_s.holstered == 1 ? "TRUE" : "FALSE");
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerHolster::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_HOLSTER, state,
                isValueTrue(getParameter("HIGH_PRECISION", state)));
        if (!sensor)
            throw ExecutionAbortedException();
        sensor_event_t* event;
        std::vector<std::string>* values;
        for (;;) {
            // Check if we are still running
            if (!state->isRunning())
                throw ExecutionAbortedException();
            // Retrieve event with values
            event = sensor->retrieveEvent();
            // Check if event was successfully retrieved
            if (NULL == event)
                continue;
            // Check returned values
            values = new std::vector<std::string>();
            RUNLOG->debug(
                    SSTR(
                            "Retrieved: HOLSTER="
                                    << (event->holster_s.holstered == 1 ? "TRUE" : "FALSE")));
            values->push_back(/*HOLSTER*/event->holster_s.holstered == 1 ? "TRUE" : "FALSE");
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
