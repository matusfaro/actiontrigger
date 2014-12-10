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

#include "actiontrigger/controller/services/sensors/actionaccelerometer.hpp"

namespace actiontrigger
{

    std::string ActionAccelerometer::TYPE = "ACTION_ACCELEROMETER";
    std::vector<ParameterDefinition*>* ActionAccelerometer::parameterDefinitions = NULL;
    Logger* ActionAccelerometer::StatementLOG = LoggerFactory::getInstance("ActionAccelerometer");
    const StatementInfo ActionAccelerometer::info(false, "Accelerometer", ActionAccelerometer::TYPE,
            "Detects device movement. The accelerometer measures the acceleration (including the force of gravity) applied to the device measured in meters/second/second: ACCELERATION = LINEAR ACCELERATION + GRAVITY.");

    bool ActionAccelerometer::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ACCELEROMETER);
    }

    const StatementInfo ActionAccelerometer::getInfo()
    {
        return ActionAccelerometer::info;
    }

    ActionAccelerometer::ActionAccelerometer(StatementModel* model) :
            Action(model)
    {
    }

    ActionAccelerometer::~ActionAccelerometer()
    {
    }

    std::string ActionAccelerometer::getType() const
    {
        return ActionAccelerometer::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionAccelerometer::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("ACCEL_X",
                        "Acceleration on x-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ACCEL_Y",
                        "Acceleration on y-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ACCEL_Z",
                        "Acceleration on z-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ACCEL_MAGNITUDE",
                        "Acceleration vector magnitude in m/s/s", "NUMBER"));
        return vars;
    }

    void ActionAccelerometer::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ACCELEROMETER, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        float magnitude = sqrt(
                event->motion.dsp.x * event->motion.dsp.x
                        + event->motion.dsp.y * event->motion.dsp.y
                        + event->motion.dsp.z * event->motion.dsp.z);
        RUNLOG->debug(
                SSTR(
                        "X=" << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                << event->motion.dsp.z << " Magnitude=" << magnitude));
        state->setUserProperty("ACCEL_X", SSTR(event->motion.dsp.x));
        state->setUserProperty("ACCEL_Y", SSTR(event->motion.dsp.y));
        state->setUserProperty("ACCEL_Z", SSTR(event->motion.dsp.z));
        state->setUserProperty("ACCEL_MAGNITUDE", SSTR(magnitude));
        delete event;
    }

} /* namespace actiontrigger */
