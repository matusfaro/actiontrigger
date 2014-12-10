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

#include "actiontrigger/controller/services/wifi/actionwifipower.hpp"

namespace actiontrigger
{

    std::string ActionWifiPower::TYPE = "ACTION_WIFI_POWER";

    std::vector<ParameterDefinition*>* ActionWifiPower::parameterDefinitions = NULL;
    Logger* ActionWifiPower::StatementLOG = LoggerFactory::getInstance("ActionWifiPower");
    const StatementInfo ActionWifiPower::info(false, "Wifi Power", ActionWifiPower::TYPE,
            RichText::html(
                    "Set the power state of Wifi." + RichText::newline()
                            + RichText::color("red",
                                    "Note: As per Blackberry API, turning Wifi OFF may be disallowed.")));

    const StatementInfo ActionWifiPower::getInfo()
    {
        return ActionWifiPower::info;
    }

    ActionWifiPower::ActionWifiPower(StatementModel* model) :
            Action(model)
    {
    }

    ActionWifiPower::~ActionWifiPower()
    {
    }

    std::vector<ParameterDefinition*>* ActionWifiPower::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Attempt to turn Wifi on or off", "Desired state");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionWifiPower::getDefaultParameter(std::string key)
    {
        if (key == "STATE") {
            return "on";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionWifiPower::getType() const
    {
        return ActionWifiPower::TYPE;
    }

    void ActionWifiPower::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        std::string paramState = getParameter("STATE", state);
        bool desiredState;
        if (paramState.compare("on") == 0) {
            desiredState = true;
        } else if (paramState.compare("off") == 0) {
            desiredState = false;
        } else {
            RUNLOG->error("Parameter STATE is incorrect");
            StatementLOG->debug("Parameter is incorrect");
            return;
        }
        int result;
        if (WIFI_SUCCESS == (result = wifi_set_sta_power(desiredState))) {
            RUNLOG->debug("Successfully set power");
            StatementLOG->debug("Wifi power successfully set");
        } else if (errno != EOK) {
            RUNLOG->error(SSTR("Received error: WIFI_FAILURE: " << strerror(errno)));
            StatementLOG->warning(
                    SSTR("wifi_set_sta_power returned unsuccesful, errno: " << strerror(errno)));
        } else {
            RUNLOG->error("Received unknown error");
            StatementLOG->warning(
                    SSTR("wifi_set_sta_power returned unsuccesful, errno: " << strerror(errno)));
        }
    }

} /* namespace actiontrigger */
