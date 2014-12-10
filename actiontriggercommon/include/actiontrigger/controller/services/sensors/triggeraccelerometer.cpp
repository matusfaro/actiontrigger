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

#include "actiontrigger/controller/services/sensors/triggeraccelerometer.hpp"

namespace actiontrigger
{

    std::string TriggerAccelerometer::TYPE = "TRIGGER_ACCELEROMETER";
    Logger* TriggerAccelerometer::StatementLOG = LoggerFactory::getInstance("TriggerAccelerometer");
    const StatementInfo TriggerAccelerometer::info(true, "Accelerometer",
            TriggerAccelerometer::TYPE,
            "Triggers on device movement. The accelerometer measures the acceleration (including the force of gravity) applied to the device measured in meters/second/second: ACCELERATION = LINEAR ACCELERATION + GRAVITY");

    const StatementInfo TriggerAccelerometer::getInfo()
    {
        return TriggerAccelerometer::info;
    }

    TriggerAccelerometer::TriggerAccelerometer(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerAccelerometer::getType() const
    {
        return TriggerAccelerometer::TYPE;
    }

    TriggerAccelerometer::~TriggerAccelerometer()
    {
    }

    bool TriggerAccelerometer::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerAccelerometer::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("ACCEL_X", "Acceleration x-axis",
                        "Acceleration on x-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ACCEL_Y", "Acceleration y-axis",
                        "Acceleration on y-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ACCEL_Z", "Acceleration z-axis",
                        "Acceleration on z-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ACCEL_MAGNITUDE", "Acceleration magnitude",
                        "Acceleration vector magnitude in m/s/s", "NUMBER"));
        return definition;
    }

    bool TriggerAccelerometer::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ACCELEROMETER, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        float magnitude = sqrt(
                event->motion.dsp.x * event->motion.dsp.x
                        + event->motion.dsp.y * event->motion.dsp.y
                        + event->motion.dsp.z * event->motion.dsp.z);
        RUNLOG->debug(
                SSTR(
                        "X=" << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                << event->motion.dsp.z << " Magnitude=" << magnitude));
        values->push_back(/*ACCEL_X*/SSTR(event->motion.dsp.x));
        values->push_back(/*ACCEL_Y*/SSTR(event->motion.dsp.y));
        values->push_back(/*ACCEL_Z*/SSTR(event->motion.dsp.z));
        values->push_back(/*ACCEL_MAGNITUDE*/SSTR(magnitude));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerAccelerometer::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ACCELEROMETER, state,
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
            float magnitude = sqrt(
                    event->motion.dsp.x * event->motion.dsp.x
                            + event->motion.dsp.y * event->motion.dsp.y
                            + event->motion.dsp.z * event->motion.dsp.z);
            RUNLOG->debug(
                    SSTR(
                            "X=" << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                    << event->motion.dsp.z << " Magnitude=" << magnitude));
            values->push_back(/*ACCEL_X*/SSTR(event->motion.dsp.x));
            values->push_back(/*ACCEL_Y*/SSTR(event->motion.dsp.y));
            values->push_back(/*ACCEL_Z*/SSTR(event->motion.dsp.z));
            values->push_back(/*ACCEL_MAGNITUDE*/SSTR(magnitude));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
