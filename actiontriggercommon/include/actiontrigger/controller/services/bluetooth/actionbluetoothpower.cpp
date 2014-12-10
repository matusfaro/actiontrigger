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

#include "actiontrigger/controller/services/bluetooth/actionbluetoothpower.hpp"

namespace actiontrigger
{

    std::string ActionBluetoothPower::TYPE = "ACTION_BLUETOOTH_POWER";

    std::vector<ParameterDefinition*>* ActionBluetoothPower::parameterDefinitions = NULL;
    Logger* ActionBluetoothPower::StatementLOG = LoggerFactory::getInstance("ActionBluetoothPower");
    const StatementInfo ActionBluetoothPower::info(false, "Bluetooth Power",
            ActionBluetoothPower::TYPE, "Set the power state of Bluetooth");

    const StatementInfo ActionBluetoothPower::getInfo()
    {
        return ActionBluetoothPower::info;
    }

    ActionBluetoothPower::ActionBluetoothPower(StatementModel* model) :
            Action(model)
    {
    }

    ActionBluetoothPower::~ActionBluetoothPower()
    {
    }

    std::vector<ParameterDefinition*>* ActionBluetoothPower::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Attempt to turn Bluetooth on or off", "Desired state");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionBluetoothPower::getDefaultParameter(std::string key)
    {
        if (key == "STATE") {
            return "on";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionBluetoothPower::getType() const
    {
        return ActionBluetoothPower::TYPE;
    }

    void ActionBluetoothPower::execute(ExecutionState* state)
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
                    RUNLOG->error(SSTR("Bluetooth failed to initialize: " << strerror(initStatus)));
                    StatementLOG->debug(SSTR("Wifi status: " << strerror(initStatus)));
                    break;
            }
            return;
        }
        errno = EOK;
        int changeStatus = bt_ldev_set_power(isValueTrue(getParameter("STATE", state)));
        if (changeStatus != 0) {
            switch (changeStatus) {
                case ENOMEM:
                    RUNLOG->error("Bluetooth failed to initialize due to insufficient memory");
                    break;
                case EPERM:
                    RUNLOG->error(
                            "Bluetooth failed to initialize due to error accessing required system data. (internal device error)");
                    break;
                case ENOENT:
                    RUNLOG->error(
                            "Bluetooth failed to initialize due to required data entry was not found. (internal device error)");
                    break;
                default:
                    RUNLOG->error(
                            SSTR("Bluetooth failed to initialize: " << strerror(changeStatus)));
                    StatementLOG->debug(SSTR("Wifi status: " << strerror(changeStatus)));
                    break;
            }
        }
    }

} /* namespace actiontrigger */
