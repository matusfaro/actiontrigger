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

#include "actiontrigger/controller/services/sensors/actionproximity.hpp"

namespace actiontrigger
{

    std::string ActionProximity::TYPE = "ACTION_PROXIMITY";
    std::vector<ParameterDefinition*>* ActionProximity::parameterDefinitions = NULL;
    Logger* ActionProximity::StatementLOG = LoggerFactory::getInstance("ActionProximity");
    const StatementInfo ActionProximity::info(false, "Proximity", ActionProximity::TYPE,
            "Detect proximity sensor values. Able to detect an object short distance from the phone. Sensor is typically positioned near the LED on the device. Original use case is to lock the screen when the phone is held against an ear during a phone call.");

    bool ActionProximity::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_PROXIMITY);
    }

    const StatementInfo ActionProximity::getInfo()
    {
        return ActionProximity::info;
    }

    ActionProximity::ActionProximity(StatementModel* model) :
            Action(model)
    {
    }

    ActionProximity::~ActionProximity()
    {
    }

    std::string ActionProximity::getType() const
    {
        return ActionProximity::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionProximity::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("PROX_RANGE_MIN",
                        "Minimum sensor range in centimeters", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("PROX_RANGE_MAX",
                        "Maximum sensor range in centimeters", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("PROX_DISTANCE",
                        "Discrete steps of distance or actual value in centimeters. Only able to detect values in range of PROX_RANGE_MIN to PROX_RANGE_MAX.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("PROX_NORMALIZED",
                        "Range from 0.0 to 1.0 (close to far), normalized unit-less signal from raw sensor.",
                        "NUMBER"));
        return vars;
    }

    void ActionProximity::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_PROXIMITY, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        sensor_info_t* info = sensor->getSensorInfo();
        delete sensor;
        if (NULL == event || NULL == info)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: MIN=" << info->range_min << " MAX=" << info->range_max
                                << " DIST=" << event->proximity_s.distance << " NORM="
                                << event->proximity_s.normalized));
        state->setUserProperty("PROX_RANGE_MIN", SSTR(info->range_min));
        state->setUserProperty("PROX_RANGE_MAX", SSTR(info->range_max));
        state->setUserProperty("PROX_DISTANCE", SSTR(event->proximity_s.distance));
        state->setUserProperty("PROX_NORMALIZED", SSTR(event->proximity_s.normalized));
        delete info;
        delete event;
    }

} /* namespace actiontrigger */
