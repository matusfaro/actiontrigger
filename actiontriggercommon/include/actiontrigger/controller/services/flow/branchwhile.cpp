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

#include "actiontrigger/controller/services/flow/branchwhile.hpp"

namespace actiontrigger
{

    std::string BranchWhile::TYPE = "BRANCH_BranchWhile";

    std::vector<ParameterDefinition*>* BranchWhile::parameterDefinitions = NULL;
    Logger* BranchWhile::StatementLOG = LoggerFactory::getInstance("BranchWhile");
    const StatementInfo BranchWhile::info(false, "While-Do", BranchWhile::TYPE,
            "While conditions are met, 'do' actions are executed.");

    const StatementInfo BranchWhile::getInfo()
    {
        return BranchWhile::info;
    }

    BranchWhile::BranchWhile(StatementModel* model) :
            Branch(model)
    {
        branchInfo.push_back(branch_info("WHILE", "While", "Conditions", "TRIGGER"));
        branchInfo.push_back(branch_info("DO", "Do", "Do this", "ACTION"));
    }

    BranchWhile::~BranchWhile()
    {
    }

    std::vector<ParameterDefinition*>* BranchWhile::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state =
                    ParameterDefinition::getDropdown("OPERATOR", "Behavior",
                            "In order for the 'Do' actions to execute, either all triggers need to be active or at least one. This loop catches 'continue' and 'break' signals.");
            state->dropdownAddOption("All true", "AND");
            state->dropdownAddOption("At least one", "OR");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string BranchWhile::getDefaultParameter(std::string key)
    {
        if (key == "OPERATOR") {
            return "AND";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string BranchWhile::getType() const
    {
        return BranchWhile::TYPE;
    }

    void BranchWhile::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string op = getParameter("OPERATOR", state);
        Trigger* condition = getTriggerBranch("WHILE");
        if (condition == NULL) {
            RUNLOG->error("Empty condition");
            return;
        }

        Action* doBranch = getActionBranch("DO");

        while (conditionMet(state, op, condition)) {
            std::size_t layerCount = RUNLOG->layerCount();
            if (!state->isRunning())
                throw ExecutionAbortedException();
            try {
                if (doBranch == NULL) {
                    RUNLOG->error("No actions to execute");
                } else {
                    doBranch->run(state);
                }
            } catch (const LoopContinueException &e) {
                RUNLOG->shrinkLayers(layerCount);
                RUNLOG->debug("Caught loop continue");
                continue;
            } catch (const LoopBreakException &e) {
                RUNLOG->shrinkLayers(layerCount);
                RUNLOG->debug("Caught loop break");
                break;
            }
        }
    }

    bool BranchWhile::conditionMet(ExecutionState* state, std::string op, Trigger* condition)
    {
        if (op == "OR") {
            if (condition->checkTriggerStateOperatorOr(state)) {
                return true;
            } else {
                return false;
            }
        } else {
            if (condition->checkTriggerStateOperatorAnd(state)) {
                return true;
            } else {
                return false;
            }
        }
    }

} /* namespace actiontrigger */
