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

#include "actiontrigger/controller/services/wifi/triggerwifistatus.hpp"

namespace actiontrigger
{

    std::string TriggerWifiStatus::TYPE = "TRIGGER_WIFI_STATUS";

    std::vector<ParameterDefinition*>* TriggerWifiStatus::parameterDefinitions = NULL;
    Logger* TriggerWifiStatus::StatementLOG = LoggerFactory::getInstance("TriggerWifiStatus");
    const StatementInfo TriggerWifiStatus::info(true, "Wifi Status", TriggerWifiStatus::TYPE,
            "Triggers on current Wifi status");

    const StatementInfo TriggerWifiStatus::getInfo()
    {
        return TriggerWifiStatus::info;
    }

    TriggerWifiStatus::TriggerWifiStatus(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerWifiStatus::getDefaultParameter(std::string key)
    {
        if (key == "STATE") {
            return "on";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string TriggerWifiStatus::getType() const
    {
        return TriggerWifiStatus::TYPE;
    }

    TriggerWifiStatus::~TriggerWifiStatus()
    {
    }

    std::vector<ParameterDefinition*>* TriggerWifiStatus::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("STATE", "State",
                    "Trigger when Wifi status is 'on' or 'off'", "Wait until");
            state->dropdownAddOption("On", "on");
            state->dropdownAddOption("Off", "off");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::vector<statement_output_variable_definition*>* TriggerWifiStatus::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("WIFI_STATUS",
                        "Wifi status. Either TRUE, FALSE, or ERROR.", "BOOL"));
        return vars;
    }

    bool TriggerWifiStatus::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        wifi_status_t status;
        std::string desiredState = getParameter("STATE", state);
        int result;
        if (WIFI_SUCCESS != (result = wifi_get_status(&status))) {
            RUNLOG->error(
                    SSTR(
                            "Status is "
                                    << (status ? "WIFI_STATUS_RADIO_ON" : "WIFI_STATUS_RADIO_OFF")));
            StatementLOG->debug(
                    SSTR(
                            "wifi_get_status returned unsuccesful " << " return:" << result << " errno: " << strerror(errno)));
        }
        std::string statusString;
        switch (status) {
            case WIFI_STATUS_RADIO_ON:
                statusString = "TRUE";
                break;
            case WIFI_STATUS_RADIO_OFF:
                statusString = "FALSE";
                break;
            case WIFI_STATUS_BUSY:
                statusString = "ERROR";
                break;
        }
        state->setUserProperty("WIFI_STATUS", statusString);
        RUNLOG->debug("Wifi power status: " + statusString);
        StatementLOG->debug("Wifi power status: " + statusString);
        return isValueTrue(desiredState) == (status == WIFI_STATUS_RADIO_ON);
    }

    bool TriggerWifiStatus::isEventOnlyTrigger()
    {
        return false;
    }

} /* namespace actiontrigger */
