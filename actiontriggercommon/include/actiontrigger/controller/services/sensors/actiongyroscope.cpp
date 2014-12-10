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

#include "actiontrigger/controller/services/sensors/actiongyroscope.hpp"

namespace actiontrigger
{

    std::string ActionGyroscope::TYPE = "ACTION_GYROSCOPE";
    std::vector<ParameterDefinition*>* ActionGyroscope::parameterDefinitions = NULL;
    Logger* ActionGyroscope::StatementLOG = LoggerFactory::getInstance("ActionGyroscope");
    const StatementInfo ActionGyroscope::info(false, "Gyroscope", ActionGyroscope::TYPE,
            "Detect magnetic field from Gyroscope sensor. Sets x, y and z axis magnetic field strength in uT (micro Tesla).");

    bool ActionGyroscope::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_GYROSCOPE);
    }

    const StatementInfo ActionGyroscope::getInfo()
    {
        return ActionGyroscope::info;
    }

    ActionGyroscope::ActionGyroscope(StatementModel* model) :
            Action(model)
    {
    }

    ActionGyroscope::~ActionGyroscope()
    {
    }

    std::string ActionGyroscope::getType() const
    {
        return ActionGyroscope::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGyroscope::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("GYRO_X",
                        "Gyroscope reading on x-axis in r/s (radians/second)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GYRO_Y",
                        "Gyroscope reading on y-axis in r/s (radians/second)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GYRO_Z",
                        "Gyroscope reading on z-axis in r/s (radians/second)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GYRO__MAGNITUDE",
                        "Gyroscope magnitude reading in r/s (radians/second)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GYRO_TEMP",
                        "Temperature reading of Gyroscope sensor in degrees Celsius", "NUMBER"));
        return vars;
    }

    void ActionGyroscope::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_GYROSCOPE, state);
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
                        "Retrieved: TEMP=" << event->motion.gyro.temperature << " X="
                                << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                << event->motion.dsp.z));
        state->setUserProperty("GYRO_X", SSTR(event->motion.dsp.x));
        state->setUserProperty("GYRO_Y", SSTR(event->motion.dsp.y));
        state->setUserProperty("GYRO_Z", SSTR(event->motion.dsp.z));
        state->setUserProperty("GYRO_MAGNITUDE", SSTR(magnitude));
        state->setUserProperty("GYRO_TEMP", SSTR(event->motion.gyro.temperature));
        delete event;
    }

} /* namespace actiontrigger */
