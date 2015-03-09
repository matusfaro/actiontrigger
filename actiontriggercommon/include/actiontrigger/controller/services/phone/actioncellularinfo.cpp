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

#include "actiontrigger/controller/services/phone/actioncellularinfo.hpp"

namespace actiontrigger
{

    std::string ActionCellularInfo::TYPE = "ACTION_ActionCellularInfo";
    std::vector<ParameterDefinition*>* ActionCellularInfo::parameterDefinitions = NULL;
    Logger* ActionCellularInfo::StatementLOG = LoggerFactory::getInstance("ActionCellularInfo");
    const StatementInfo ActionCellularInfo::info(false, "Cellular Info", ActionCellularInfo::TYPE,
            "Cellular network information including signal strength, network id, cell id, roaming.");

    const StatementInfo ActionCellularInfo::getInfo()
    {
        return ActionCellularInfo::info;
    }

    ActionCellularInfo::ActionCellularInfo(StatementModel* model) :
            Action(model)
    {
    }

    ActionCellularInfo::~ActionCellularInfo()
    {
    }

    std::string ActionCellularInfo::getType() const
    {
        return ActionCellularInfo::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionCellularInfo::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("NETWORK_DISPLAY_NAME",
                        "Returns the network name, cross referenced with other information, as reported by Settings and QuickSettings, that provides a user friendly name, or an empty string if no network is currently connected or the network does not provide a name.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("BASE_STATION_ID",
                        "Unique identifier for the base station serving the CDMA network.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("CELL_ID",
                        "Unique identifier for the base station serving the GSM/UMTS/LTE network.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("LOCATION_AREA_CODE",
                        "Identifier for the geographic region which contains the base station serving the GSM/UMTS network.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MOBILE_COUNTRY_CODE",
                        "Unique identifier for the country where the network is located.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MOBILE_NETWORK_CODE",
                        "Unique identifier for the network within its country.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("NETWORK_ID",
                        "Identifier for a geographic region which contains the base station serving the CDMA network.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("IS_ROAMING",
                        "Flag indicating whether the device is beyond coverage of its home network.",
                        "BOOL"));
        vars->push_back(
                new statement_output_variable_definition("SIGNAL_STRENGTH",
                        "Power measurement of the network's radio signal.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("SYSTEM_ID",
                        "Unique identifier for a geographic region which contains the base station serving the CDMA network.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("TRACKING_AREA_CODE",
                        "Unique identifier for the geographic region which contains the base station serving the LTE network.",
                        "NUMBER"));
        return vars;
    }

    void ActionCellularInfo::execute(ExecutionState* state)
    {
        bb::device::CellularNetworkInfo info;
        state->setUserProperty("NETWORK_DISPLAY_NAME", SSTR(info.displayName().toStdString()));
        state->setUserProperty("BASE_STATION_ID", SSTR(info.baseStationId()));
        state->setUserProperty("CELL_ID", SSTR(info.cellId()));
        state->setUserProperty("LOCATION_AREA_CODE", SSTR(info.locationAreaCode()));
        state->setUserProperty("MOBILE_COUNTRY_CODE", SSTR(info.mobileCountryCode().toStdString()));
        state->setUserProperty("MOBILE_NETWORK_CODE", SSTR(info.mobileNetworkCode().toStdString()));
        state->setUserProperty("NETWORK_I", SSTR(info.networkId()));
        state->setUserProperty("IS_ROAMING", SSTR(info.isRoaming()));
        state->setUserProperty("SIGNAL_STRENGTH", SSTR(info.signalStrength()));
        state->setUserProperty("SYSTEM_ID", SSTR(info.systemId()));
        state->setUserProperty("TRACKING_AREA_CODE", SSTR(info.trackingAreaCode()));
    }

} /* namespace actiontrigger */
