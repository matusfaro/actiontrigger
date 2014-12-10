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

#include "actiontrigger/controller/services/flow/branchdowhile.hpp"

namespace actiontrigger
{

    std::string BranchDoWhile::TYPE = "BRANCH_BranchDoWhile";

    std::vector<ParameterDefinition*>* BranchDoWhile::parameterDefinitions = NULL;
    Logger* BranchDoWhile::StatementLOG = LoggerFactory::getInstance("BranchDoWhile");
    const StatementInfo BranchDoWhile::info(false, "Do-While", BranchDoWhile::TYPE,
            "Execute 'do' actions once. Then 'while' conditions are met, execute 'do' actions again. This loop catches 'continue' and 'break' signals.");

    const StatementInfo BranchDoWhile::getInfo()
    {
        return BranchDoWhile::info;
    }

    BranchDoWhile::BranchDoWhile(StatementModel* model) :
            Branch(model)
    {
        branchInfo.push_back(branch_info("DO", "Do", "Do this", "ACTION"));
        branchInfo.push_back(branch_info("WHILE", "While", "Conditions", "TRIGGER"));
    }

    BranchDoWhile::~BranchDoWhile()
    {
    }

    std::vector<ParameterDefinition*>* BranchDoWhile::getParameterDefinitions()
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

    std::string BranchDoWhile::getDefaultParameter(std::string key)
    {
        if (key == "OPERATOR") {
            return "AND";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string BranchDoWhile::getType() const
    {
        return BranchDoWhile::TYPE;
    }

    void BranchDoWhile::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::string op = getParameter("OPERATOR", state);
        Trigger* condition = getTriggerBranch("WHILE");
        if (condition == NULL) {
            RUNLOG->error("Empty condition");
            return;
        }

        Action* doBranch = getActionBranch("DO");

        do {
            std::size_t layerCount = RUNLOG->layerCount();
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
            if (!state->isRunning())
                throw ExecutionAbortedException();
        } while (conditionMet(state, op, condition));
    }

    bool BranchDoWhile::conditionMet(ExecutionState* state, std::string op, Trigger* condition)
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
