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

#include "actiontrigger/controller/services/sensors/triggeraltimeter.hpp"

namespace actiontrigger
{

    std::string TriggerAltimeter::TYPE = "TRIGGER_ALTIMETER";
    Logger* TriggerAltimeter::StatementLOG = LoggerFactory::getInstance("TriggerAltimeter");
    const StatementInfo TriggerAltimeter::info(true, "Altimeter", TriggerAltimeter::TYPE,
            RichText::html(
                    "Triggers on device altitude reported in meters relative to mean sea level. On BlackBerry, the altimeter uses a combination of pressure and location to determine the altitude, as using pressure alone would yield to inaccurate results due to changes in air pressure caused by the weather. The location information is used to compensate for the weather. This requires that the user has enabled location services in the global settings."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool TriggerAltimeter::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ALTIMETER);
    }

    const StatementInfo TriggerAltimeter::getInfo()
    {
        return TriggerAltimeter::info;
    }

    TriggerAltimeter::TriggerAltimeter(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerAltimeter::getType() const
    {
        return TriggerAltimeter::TYPE;
    }

    TriggerAltimeter::~TriggerAltimeter()
    {
    }

    bool TriggerAltimeter::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerAltimeter::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("ALTITUDE", "Acceleration x-axis",
                        "The altitude in meters relative to mean sea level", "NUMBER"));
        return definition;
    }

    bool TriggerAltimeter::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ALTIMETER, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(SSTR("Retrieved: ALT=" << event->altitude_s.altitude));
        values->push_back(/*ALTITUDE*/SSTR(event->altitude_s.altitude));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerAltimeter::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ALTIMETER, state,
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
            RUNLOG->debug(SSTR("Retrieved: ALT=" << event->altitude_s.altitude));
            values->push_back(/*ALTITUDE*/SSTR(event->altitude_s.altitude));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
