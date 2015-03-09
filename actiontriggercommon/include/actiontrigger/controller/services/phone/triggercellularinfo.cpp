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

#include "actiontrigger/controller/services/phone/triggercellularinfo.hpp"

namespace actiontrigger
{

    std::string TriggerCellularInfo::TYPE = "TRIGGER_TriggerCellularInfo";
    Logger* TriggerCellularInfo::StatementLOG = LoggerFactory::getInstance("TriggerCellularInfo");
    const StatementInfo TriggerCellularInfo::info(true, "Cellular Info", TriggerCellularInfo::TYPE,
            "Trigger on cellular network information including signal strength, network id, cell id, roaming.");

    const StatementInfo TriggerCellularInfo::getInfo()
    {
        return TriggerCellularInfo::info;
    }

    TriggerCellularInfo::TriggerCellularInfo(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerCellularInfo::getType() const
    {
        return TriggerCellularInfo::TYPE;
    }

    TriggerCellularInfo::~TriggerCellularInfo()
    {
    }

    bool TriggerCellularInfo::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerCellularInfo::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("NETWORK_DISPLAY_NAME",
                        "Network Display Name",
                        "Returns the network name, cross referenced with other information, as reported by Settings and QuickSettings, that provides a user friendly name, or an empty string if no network is currently connected or the network does not provide a name.",
                        "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("BASE_STATION_ID", "Base Station ID",
                        "Unique identifier for the base station serving the CDMA network.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("CELL_ID", "Cell ID",
                        "Unique identifier for the base station serving the GSM/UMTS/LTE network.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("LOCATION_AREA_CODE", "Location Area Code",
                        "Identifier for the geographic region which contains the base station serving the GSM/UMTS network.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("MOBILE_COUNTRY_CODE", "Mobile Country Code",
                        "Unique identifier for the country where the network is located.",
                        "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("MOBILE_NETWORK_CODE", "Mobile Netowrk Code",
                        "Unique identifier for the network within its country.", "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("NETWORK_ID", "Network ID",
                        "Identifier for a geographic region which contains the base station serving the CDMA network.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("IS_ROAMING", "Is roaming",
                        "Flag indicating whether the device is beyond coverage of its home network.",
                        "BOOL"));
        definition->push_back(
                new trigger_output_variable_definition("SIGNAL_STRENGTH", "Signal Strength",
                        "Power measurement of the network's radio signal.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("SYSTEM_ID", "System ID",
                        "Unique identifier for a geographic region which contains the base station serving the CDMA network.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("TRACKING_AREA_CODE", "Tracking Area Code",
                        "Unique identifier for the geographic region which contains the base station serving the LTE network.",
                        "NUMBER"));
        return definition;
    }

    bool TriggerCellularInfo::isActive(ExecutionState* state)
    {
        bb::device::CellularNetworkInfo info;
        std::vector<std::string> *values = new std::vector<std::string>();
        values->push_back(/*NETWORK_DISPLAY_NAME*/SSTR(info.displayName().toStdString()));
        values->push_back(/*BASE_STATION_ID*/SSTR(info.baseStationId()));
        values->push_back(/*CELL_ID*/SSTR(info.cellId()));
        values->push_back(/*LOCATION_AREA_CODE*/SSTR(info.locationAreaCode()));
        values->push_back(/*MOBILE_COUNTRY_CODE*/SSTR(info.mobileCountryCode().toStdString()));
        values->push_back(/*MOBILE_NETWORK_CODE*/SSTR(info.mobileNetworkCode().toStdString()));
        values->push_back(/*NETWORK_ID*/SSTR(info.networkId()));
        values->push_back(/*IS_ROAMING*/SSTR(info.isRoaming()));
        values->push_back(/*SIGNAL_STRENGTH*/SSTR(info.signalStrength()));
        values->push_back(/*SYSTEM_ID*/SSTR(info.systemId()));
        values->push_back(/*TRACKING_AREA_CODE*/SSTR(info.trackingAreaCode()));
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
