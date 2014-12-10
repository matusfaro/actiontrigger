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

#include "actiontrigger/controller/services/sensors/actionlinearacceleration.hpp"

namespace actiontrigger
{

    std::string ActionLinearAcceleration::TYPE = "ACTION_LINEAR_ACCELERATION";
    std::vector<ParameterDefinition*>* ActionLinearAcceleration::parameterDefinitions = NULL;
    Logger* ActionLinearAcceleration::StatementLOG = LoggerFactory::getInstance(
            "ActionLinearAcceleration");
    const StatementInfo ActionLinearAcceleration::info(false, "Linear Acceleration",
            ActionLinearAcceleration::TYPE,
            "Detects device movement excluding gravity. Sets x, y and z axis for linear acceleration. Linear acceleration is the acceleration of the device minus the acceleration due to gravity measured in meters/second/second: LINEAR ACCELERATION = ACCELERATION - GRAVITY.");

    bool ActionLinearAcceleration::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_LINEAR_ACCEL);
    }

    const StatementInfo ActionLinearAcceleration::getInfo()
    {
        return ActionLinearAcceleration::info;
    }

    ActionLinearAcceleration::ActionLinearAcceleration(StatementModel* model) :
            Action(model)
    {
    }

    ActionLinearAcceleration::~ActionLinearAcceleration()
    {
    }

    std::string ActionLinearAcceleration::getType() const
    {
        return ActionLinearAcceleration::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionLinearAcceleration::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("LIN_ACC_X",
                        "Linear Acceleration on x-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("LIN_ACC_Y",
                        "Linear Acceleration on y-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("LIN_ACC_Z",
                        "Linear Acceleration on z-axis in m/s/s", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("LIN_ACC_MAGNITUDE",
                        "Linear Acceleration vector magnitude in m/s/s", "NUMBER"));
        return vars;
    }

    void ActionLinearAcceleration::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LINEAR_ACCEL, state);
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
        state->setUserProperty("LIN_ACC_X", SSTR(event->motion.dsp.x));
        state->setUserProperty("LIN_ACC_Y", SSTR(event->motion.dsp.y));
        state->setUserProperty("LIN_ACC_Z", SSTR(event->motion.dsp.z));
        state->setUserProperty("LIN_ACC_MAGNITUDE", SSTR(magnitude));
        delete event;
    }

} /* namespace actiontrigger */
