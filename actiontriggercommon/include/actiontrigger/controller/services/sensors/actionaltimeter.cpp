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

#include "actiontrigger/controller/services/sensors/actionaltimeter.hpp"

namespace actiontrigger
{

    std::string ActionAltimeter::TYPE = "ACTION_ALTIMETER";
    std::vector<ParameterDefinition*>* ActionAltimeter::parameterDefinitions = NULL;
    Logger* ActionAltimeter::StatementLOG = LoggerFactory::getInstance("ActionAltimeter");
    const StatementInfo ActionAltimeter::info(false, "Altimeter", ActionAltimeter::TYPE,
            RichText::html(
                    "Retrieve altitude reported in meters relative to mean sea level. On BlackBerry, the altimeter uses a combination of pressure and location to determine the altitude, as using pressure alone would yield to inaccurate results due to changes in air pressure caused by the weather. The location information is used to compensate for the weather. This requires that the user has enabled location services in the global settings."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool ActionAltimeter::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ALTIMETER);
    }

    const StatementInfo ActionAltimeter::getInfo()
    {
        return ActionAltimeter::info;
    }

    ActionAltimeter::ActionAltimeter(StatementModel* model) :
            Action(model)
    {
    }

    ActionAltimeter::~ActionAltimeter()
    {
    }

    std::string ActionAltimeter::getType() const
    {
        return ActionAltimeter::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionAltimeter::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("ALTITUDE",
                        "The altitude in meters relative to mean sea level", "NUMBER"));
        return vars;
    }

    void ActionAltimeter::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ALTIMETER, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(SSTR("Retrieved: ALT=" << event->altitude_s.altitude));
        state->setUserProperty("ALTITUDE", SSTR(event->altitude_s.altitude));
        delete event;
    }

} /* namespace actiontrigger */
