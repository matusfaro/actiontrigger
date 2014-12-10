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

#include "actiontrigger/controller/services/sensors/triggertemperature.hpp"

namespace actiontrigger
{

    std::string TriggerTemperature::TYPE = "TRIGGER_TEMPERATURE";
    Logger* TriggerTemperature::StatementLOG = LoggerFactory::getInstance("TriggerTemperature");
    const StatementInfo TriggerTemperature::info(true, "Temperature", TriggerTemperature::TYPE,
            RichText::html(
                    "Triggers on device's ambient temperature measured in degrees Celsius."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool TriggerTemperature::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_TEMPERATURE);
    }

    const StatementInfo TriggerTemperature::getInfo()
    {
        return TriggerTemperature::info;
    }

    TriggerTemperature::TriggerTemperature(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerTemperature::getType() const
    {
        return TriggerTemperature::TYPE;
    }

    TriggerTemperature::~TriggerTemperature()
    {
    }

    bool TriggerTemperature::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerTemperature::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("TEMPERATURE", "Temperature",
                        "The ambient temperature measured in degrees Celsius.", "NUMBER"));
        return definition;
    }

    bool TriggerTemperature::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_TEMPERATURE, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(SSTR("Retrieved: TEMP=" << event->temperature_s.temperature));
        values->push_back(/*TEMPERATURE*/SSTR(event->temperature_s.temperature));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerTemperature::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_TEMPERATURE, state,
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
            RUNLOG->debug(SSTR("Retrieved: TEMP=" << event->temperature_s.temperature));
            values->push_back(/*TEMPERATURE*/SSTR(event->temperature_s.temperature));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
