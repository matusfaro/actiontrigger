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

#include "actiontrigger/controller/services/sensors/actionorientation.hpp"

namespace actiontrigger
{

    std::string ActionOrientation::TYPE = "ACTION_ORIENTATION";
    std::vector<ParameterDefinition*>* ActionOrientation::parameterDefinitions = NULL;
    Logger* ActionOrientation::StatementLOG = LoggerFactory::getInstance("ActionOrientation");
    const StatementInfo ActionOrientation::info(false, "Orientation", ActionOrientation::TYPE,
            "Detect screen orientation. Valid values are 0, 90, 180 and 270 degrees.");

    bool ActionOrientation::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ORIENTATION);
    }

    const StatementInfo ActionOrientation::getInfo()
    {
        return ActionOrientation::info;
    }

    ActionOrientation::ActionOrientation(StatementModel* model) :
            Action(model)
    {
    }

    ActionOrientation::~ActionOrientation()
    {
    }

    std::string ActionOrientation::getType() const
    {
        return ActionOrientation::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionOrientation::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("SCREEN_ORIENT",
                        "Current screen orientation", "NUMBER"));
        return vars;
    }

    void ActionOrientation::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ORIENTATION, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(SSTR("Retrieved: ORIENT=" << event->orientation.screen));
        state->setUserProperty("SCREEN_ORIENT", SSTR(event->orientation.screen));
        delete event;
    }

} /* namespace actiontrigger */
