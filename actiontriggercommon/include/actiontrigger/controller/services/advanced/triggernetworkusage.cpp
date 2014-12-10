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

#include "actiontrigger/controller/services/advanced/triggernetworkusage.hpp"

namespace actiontrigger
{

    std::string TriggerNetworkUsage::TYPE = "TRIGGER_TriggerNetworkUsage";
    Logger* TriggerNetworkUsage::StatementLOG = LoggerFactory::getInstance("TriggerNetworkUsage");
    const StatementInfo TriggerNetworkUsage::info(true, "Network Usage", TriggerNetworkUsage::TYPE,
            "Triggers on network usage such as Wifi and Cellular");

    const StatementInfo TriggerNetworkUsage::getInfo()
    {
        return TriggerNetworkUsage::info;
    }

    TriggerNetworkUsage::TriggerNetworkUsage(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerNetworkUsage::getType() const
    {
        return TriggerNetworkUsage::TYPE;
    }

    TriggerNetworkUsage::~TriggerNetworkUsage()
    {
    }

    bool TriggerNetworkUsage::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerNetworkUsage::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        std::map<std::string, std::string>* interfaces = NetworkUsage::get()->supportedInterfaces();
        for (std::map<std::string, std::string>::iterator it = interfaces->begin();
                it != interfaces->end(); ++it) {
            definition->push_back(
                    new trigger_output_variable_definition("BYTES_" + it->first + "_IN",
                            it->first + " inbound bytes",
                            "Holds count of inbound bytes for " + it->first, "NUMBER"));
            definition->push_back(
                    new trigger_output_variable_definition("MBYTES_" + it->first + "_IN",
                            it->first + " inbound megabytes",
                            "Holds count of inbound megabytes for " + it->first, "NUMBER"));
            definition->push_back(
                    new trigger_output_variable_definition("BYTES_" + it->first + "_OUT",
                            it->first + " outbound bytes",
                            "Holds count of outbound bytes for " + it->first, "NUMBER"));
            definition->push_back(
                    new trigger_output_variable_definition("MBYTES_" + it->first + "_OUT",
                            it->first + " outbound megabytes",
                            "Holds count of outbound megabytes for " + it->first, "NUMBER"));
            definition->push_back(
                    new trigger_output_variable_definition("BYTES_" + it->first + "_TOTAL",
                            it->first + " total bytes",
                            "Holds count of total bytes for " + it->first, "NUMBER"));
            definition->push_back(
                    new trigger_output_variable_definition("MBYTES_" + it->first + "_TOTAL",
                            it->first + " total megabytes",
                            "Holds count of total megabytes for " + it->first, "NUMBER"));
        }
        return definition;
    }

    bool TriggerNetworkUsage::isActive(ExecutionState* state)
    {
        std::vector<std::string>* values = new std::vector<std::string>();
        NetworkUsage::get()->update();
        std::map<std::string, std::string>* usage = NetworkUsage::get()->fetch();
        for (std::map<std::string, std::string>::iterator it = usage->begin(); it != usage->end();
                ++it) {
            values->push_back(/*BYTES*/it->second);
            values->push_back(
                    /*MBYTES*/SSTR(
                            Common::StringToNumber<unsigned long>(it->second) / 1024 / 1024));
        }
        return checkRequirements(state, values);
    }

} /* namespace actiontrigger */
