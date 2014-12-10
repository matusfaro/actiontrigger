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

#include "actiontrigger/controller/services/flow/branchwaituntil.hpp"

namespace actiontrigger
{

    std::string BranchWaitUntil::TYPE = "BRANCH_BranchWaitUntil";

    std::vector<ParameterDefinition*>* BranchWaitUntil::parameterDefinitions = NULL;
    Logger* BranchWaitUntil::StatementLOG = LoggerFactory::getInstance("BranchWaitUntil");
    const StatementInfo BranchWaitUntil::info(false, "Wait until", BranchWaitUntil::TYPE,
            "Halts execution until triggers are active. Can be configured to wait until at least one trigger is active or all are active at the some time.");

    const StatementInfo BranchWaitUntil::getInfo()
    {
        return BranchWaitUntil::info;
    }

    BranchWaitUntil::BranchWaitUntil(StatementModel* model) :
            Branch(model)
    {
        branchInfo.push_back(branch_info("UNTIL", "Until", "Wait Until", "TRIGGER"));
    }

    BranchWaitUntil::~BranchWaitUntil()
    {
    }

    std::vector<ParameterDefinition*>* BranchWaitUntil::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state =
                    ParameterDefinition::getDropdown("OPERATOR", "Behavior",
                            "In order for the 'Then' actions to execute, either all triggers need to be active or at least one.");
            state->dropdownAddOption("All true", "AND");
            state->dropdownAddOption("At least one", "OR");
            parameterDefinitions->push_back(state);

        }
        return parameterDefinitions;
    }

    std::string BranchWaitUntil::getDefaultParameter(std::string key)
    {
        if (key == "OPERATOR") {
            return "AND";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string BranchWaitUntil::getType() const
    {
        return BranchWaitUntil::TYPE;
    }

    void BranchWaitUntil::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string op = getParameter("OPERATOR", state);
        Trigger* untilBranch = getTriggerBranch("UNTIL");
        if (untilBranch == NULL) {
            RUNLOG->error("Empty until clause");
            return;
        }

        if (op == "OR") {
            // Continuously check state until at least one is active
            useconds_t granularity = getGranularity(state);
            while (true) {
                state->giveUpIfNotRunning();
                if (untilBranch->checkTriggerStateOperatorOr(state))
                    return;
                state->giveUpIfNotRunning();
                if (granularity != 0)
                    usleep(granularity);
            }
        } else {
            // Block until all are active
            untilBranch->blockTriggerState(state);
        }
    }

} /* namespace actiontrigger */
