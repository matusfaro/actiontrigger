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

#include "actiontrigger/controller/services/advanced/actionnetworkusage.hpp"

namespace actiontrigger
{

    std::string ActionNetworkUsage::TYPE = "ACTION_ActionNetworkUsage";

    std::vector<ParameterDefinition*>* ActionNetworkUsage::parameterDefinitions = NULL;
    Logger* ActionNetworkUsage::StatementLOG = LoggerFactory::getInstance("ActionNetworkUsage");
    const StatementInfo ActionNetworkUsage::info(false, "Network Usage", ActionNetworkUsage::TYPE,
            "Retrieves network usage. The network usage counter is a shared resource. Note that the device clears the internal network usage counter on reboot. This action retrieves the values only when executed. Any network usage between the last call to this action and a device reboot will not be picked up by this application after the reboot.");

    const StatementInfo ActionNetworkUsage::getInfo()
    {
        return ActionNetworkUsage::info;
    }

    ActionNetworkUsage::ActionNetworkUsage(StatementModel* model) :
            Action(model)
    {
    }

    ActionNetworkUsage::~ActionNetworkUsage()
    {
    }

    std::string ActionNetworkUsage::getType() const
    {
        return ActionNetworkUsage::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionNetworkUsage::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        std::map<std::string, std::string>* interfaces = NetworkUsage::get()->supportedInterfaces();
        for (std::map<std::string, std::string>::iterator it = interfaces->begin();
                it != interfaces->end(); ++it) {
            vars->push_back(
                    new statement_output_variable_definition("BYTES_" + it->first + "_IN",
                            "Holds count of inbound bytes for " + it->first, "NUMBER"));
            vars->push_back(
                    new statement_output_variable_definition("MBYTES_" + it->first + "_IN",
                            "Holds count of inbound megabytes for " + it->first, "NUMBER"));
            vars->push_back(
                    new statement_output_variable_definition("BYTES_" + it->first + "_OUT",
                            "Holds count of outbound bytes for " + it->first, "NUMBER"));
            vars->push_back(
                    new statement_output_variable_definition("MBYTES_" + it->first + "_OUT",
                            "Holds count of outbound megabytes for " + it->first, "NUMBER"));
        }
        return vars;
    }

    void ActionNetworkUsage::execute(ExecutionState* state)
    {
        NetworkUsage::get()->update();
        std::map<std::string, std::string>* usage = NetworkUsage::get()->fetch();
        for (std::map<std::string, std::string>::iterator it = usage->begin(); it != usage->end();
                ++it) {
            state->setUserProperty("BYTES_" + it->first, it->second);
            state->setUserProperty("MBYTES_" + it->first,
                    SSTR(Common::StringToNumber<unsigned long>(it->second) / 1024 / 1024));
        }
        delete usage;
    }

} /* namespace actiontrigger */
