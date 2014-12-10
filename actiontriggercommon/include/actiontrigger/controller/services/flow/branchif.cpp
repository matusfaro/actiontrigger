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

#include "actiontrigger/controller/services/flow/branchif.hpp"

namespace actiontrigger
{

    std::string BranchIf::TYPE = "BRANCH_BranchIf";

    std::vector<ParameterDefinition*>* BranchIf::parameterDefinitions = NULL;
    Logger* BranchIf::StatementLOG = LoggerFactory::getInstance("BranchIf");
    const StatementInfo BranchIf::info(false, "If-Then-Else", BranchIf::TYPE,
            "Evaluates conditions. If met, executes 'Then' actions, otherwise executes 'Else' actions.");

    const StatementInfo BranchIf::getInfo()
    {
        return BranchIf::info;
    }

    BranchIf::BranchIf(StatementModel* model) :
            Branch(model)
    {
        branchInfo.push_back(branch_info("IF", "If", "Conditions", "TRIGGER"));
        branchInfo.push_back(branch_info("THEN", "Then", "Conditions met", "ACTION"));
        branchInfo.push_back(branch_info("ELSE", "Else", "Conditions not met", "ACTION"));
    }

    BranchIf::~BranchIf()
    {
    }

    std::vector<ParameterDefinition*>* BranchIf::getParameterDefinitions()
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

    std::string BranchIf::getDefaultParameter(std::string key)
    {
        if (key == "OPERATOR") {
            return "AND";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string BranchIf::getType() const
    {
        return BranchIf::TYPE;
    }

    void BranchIf::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string op = getParameter("OPERATOR", state);
        Trigger* ifBranch = getTriggerBranch("IF");
        if (ifBranch == NULL) {
            RUNLOG->error("Empty IF clause");
            return;
        }

        bool conditionMet;
        if (op == "OR") {
            if (ifBranch->checkTriggerStateOperatorOr(state)) {
                conditionMet = true;
            } else {
                conditionMet = false;
            }
        } else {
            if (ifBranch->checkTriggerStateOperatorAnd(state)) {
                conditionMet = true;
            } else {
                conditionMet = false;
            }
        }

        if (conditionMet) {
            RUNLOG->debug("Conditions met");
            Action* thenBranch = getActionBranch("THEN");
            if (thenBranch == NULL) {
                RUNLOG->debug("Empty THEN clause");
                return;
            }
            thenBranch->run(state);
        } else {
            RUNLOG->debug("Conditions not met");
            Action* elseBranch = getActionBranch("ELSE");
            if (elseBranch == NULL) {
                RUNLOG->debug("Empty ELSE clause");
                return;
            }
            elseBranch->run(state);
        }
    }

} /* namespace actiontrigger */
