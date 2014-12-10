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

#ifndef BranchWhile_HPP_
#define BranchWhile_HPP_

#include "actiontrigger/controller/services/branch.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"

namespace actiontrigger
{

    class BranchWhile: public Branch
    {
    public:
        BranchWhile(StatementModel* model);
        virtual ~BranchWhile();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
    protected:
        std::string getDefaultParameter(std::string key);
        void execute(ExecutionState* state);
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;

        bool conditionMet(ExecutionState* state, std::string op, Trigger* condition);
    };

} /* namespace actiontrigger */
#endif /* BranchWhile_HPP_ */
