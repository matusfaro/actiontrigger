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

#include "actiontrigger/controller/services/sensors/triggerorientation.hpp"

namespace actiontrigger
{

    std::string TriggerOrientation::TYPE = "TRIGGER_ORIENTATION";
    Logger* TriggerOrientation::StatementLOG = LoggerFactory::getInstance("TriggerOrientation");
    const StatementInfo TriggerOrientation::info(true, "Orientation", TriggerOrientation::TYPE,
            "Trigger on screen orientation. Valid values are 0, 90, 180 and 270 degrees.");

    bool TriggerOrientation::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ORIENTATION);
    }

    const StatementInfo TriggerOrientation::getInfo()
    {
        return TriggerOrientation::info;
    }

    TriggerOrientation::TriggerOrientation(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerOrientation::getType() const
    {
        return TriggerOrientation::TYPE;
    }

    TriggerOrientation::~TriggerOrientation()
    {
    }

    bool TriggerOrientation::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerOrientation::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("ILLUMINANCE", "Illuminance",
                        "Illuminance in one lumen per square metre. (lux)", "NUMBER"));
        return definition;
    }

    bool TriggerOrientation::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ORIENTATION, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(SSTR("Retrieved: ORIENT=" << event->orientation.screen));
        values->push_back(/*SCREEN_ORIENT*/SSTR(event->orientation.screen));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerOrientation::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ORIENTATION, state,
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
            RUNLOG->debug(SSTR("Retrieved: ORIENT=" << event->orientation.screen));
            values->push_back(/*SCREEN_ORIENT*/SSTR(event->orientation.screen));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
