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

#include "actiontrigger/controller/services/sensors/triggerpressure.hpp"

namespace actiontrigger
{

    std::string TriggerPressure::TYPE = "TRIGGER_PRESSURE";
    Logger* TriggerPressure::StatementLOG = LoggerFactory::getInstance("TriggerPressure");
    const StatementInfo TriggerPressure::info(true, "Pressure", TriggerPressure::TYPE,
            RichText::html(
                    "Triggers on current pressure and temperature of sensor. Pressure is measured in Pascals and temperature in degrees Celsius."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool TriggerPressure::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_PRESSURE);
    }

    const StatementInfo TriggerPressure::getInfo()
    {
        return TriggerPressure::info;
    }

    TriggerPressure::TriggerPressure(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerPressure::getType() const
    {
        return TriggerPressure::TYPE;
    }

    TriggerPressure::~TriggerPressure()
    {
    }

    bool TriggerPressure::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerPressure::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("PRESSURE", "Pressure",
                        "Current pressure in Pascal", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("PRESSURE_TEMP",
                        "Pressure sensor temperature",
                        "Temperature reading of pressure sensor in degrees Celsius", "NUMBER"));
        return definition;
    }

    bool TriggerPressure::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PRESSURE, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: TEMP=" << event->pressure_s.temperature << " PRESSURE="
                                << event->pressure_s.pressure));
        values->push_back(/*PRESSURE*/SSTR(event->pressure_s.pressure));
        values->push_back(/*PRESSURE_TEMP*/SSTR(event->pressure_s.temperature));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerPressure::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PRESSURE, state,
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
                            "Retrieved: TEMP=" << event->pressure_s.temperature << " PRESSURE="
                                    << event->pressure_s.pressure));
            values->push_back(/*PRESSURE*/SSTR(event->pressure_s.pressure));
            values->push_back(/*PRESSURE_TEMP*/SSTR(event->pressure_s.temperature));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
