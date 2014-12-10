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

#include "actiontrigger/controller/services/sample/branchsample.hpp"

namespace actiontrigger
{

    std::string BranchSample::TYPE = "BRANCH_BranchSample";

    std::vector<ParameterDefinition*>* BranchSample::parameterDefinitions = NULL;
    Logger* BranchSample::StatementLOG = LoggerFactory::getInstance("BranchSample");
    const StatementInfo BranchSample::info(false, "BranchSample", BranchSample::TYPE,
            "Branch to x as it approaches 2pi");

    const StatementInfo BranchSample::getInfo()
    {
        return BranchSample::info;
    }

    BranchSample::BranchSample(StatementModel* model) :
            Branch(model)
    {

        // T O D O populate branch info
        branchInfo.push_back(branch_info("IF", "If", "Conditions", "TRIGGER"));
        branchInfo.push_back(branch_info("THEN", "Then", "Conditions met", "ACTION"));
        branchInfo.push_back(branch_info("ELSE", "Else", "Conditions not met", "ACTION"));

    }

    BranchSample::~BranchSample()
    {
    }

    std::vector<ParameterDefinition*>* BranchSample::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            // T O D O populate inputs
            ParameterDefinition* state =
                    ParameterDefinition::getDropdown("OPERTATOR", "Behavior",
                            "In order for the 'Then' branch to execute, either all triggers need to be active or at least one.");
            state->dropdownAddOption("All true", "AND");
            state->dropdownAddOption("At least one", "OR");
            parameterDefinitions->push_back(state);

        }
        return parameterDefinitions;
    }

    std::string BranchSample::getDefaultParameter(std::string key)
    {
        if (key == "Sample") {
            return "asdf";
        } else if (key == "Sample") {
            return "fdsa";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string BranchSample::getType() const
    {
        return BranchSample::TYPE;
    }

    void BranchSample::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        // T O D O branch
        getParameter("BLAH", state);
        if (getTriggerBranch("IF")->blockTriggerState(state)) {
            RUNLOG->debug("Conditions met");
            getBranch("THEN")->execute(state);
        } else {
            RUNLOG->debug("Conditions not met");
            getBranch("ELSE")->execute(state);
        }
    }

} /* namespace actiontrigger */
