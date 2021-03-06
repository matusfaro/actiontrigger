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

#ifndef ActionLinearAcceleration_HPP_
#define ActionLinearAcceleration_HPP_

#include <errno.h>
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/controller/services/sensors/sensorhandler.hpp"

namespace actiontrigger
{

    class ActionLinearAcceleration: public Action
    {
    public:
        static bool isSupported();
        ActionLinearAcceleration(StatementModel* model);
        virtual ~ActionLinearAcceleration();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();
    protected:
        void execute(ExecutionState* state);
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;
    };

} /* namespace actiontrigger */
#endif /* ActionLinearAcceleration_HPP_ */
