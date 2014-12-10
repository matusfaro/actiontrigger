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

#include "actiontrigger/controller/services/sensors/triggerlight.hpp"

namespace actiontrigger
{

    std::string TriggerLight::TYPE = "TRIGGER_LIGHT";
    Logger* TriggerLight::StatementLOG = LoggerFactory::getInstance("TriggerLight");
    const StatementInfo TriggerLight::info(true, "Light", TriggerLight::TYPE,
            "Triggers on illuminance measured in one lumen per square metre. (lux)");

    bool TriggerLight::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_LIGHT);
    }

    const StatementInfo TriggerLight::getInfo()
    {
        return TriggerLight::info;
    }

    TriggerLight::TriggerLight(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerLight::getType() const
    {
        return TriggerLight::TYPE;
    }

    TriggerLight::~TriggerLight()
    {
    }

    bool TriggerLight::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerLight::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("ILLUMINANCE", "Illuminance",
                        "Illuminance in one lumen per square metre. (lux)", "NUMBER"));
        return definition;
    }

    bool TriggerLight::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LIGHT, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(SSTR("Retrieved: ILLUM=" << event->light_s.illuminance));
        values->push_back(/*ILLUMINANCE*/SSTR(event->light_s.illuminance));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerLight::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LIGHT, state,
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
            RUNLOG->debug(SSTR("Retrieved: ILLUM=" << event->light_s.illuminance));
            values->push_back(/*ILLUMINANCE*/SSTR(event->light_s.illuminance));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
