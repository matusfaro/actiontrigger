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

#ifndef SYSTEMTRUE_HPP_
#define SYSTEMTRUE_HPP_

#include <iostream>
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include <unistd.h>
#include "actiontrigger/controller/executionstate.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"

namespace actiontrigger
{

    class TriggerSystemTrue: public Trigger
    {
    public:
        TriggerSystemTrue(StatementModel* model);
        ~TriggerSystemTrue();
        static std::string TYPE;
        std::string getType() const;
        bool isActive(ExecutionState* state);
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
    protected:
        bool isEventOnlyTrigger();
        void blockUntilActive(ExecutionState* state);
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;
    };

} /* namespace actiontrigger */
#endif /* SYSTEMTRUE_HPP_ */
