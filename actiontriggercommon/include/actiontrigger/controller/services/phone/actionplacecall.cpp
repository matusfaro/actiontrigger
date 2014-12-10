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

#include "actiontrigger/controller/services/phone/actionplacecall.hpp"

namespace actiontrigger
{

    std::string ActionPlaceCall::TYPE = "ACTION_ActionPlaceCall";

    std::vector<ParameterDefinition*>* ActionPlaceCall::parameterDefinitions = NULL;
    Logger* ActionPlaceCall::StatementLOG = LoggerFactory::getInstance("ActionPlaceCall");
    const StatementInfo ActionPlaceCall::info(false, "Place call", ActionPlaceCall::TYPE,
            "Use your phone to place a call. Does not work when application is backgrounded. User is prompted before the call is made.");

    const StatementInfo ActionPlaceCall::getInfo()
    {
        return ActionPlaceCall::info;
    }

    ActionPlaceCall::ActionPlaceCall(StatementModel* model) :
            Action(model)
    {
    }

    ActionPlaceCall::~ActionPlaceCall()
    {
        StatementLOG->trace("~ActionPlaceCall");
    }

    std::vector<ParameterDefinition*>* ActionPlaceCall::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getDropdown("LINE", "Line",
                    "Select the line to use", "Line");
            state->dropdownAddOption("Cellular", "CELL");
            state->dropdownAddOption("Video Chat", "VIDEO");
            state->dropdownAddOption("Mobile Voice System", "MVS");
            parameterDefinitions->push_back(state);
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("TARGET", "Contact", "Contact to call"));
        }
        return parameterDefinitions;
    }

    std::string ActionPlaceCall::getDefaultParameter(std::string key)
    {
        if (key == "LINE") {
            return "CELL";
        } else if (key == "TARGET") {
            return "555-0100";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionPlaceCall::getType() const
    {
        return ActionPlaceCall::TYPE;
    }

    void ActionPlaceCall::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        bb::system::phone::Phone phone;
        bb::system::phone::LineType::Type line = PhoneService::stringToLineType(
                getParameter("LINE"));
        std::string target = getParameter("TARGET", state);

        if (line == bb::system::phone::LineType::Cellular) {
            // For cellular, use smart dialing as well
            phone.initiateCellularCall(QString::fromStdString(target), true);
        } else {
            phone.initiateCall(QString::fromStdString(target), line);
        }
        RUNLOG->debug("Dialed '" + target + "'");
    }

} /* namespace actiontrigger */
