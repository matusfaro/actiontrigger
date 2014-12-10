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

#include "actiontrigger/controller/services/sensors/actionazimuthpitchroll.hpp"

namespace actiontrigger
{

    std::string ActionAzimuthPitchRoll::TYPE = "ACTION_APR";
    std::vector<ParameterDefinition*>* ActionAzimuthPitchRoll::parameterDefinitions = NULL;
    Logger* ActionAzimuthPitchRoll::StatementLOG = LoggerFactory::getInstance(
            "ActionAzimuthPitchRoll");
    const StatementInfo ActionAzimuthPitchRoll::info(false, "Azimuth Pitch Roll",
            ActionAzimuthPitchRoll::TYPE,
            "Detects azimuth, pitch and roll. Azimuth is the heading of the device, which is the angle between the device's y-axis and magnetic north measured in degrees from 0 to 359. Pitch is the rotation around the device's x-axis measured in degrees from -180 to 180. Roll is the rotation around the device's y-axis measured in degrees from -90 to 90.");

    bool ActionAzimuthPitchRoll::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_AZIMUTH_PITCH_ROLL);
    }

    const StatementInfo ActionAzimuthPitchRoll::getInfo()
    {
        return ActionAzimuthPitchRoll::info;
    }

    ActionAzimuthPitchRoll::ActionAzimuthPitchRoll(StatementModel* model) :
            Action(model)
    {
    }

    ActionAzimuthPitchRoll::~ActionAzimuthPitchRoll()
    {
    }

    std::string ActionAzimuthPitchRoll::getType() const
    {
        return ActionAzimuthPitchRoll::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionAzimuthPitchRoll::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("AZIMUTH",
                        "Azimuth: angle between the device's y-axis and magnetic north", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("PITCH",
                        "Pitch: rotation around the device's x-axis", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROLL",
                        "Roll: rotation around the device's y-axis", "NUMBER"));
        return vars;
    }

    void ActionAzimuthPitchRoll::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: Azimuth=" << event->apr.azimuth << " Pitch=" << event->apr.pitch
                                << " Roll=" << event->apr.roll));
        state->setUserProperty("AZIMUTH", SSTR(event->apr.azimuth));
        state->setUserProperty("PITCH", SSTR(event->apr.pitch));
        state->setUserProperty("ROLL", SSTR(event->apr.roll));
        delete event;
    }

} /* namespace actiontrigger */
