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

#ifndef ActionSetPersistentVar_HPP_
#define ActionSetPersistentVar_HPP_

#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/model/database/sqlite/persistentstorage.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/controller/globalresources.hpp"
#include "actiontrigger/view/cascades/procedurework.hpp"
#include "actiontrigger/controller/common.hpp"

namespace actiontrigger
{

    class ActionSetPersistentVar: public Action
    {
    public:
        ActionSetPersistentVar(StatementModel* model);
        virtual ~ActionSetPersistentVar();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
    protected:
        void execute(ExecutionState* state);
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;
    };

} /* namespace actiontrigger */
#endif /* ActionSetPersistentVar_HPP_ */
