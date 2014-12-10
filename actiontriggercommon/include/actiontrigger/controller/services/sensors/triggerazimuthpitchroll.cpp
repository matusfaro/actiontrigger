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

#include "actiontrigger/controller/services/sensors/triggerazimuthpitchroll.hpp"

namespace actiontrigger
{

    std::string TriggerAzimuthPitchRoll::TYPE = "TRIGGER_APR";
    Logger* TriggerAzimuthPitchRoll::StatementLOG = LoggerFactory::getInstance(
            "TriggerAzimuthPitchRoll");
    const StatementInfo TriggerAzimuthPitchRoll::info(true, "Azimuth Pitch Roll",
            TriggerAzimuthPitchRoll::TYPE,
            "Triggers on azimuth, pitch or roll. Azimuth is the heading of the device, which is the angle between the device's y-axis and magnetic north measured in degrees from 0 to 359. Pitch is the rotation around the device's x-axis measured in degrees from -180 to 180. Roll is the rotation around the device's y-axis measured in degrees from -90 to 90.");

    bool TriggerAzimuthPitchRoll::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_AZIMUTH_PITCH_ROLL);
    }

    const StatementInfo TriggerAzimuthPitchRoll::getInfo()
    {
        return TriggerAzimuthPitchRoll::info;
    }

    TriggerAzimuthPitchRoll::TriggerAzimuthPitchRoll(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerAzimuthPitchRoll::getType() const
    {
        return TriggerAzimuthPitchRoll::TYPE;
    }

    TriggerAzimuthPitchRoll::~TriggerAzimuthPitchRoll()
    {
    }

    bool TriggerAzimuthPitchRoll::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerAzimuthPitchRoll::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("AZIMUTH", "Azimuth",
                        "Azimuth: angle between the device's y-axis and magnetic north", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("PITCH", "Pitch",
                        "Pitch: rotation around the device's x-axis", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROLL", "Roll",
                        "Roll: rotation around the device's y-axis", "NUMBER"));
        return definition;
    }

    bool TriggerAzimuthPitchRoll::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: Azimuth=" << event->apr.azimuth << " Pitch=" << event->apr.pitch
                                << " Roll=" << event->apr.roll));
        values->push_back(/*AZIMUTH*/SSTR(event->apr.azimuth));
        values->push_back(/*PITCH*/SSTR(event->apr.pitch));
        values->push_back(/*ROLL*/SSTR(event->apr.roll));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerAzimuthPitchRoll::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, state,
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
            RUNLOG->debug(
                    SSTR(
                            "Retrieved: Azimuth=" << event->apr.azimuth << " Pitch="
                                    << event->apr.pitch << " Roll=" << event->apr.roll));
            values->push_back(/*AZIMUTH*/SSTR(event->apr.azimuth));
            values->push_back(/*PITCH*/SSTR(event->apr.pitch));
            values->push_back(/*ROLL*/SSTR(event->apr.roll));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
