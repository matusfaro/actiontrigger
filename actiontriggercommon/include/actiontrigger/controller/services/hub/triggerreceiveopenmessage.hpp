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

#ifndef TriggerReceiveOpenMessage_HPP_
#define TriggerReceiveOpenMessage_HPP_

#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/controller/services/hub/messageservice.hpp"
#include "actiontrigger/view/cascades/richtext.hpp"

namespace actiontrigger
{

    class TriggerReceiveOpenMessage: public Trigger
    {
    public:
        TriggerReceiveOpenMessage(StatementModel* model);
        ~TriggerReceiveOpenMessage();
        static std::string TYPE;
        std::string getType() const;
        bool isActive(ExecutionState* state);
        void blockUntilActive(ExecutionState* state);
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
        std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();
    protected:
        std::string getDefaultParameter(std::string key);
        bool isEventOnlyTrigger();
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;
        bool trigger(ExecutionState* state, bool block);
    };

} /* namespace actiontrigger */
#endif /* TriggerReceiveOpenMessage_HPP_ */
