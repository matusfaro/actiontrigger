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

#include "actiontrigger/controller/services/sensors/actionlight.hpp"

namespace actiontrigger
{

    std::string ActionLight::TYPE = "ACTION_LIGHT";
    std::vector<ParameterDefinition*>* ActionLight::parameterDefinitions = NULL;
    Logger* ActionLight::StatementLOG = LoggerFactory::getInstance("ActionLight");
    const StatementInfo ActionLight::info(false, "Light", ActionLight::TYPE,
            "Retrieves illuminance measured in one lumen per square metre. (lux)");

    bool ActionLight::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_LIGHT);
    }

    const StatementInfo ActionLight::getInfo()
    {
        return ActionLight::info;
    }

    ActionLight::ActionLight(StatementModel* model) :
            Action(model)
    {
    }

    ActionLight::~ActionLight()
    {
    }

    std::string ActionLight::getType() const
    {
        return ActionLight::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionLight::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("ILLUMINANCE",
                        "Illuminance in one lumen per square metre. (lux)", "NUMBER"));
        return vars;
    }

    void ActionLight::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_LIGHT, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(SSTR("Retrieved: ILLUM=" << event->light_s.illuminance));
        state->setUserProperty("ILLUMINANCE", SSTR(event->light_s.illuminance));
        delete event;
    }

} /* namespace actiontrigger */
