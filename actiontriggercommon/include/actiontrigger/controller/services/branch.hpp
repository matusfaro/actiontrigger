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

#ifndef BRANCH_HPP_
#define BRANCH_HPP_

#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/globalresources.hpp"

namespace actiontrigger
{

    class Branch: public Action
    {
    public:
        Branch(StatementModel* model);
        virtual void initBranches(std::map<std::string, Trigger*>* triggerBranches,
                std::map<std::string, Action*>* actionBranches);
        virtual ~Branch();
//    virtual void remove();
        void removeChild(std::string branchName, Statement* statement);

        // Move statement within a branch
        bool moveUp(std::string branchName, Statement* statement);
        bool moveDown(std::string branchName, Statement* statement);

        Statement* newBranchChild(std::string branchName, std::string statementType);
        Statement* pasteBranchChild(std::string branchName);
        Statement* getBranch(std::string key);
        virtual Statement* getBranch(std::string key, std::string type);
        Action* getActionBranch(std::string key);
        Trigger* getTriggerBranch(std::string key);

        virtual std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();

    private:
        Statement* appendBranchChild(std::string branchName, StatementModel* newModel);
        std::map<std::string, Trigger*>* triggerBranches;
        std::map<std::string, Action*>* actionBranches;
        // Retrieves a branch of statements. Null if no statements
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* BRANCH_HPP_ */
