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

#include "actiontrigger/controller/services/sensors/actionpressure.hpp"

namespace actiontrigger
{

    std::string ActionPressure::TYPE = "ACTION_PRESSURE";
    std::vector<ParameterDefinition*>* ActionPressure::parameterDefinitions = NULL;
    Logger* ActionPressure::StatementLOG = LoggerFactory::getInstance("ActionPressure");
    const StatementInfo ActionPressure::info(false, "Pressure", ActionPressure::TYPE,
            RichText::html(
                    "Retrieves current pressure and temperature of sensor. Pressure is measured in Pascals and temperature in degrees Celsius."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool ActionPressure::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_PRESSURE);
    }

    const StatementInfo ActionPressure::getInfo()
    {
        return ActionPressure::info;
    }

    ActionPressure::ActionPressure(StatementModel* model) :
            Action(model)
    {
    }

    ActionPressure::~ActionPressure()
    {
    }

    std::string ActionPressure::getType() const
    {
        return ActionPressure::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionPressure::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("PRESSURE", "Current pressure in Pascal",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("PRESSURE_TEMP",
                        "Temperature reading of pressure sensor in degrees Celsius", "NUMBER"));
        return vars;
    }

    void ActionPressure::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PRESSURE, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: TEMP=" << event->pressure_s.temperature << " PRESSURE="
                                << event->pressure_s.pressure));
        state->setUserProperty("PRESSURE", SSTR(event->pressure_s.pressure));
        state->setUserProperty("PRESSURE_TEMP", SSTR(event->pressure_s.temperature));
        delete event;
    }

} /* namespace actiontrigger */
