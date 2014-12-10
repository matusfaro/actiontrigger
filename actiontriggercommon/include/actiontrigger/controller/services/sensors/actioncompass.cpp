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

#include "actiontrigger/controller/services/sensors/actioncompass.hpp"

namespace actiontrigger
{

    std::string ActionCompass::TYPE = "ACTION_COMPASS";
    std::vector<ParameterDefinition*>* ActionCompass::parameterDefinitions = NULL;
    Logger* ActionCompass::StatementLOG = LoggerFactory::getInstance("ActionCompass");
    const StatementInfo ActionCompass::info(false, "Compass", ActionCompass::TYPE,
            "Detects azimuth and device's face up-down position. Azimuth is the heading of the device, which is the angle between the device's y-axis and magnetic north measured in degrees from 0 to 359. If device is face down TRUE is returned, otherwise FALSE.");

    bool ActionCompass::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_COMPASS);
    }

    const StatementInfo ActionCompass::getInfo()
    {
        return ActionCompass::info;
    }

    ActionCompass::ActionCompass(StatementModel* model) :
            Action(model)
    {
    }

    ActionCompass::~ActionCompass()
    {
    }

    std::string ActionCompass::getType() const
    {
        return ActionCompass::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionCompass::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("AZIMUTH",
                        "Azimuth: angle between the device's y-axis and magnetic north", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("FACE_DOWN", "Device face down", "BOOL"));
        return vars;
    }

    void ActionCompass::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_COMPASS, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: Azimuth=" << event->compass_s.azimuth << " FaceDown="
                                << (event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE")));
        state->setUserProperty("AZIMUTH", SSTR(event->compass_s.azimuth));
        state->setUserProperty("FACE_DOWN", event->compass_s.is_face_down == 1 ? "TRUE" : "FALSE");
        delete event;
    }

} /* namespace actiontrigger */
