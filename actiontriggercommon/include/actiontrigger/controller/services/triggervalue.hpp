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

#ifndef TRIGGERVALUE_HPP_
#define TRIGGERVALUE_HPP_

#include <map>
#include <utility>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include <math.h>

namespace actiontrigger
{

// Variable definition
    typedef KeyNameDescriptionType trigger_output_variable_definition;

    class TriggerValue: public Trigger
    {
        typedef bool (TriggerValue::*ReqFunc)(ExecutionState* state, std::string type,
                std::string key, std::string value);
    public:
        TriggerValue(StatementModel* model);
        ~TriggerValue();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
        std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();
    protected:
        virtual std::vector<trigger_output_variable_definition*>* getValueDefinitions() = 0;
        // Check requirements
        bool checkRequirements(ExecutionState* state, std::vector<std::string>* values);
    private:
        static Logger* StatementLOG;
        std::vector<ParameterDefinition*>* parameterDefinitions;
        std::map<std::string, ReqFunc> requirementToFunctionMap;
        bool requirementTrue(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementFalse(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementRegex(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementContains(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementEqual(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementDetectDifference(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementDetectChange(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementNumberInRange(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementNumberInRangeOrOn(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementNumberHigher(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementNumberHigherOrEqual(ExecutionState* state, std::string type,
                std::string key, std::string value);
        bool requirementNumberHigherAbsoluteValue(ExecutionState* state, std::string type,
                std::string key, std::string value);
        bool requirementNumberLower(ExecutionState* state, std::string type, std::string key,
                std::string value);
        bool requirementNumberLowerOrEqual(ExecutionState* state, std::string type, std::string key,
                std::string value);
    };

} /* namespace actiontrigger */
#endif /* TRIGGERVALUE_HPP_ */
