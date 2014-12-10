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

#include "actiontrigger/controller/services/advanced/actionclearnetworkusage.hpp"

namespace actiontrigger
{

    std::string ActionClearNetworkUsage::TYPE = "ACTION_ActionClearNetworkUsage";

    std::vector<ParameterDefinition*>* ActionClearNetworkUsage::parameterDefinitions = NULL;
    Logger* ActionClearNetworkUsage::StatementLOG = LoggerFactory::getInstance(
            "ActionClearNetworkUsage");
    const StatementInfo ActionClearNetworkUsage::info(false, "Clear Network Usage",
            ActionClearNetworkUsage::TYPE,
            "Clears the network usage counter. The network usage counter is a shared resource and all procedures will be effected.");

    const StatementInfo ActionClearNetworkUsage::getInfo()
    {
        return ActionClearNetworkUsage::info;
    }

    ActionClearNetworkUsage::ActionClearNetworkUsage(StatementModel* model) :
            Action(model)
    {
    }

    ActionClearNetworkUsage::~ActionClearNetworkUsage()
    {
    }

    std::string ActionClearNetworkUsage::getType() const
    {
        return ActionClearNetworkUsage::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionClearNetworkUsage::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionClearNetworkUsage::execute(ExecutionState* state)
    {
        (void) state;
        NetworkUsage::get()->reset();
        NetworkUsage::get()->update();
    }

} /* namespace actiontrigger */
