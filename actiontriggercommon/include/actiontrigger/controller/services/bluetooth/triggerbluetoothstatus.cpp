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

#include "actiontrigger/controller/services/bluetooth/triggerbluetoothstatus.hpp"

namespace actiontrigger
{

    std::string TriggerBluetoothStatus::TYPE = "TRIGGER_BLUETOOTH_STATUS";

    std::vector<ParameterDefinition*>* TriggerBluetoothStatus::parameterDefinitions = NULL;
    Logger* TriggerBluetoothStatus::StatementLOG = LoggerFactory::getInstance(
            "TriggerBluetoothStatus");
    const StatementInfo TriggerBluetoothStatus::info(true, "Bluetooth Status",
            TriggerBluetoothStatus::TYPE, "Check current Bluetooth status");

    const StatementInfo TriggerBluetoothStatus::getInfo()
    {
        return TriggerBluetoothStatus::info;
    }

    TriggerBluetoothStatus::TriggerBluetoothStatus(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerBluetoothStatus::getType() const
    {
        return TriggerBluetoothStatus::TYPE;
    }

    TriggerBluetoothStatus::~TriggerBluetoothStatus()
    {
    }

    std::vector<ParameterDefinition*>* TriggerBluetoothStatus::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Trigger when Bluetooth status is 'on' or 'off'", "Wait until");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
//        parameterDefinitions->push_back(
//                ParameterDefinition::getSlider("GRANULARITY", "Granularity", "Amount of time (milliseconds) to wait between checking again", "0",
//                        "10000", "100"));
        }
        return parameterDefinitions;
    }

    std::string TriggerBluetoothStatus::getDefaultParameter(std::string key)
    {
        if (key == "STATE") {
            return "on";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::vector<statement_output_variable_definition*>* TriggerBluetoothStatus::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("BLUETOOTH_STATUS",
                        "Bluetooth status. Either TRUE or FALSE.", "BOOL"));
        return vars;
    }

    bool TriggerBluetoothStatus::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        int initStatus = state->getGlobalResources()->setupBluetooth();
        if (initStatus != 0) {
            switch (initStatus) {
                case ENOMEM:
                    RUNLOG->error("Bluetooth failed to initialize due to insufficient memory");
                    break;
                default:
                    RUNLOG->debug(SSTR("Bluetooth failed to initialize: " << strerror(initStatus)));
                    StatementLOG->debug(SSTR("Wifi status: " << strerror(initStatus)));
                    break;
            }
            return false;
        }
        return bt_ldev_get_power() == isValueTrue(getParameter("STATE", state));
    }

    bool TriggerBluetoothStatus::isEventOnlyTrigger()
    {
        return false;
    }

} /* namespace actiontrigger */
