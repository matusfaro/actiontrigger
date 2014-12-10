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

#include "actiontrigger/controller/services/sensors/actiontemperature.hpp"

namespace actiontrigger
{

    std::string ActionTemperature::TYPE = "ACTION_TEMPERATURE";
    std::vector<ParameterDefinition*>* ActionTemperature::parameterDefinitions = NULL;
    Logger* ActionTemperature::StatementLOG = LoggerFactory::getInstance("ActionTemperature");
    const StatementInfo ActionTemperature::info(false, "Temperature", ActionTemperature::TYPE,
            RichText::html(
                    "Retrieves device's ambient temperature measured in degrees Celsius."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: This sensor may not be present on your device.")));

    bool ActionTemperature::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_TEMPERATURE);
    }

    const StatementInfo ActionTemperature::getInfo()
    {
        return ActionTemperature::info;
    }

    ActionTemperature::ActionTemperature(StatementModel* model) :
            Action(model)
    {
    }

    ActionTemperature::~ActionTemperature()
    {
    }

    std::string ActionTemperature::getType() const
    {
        return ActionTemperature::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionTemperature::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("TEMPERATURE",
                        "The ambient temperature measured in degrees Celsius.", "NUMBER"));
        return vars;
    }

    void ActionTemperature::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_TEMPERATURE, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(SSTR("Retrieved: TEMP=" << event->temperature_s.temperature));
        state->setUserProperty("TEMPERATURE", SSTR(event->temperature_s.temperature));
        delete event;
    }

} /* namespace actiontrigger */
