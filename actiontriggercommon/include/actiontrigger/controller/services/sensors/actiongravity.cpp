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

#include "actiontrigger/controller/services/sensors/actiongravity.hpp"

namespace actiontrigger
{

    std::string ActionGravity::TYPE = "ACTION_GRAVITY";
    std::vector<ParameterDefinition*>* ActionGravity::parameterDefinitions = NULL;
    Logger* ActionGravity::StatementLOG = LoggerFactory::getInstance("ActionGravity");
    const StatementInfo ActionGravity::info(false, "Gravity", ActionGravity::TYPE,
            "Detect gravity force. Sets x, y and z axis for gravity measured in meters/second/second. Gravity is calculated by subtracting the device's linear acceleration from its acceleration: GRAVITY = ACCELERATION - LINEAR ACCELERATION.");

    bool ActionGravity::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_GRAVITY);
    }

    const StatementInfo ActionGravity::getInfo()
    {
        return ActionGravity::info;
    }

    ActionGravity::ActionGravity(StatementModel* model) :
            Action(model)
    {
    }

    ActionGravity::~ActionGravity()
    {
    }

    std::string ActionGravity::getType() const
    {
        return ActionGravity::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGravity::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("GRAV_X", "Gravity on x-axis in m/s/s",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GRAV_Y", "Gravity on y-axis in m/s/s",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GRAV_Z", "Gravity on z-axis in m/s/s",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GRAV_MAGNITUDE",
                        "Gravity vector magnitude in m/s/s", "NUMBER"));
        return vars;
    }

    void ActionGravity::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_GRAVITY, state);
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
        state->setUserProperty("GRAV_X", SSTR(event->motion.dsp.x));
        state->setUserProperty("GRAV_Y", SSTR(event->motion.dsp.y));
        state->setUserProperty("GRAV_Z", SSTR(event->motion.dsp.z));
        state->setUserProperty("GRAV_MAGNITUDE", SSTR(magnitude));
        delete event;
    }

} /* namespace actiontrigger */
