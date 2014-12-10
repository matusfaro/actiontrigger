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

#include "actiontrigger/controller/services/device/actionholster.hpp"

namespace actiontrigger
{

    std::string ActionHolster::TYPE = "ACTION_HOLSTER";
    std::vector<ParameterDefinition*>* ActionHolster::parameterDefinitions = NULL;
    Logger* ActionHolster::StatementLOG = LoggerFactory::getInstance("ActionHolster");
    const StatementInfo ActionHolster::info(false, "Holster", ActionHolster::TYPE,
            "Checks if device is holstered. Returns TRUE if holstered, otherwise FALSE.");

    bool ActionHolster::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_HOLSTER);
    }

    const StatementInfo ActionHolster::getInfo()
    {
        return ActionHolster::info;
    }

    ActionHolster::ActionHolster(StatementModel* model) :
            Action(model)
    {
    }

    ActionHolster::~ActionHolster()
    {
    }

    std::string ActionHolster::getType() const
    {
        return ActionHolster::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionHolster::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("HOLSTERED", "Device in a holster",
                        "BOOL"));
        return vars;
    }

    void ActionHolster::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_HOLSTER, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: HOLSTER="
                                << (event->holster_s.holstered == 1 ? "TRUE" : "FALSE")));
        state->setUserProperty("HOLSTER", event->holster_s.holstered == 1 ? "TRUE" : "FALSE");
    }

} /* namespace actiontrigger */
