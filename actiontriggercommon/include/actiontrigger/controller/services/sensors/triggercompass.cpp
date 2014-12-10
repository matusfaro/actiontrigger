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

#include "actiontrigger/controller/services/sensors/triggercompass.hpp"

namespace actiontrigger
{

    std::string TriggerCompass::TYPE = "TRIGGER_COMPASS";
    Logger* TriggerCompass::StatementLOG = LoggerFactory::getInstance("TriggerCompass");
    const StatementInfo TriggerCompass::info(true, "Compass", TriggerCompass::TYPE,
            "Trigger on azimuth, pitch or device's face up-down position. Azimuth is the heading of the device, which is the angle between the device's y-axis and magnetic north measured in degrees from 0 to 359. If device is face down TRUE is returned, otherwise FALSE.");

    bool TriggerCompass::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_COMPASS);
    }

    const StatementInfo TriggerCompass::getInfo()
    {
        return TriggerCompass::info;
    }

    TriggerCompass::TriggerCompass(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerCompass::getType() const
    {
        return TriggerCompass::TYPE;
    }

    TriggerCompass::~TriggerCompass()
    {
    }

    bool TriggerCompass::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerCompass::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("AZIMUTH", "Azimuth",
                        "Azimuth: angle between the device's y-axis and magnetic north", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("FACE_DOWN", "Face Down",
                        "Is the device face down", "BOOL"));
        return definition;
    }

    bool TriggerCompass::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_COMPASS, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: Azimuth=" << event->compass_s.azimuth << " FaceDown="
                                << (event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE")));
        values->push_back(/*AZIMUTH*/SSTR(event->compass_s.azimuth));
        values->push_back(/*FACE_DOWN*/event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE");
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerCompass::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_COMPASS, state,
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
                            "Retrieved: Azimuth=" << event->compass_s.azimuth << " FaceDown="
                                    << (event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE")));
            values->push_back(/*AZIMUTH*/SSTR(event->compass_s.azimuth));
            values->push_back(/*FACE_DOWN*/event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE");
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
