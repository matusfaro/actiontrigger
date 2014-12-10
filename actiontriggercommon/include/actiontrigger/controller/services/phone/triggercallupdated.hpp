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

#ifndef TriggerCallUpdated_HPP_
#define TriggerCallUpdated_HPP_

#include "actiontrigger/controller/services/triggervalue.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/controller/services/phone/phoneservice.hpp"

namespace actiontrigger
{

    class TriggerCallUpdated: public TriggerValue
    {
    public:
        TriggerCallUpdated(StatementModel* model);
        ~TriggerCallUpdated();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
        std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();
        bool isActive(ExecutionState* state);
        void blockUntilActive(ExecutionState* state);
    protected:
        std::string getDefaultParameter(std::string key);
        bool isEventOnlyTrigger();
        // Pair contains variable name and type
        std::vector<trigger_output_variable_definition*>* getValueDefinitions();
    private:
        static Logger* StatementLOG;
        std::vector<ParameterDefinition*>* parameterDefinitions;
        bool trigger(ExecutionState* state, bool block);
        bool checkCall(ExecutionState* state, const bb::system::phone::Call* call);
    };

} /* namespace actiontrigger */
#endif /* TriggerCallUpdated_HPP_ */
