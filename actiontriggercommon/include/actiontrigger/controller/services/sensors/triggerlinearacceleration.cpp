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

#include "actiontrigger/controller/services/sensors/triggerlinearacceleration.hpp"

namespace actiontrigger
{

    std::string TriggerLinearAcceleration::TYPE = "TRIGGER_LINEAR_ACCELERATION";
    Logger* TriggerLinearAcceleration::StatementLOG = LoggerFactory::getInstance(
            "TriggerLinearAcceleration");
    const StatementInfo TriggerLinearAcceleration::info(true, "Linear Acceleration",
            TriggerLinearAcceleration::TYPE,
            "Triggers on device movement excluding gravity. Linear acceleration is the acceleration of the device minus the acceleration due to gravity measured in meters/second/second: LINEAR ACCELERATION = ACCELERATION - GRAVITY.");

    bool TriggerLinearAcceleration::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_LINEAR_ACCEL);
    }

    const StatementInfo TriggerLinearAcceleration::getInfo()
    {
        return TriggerLinearAcceleration::info;
    }

    TriggerLinearAcceleration::TriggerLinearAcceleration(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerLinearAcceleration::getType() const
    {
        return TriggerLinearAcceleration::TYPE;
    }

    TriggerLinearAcceleration::~TriggerLinearAcceleration()
    {
    }

    bool TriggerLinearAcceleration::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerLinearAcceleration::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("LIN_ACC_X", "Linear Acceleration x-axis",
                        "Linear Acceleration on x-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("LIN_ACC_Y", "Linear Acceleration y-axis",
                        "Linear Acceleration on y-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("LIN_ACC_Z", "Linear Acceleration z-axis",
                        "Linear Acceleration on z-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("LIN_ACC_MAGNITUDE",
                        "Linear Acceleration magnitude",
                        "Linear Acceleration vector magnitude in m/s/s", "NUMBER"));
        return definition;
    }

    bool TriggerLinearAcceleration::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LINEAR_ACCEL, state);
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
        values->push_back(/*LIN_ACC_X*/SSTR(event->motion.dsp.x));
        values->push_back(/*LIN_ACC_Y*/SSTR(event->motion.dsp.y));
        values->push_back(/*LIN_ACC_Z*/SSTR(event->motion.dsp.z));
        values->push_back(/*LIN_ACC_MAGNITUDE*/SSTR(magnitude));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerLinearAcceleration::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LINEAR_ACCEL, state,
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
            values->push_back(/*LIN_ACC_X*/SSTR(event->motion.dsp.x));
            values->push_back(/*LIN_ACC_Y*/SSTR(event->motion.dsp.y));
            values->push_back(/*LIN_ACC_Z*/SSTR(event->motion.dsp.z));
            values->push_back(/*LIN_ACC_MAGNITUDE*/SSTR(magnitude));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
