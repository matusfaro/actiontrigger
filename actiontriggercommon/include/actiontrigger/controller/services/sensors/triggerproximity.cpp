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

#include "actiontrigger/controller/services/sensors/triggerproximity.hpp"

namespace actiontrigger
{

    std::string TriggerProximity::TYPE = "TRIGGER_PROXIMITY";
    Logger* TriggerProximity::StatementLOG = LoggerFactory::getInstance("TriggerProximity");
    const StatementInfo TriggerProximity::info(true, "Proximity", TriggerProximity::TYPE,
            "Trigger on proximity sensor. Able to detect an object short distance from the phone. Sensor is typically positioned near the LED on the device. Original use case is to lock the screen when the phone is held against an ear during a phone call.");

    bool TriggerProximity::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_PROXIMITY);
    }

    const StatementInfo TriggerProximity::getInfo()
    {
        return TriggerProximity::info;
    }

    TriggerProximity::TriggerProximity(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerProximity::getType() const
    {
        return TriggerProximity::TYPE;
    }

    TriggerProximity::~TriggerProximity()
    {
    }

    bool TriggerProximity::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerProximity::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("PROX_RANGE_MIN", "Proximity range minimum",
                        "Minimum sensor range in centimeters", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("PROX_RANGE_MAX", "Proximity range maximum",
                        "mum sensor range in centimeters", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("PROX_DISTANCE", "Proximity distance",
                        "Discrete steps of distance or actual value in centimeters. Only able to detect values in range of PROX_RANGE_MIN to PROX_RANGE_MAX.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("PROX_NORMALIZED",
                        "Proximity distance normalized",
                        "Range from 0.0 to 1.0 (close to far), normalized unit-less signal from raw sensor.",
                        "NUMBER"));
        return definition;
    }

    bool TriggerProximity::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PROXIMITY, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        sensor_info_t* info = sensor->getSensorInfo();
        delete sensor;
        if (NULL == event || NULL == info)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: MIN=" << info->range_min << " MAX=" << info->range_max
                                << " DIST=" << event->proximity_s.distance << " NORM="
                                << event->proximity_s.normalized));
        values->push_back(/*PROX_RANGE_MIN*/SSTR(info->range_min));
        values->push_back(/*PROX_RANGE_MAX*/SSTR(info->range_max));
        values->push_back(/*PROX_DISTANCE*/SSTR(event->proximity_s.distance));
        values->push_back(/*PROX_NORMALIZED*/SSTR(event->proximity_s.normalized));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerProximity::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PROXIMITY, state,
                isValueTrue(getParameter("HIGH_PRECISION", state)));
        if (!sensor)
            throw ExecutionAbortedException();
        sensor_event_t* event;
        sensor_info_t* info = sensor->getSensorInfo();
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
                            "Retrieved: MIN=" << info->range_min << " MAX=" << info->range_max
                                    << " DIST=" << event->proximity_s.distance << " NORM="
                                    << event->proximity_s.normalized));
            values->push_back(/*PROX_RANGE_MIN*/SSTR(info->range_min));
            values->push_back(/*PROX_RANGE_MAX*/SSTR(info->range_max));
            values->push_back(/*PROX_DISTANCE*/SSTR(event->proximity_s.distance));
            values->push_back(/*PROX_NORMALIZED*/SSTR(event->proximity_s.normalized));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
