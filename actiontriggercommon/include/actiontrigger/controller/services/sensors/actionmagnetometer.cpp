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

#include "actiontrigger/controller/services/sensors/actionmagnetometer.hpp"

namespace actiontrigger
{

    std::string ActionMagnetometer::TYPE = "ACTION_MAGNETOMETER";
    std::vector<ParameterDefinition*>* ActionMagnetometer::parameterDefinitions = NULL;
    Logger* ActionMagnetometer::StatementLOG = LoggerFactory::getInstance("ActionMagnetometer");
    const StatementInfo ActionMagnetometer::info(false, "Magnetometer", ActionMagnetometer::TYPE,
            "Retrieves detected magnetic field. Sets x, y and z axis magnetic field strength in uT (micro Tesla). Can be used as metal detector.");

    bool ActionMagnetometer::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_MAGNETOMETER);
    }

    const StatementInfo ActionMagnetometer::getInfo()
    {
        return ActionMagnetometer::info;
    }

    ActionMagnetometer::ActionMagnetometer(StatementModel* model) :
            Action(model)
    {
    }

    ActionMagnetometer::~ActionMagnetometer()
    {
    }

    std::string ActionMagnetometer::getType() const
    {
        return ActionMagnetometer::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionMagnetometer::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("MAGNET_X",
                        "Magnetic field strength on x-axis in uT (micro Tesla)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MAGNET_Y",
                        "Magnetic field strength on y-axis in uT (micro Tesla)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MAGNET_Z",
                        "Magnetic field strength on z-axis in uT (micro Tesla)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MAGNET_MAGNITUDE",
                        "Magnetic field strength magnitude in uT (micro Tesla)", "NUMBER"));
        return vars;
    }

    void ActionMagnetometer::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_MAGNETOMETER, state);
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
        state->setUserProperty("MAGNET_X", SSTR(event->motion.dsp.x));
        state->setUserProperty("MAGNET_Y", SSTR(event->motion.dsp.y));
        state->setUserProperty("MAGNET_Z", SSTR(event->motion.dsp.z));
        state->setUserProperty("MAGNET_MAGNITUDE", SSTR(magnitude));
        delete event;
    }

} /* namespace actiontrigger */
