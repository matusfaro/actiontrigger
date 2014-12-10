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

#include "actiontrigger/controller/services/invocation/actioninvokephone.hpp"

namespace actiontrigger
{

    std::string ActionInvokePhone::TYPE = "ACTION_INVOKE_PHONE";

    std::vector<ParameterDefinition*>* ActionInvokePhone::parameterDefinitions = NULL;
    Logger* ActionInvokePhone::StatementLOG = LoggerFactory::getInstance("ActionInvokePhone");
    const StatementInfo ActionInvokePhone::info(false, "Phone invocation", ActionInvokePhone::TYPE,
            RichText::html(
                    "Invokes phone application for initiating calls and viewing call logs."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: Due to Blackberry restrictions, invocation will not work if app is minimized.")));

    const StatementInfo ActionInvokePhone::getInfo()
    {
        return ActionInvokePhone::info;
    }

    ActionInvokePhone::ActionInvokePhone(StatementModel* model) :
            Action(model)
    {
    }

    ActionInvokePhone::~ActionInvokePhone()
    {
    }

    std::vector<ParameterDefinition*>* ActionInvokePhone::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            // First let's display the filter dropdown
            ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION", "Action",
                    "Phone action to invoke", ".");
            parameterDefinitions->push_back(filter);
            filter->createFilter("Open call logs", "OPEN_CALL_LOGS");
            filter->createFilter("Call number", "CALL_NUMBER");
            filter->addToFilter(
                    ParameterDefinition::getNumber("NUMBER", "Phone number",
                            "Phone number to dial on invocation"), parameterDefinitions);
            // TODO Decide what to do with this. Commented out for now because it doesn't work for unknown reason without SIM and radio.
            //filter->createFilter("Call emergency number", "CALL_EMERGENCY");
        }
        return parameterDefinitions;
    }

    std::string ActionInvokePhone::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "OPEN_CALL_LOGS";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionInvokePhone::getType() const
    {
        return ActionInvokePhone::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionInvokePhone::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_DATA",
                        "Data received from invocation.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_DATA_TYPE",
                        "Type of data received from invocation.", "STRING"));
        return vars;
    }

    void ActionInvokePhone::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        // Retrieve desired action
        std::string action = getParameter("ACTION", state);

        // Determine parameters
        std::string targetId = "";
        std::string invokeAction = "";
        std::string mimeType = "";
        std::string uri = "";
        QByteArray data = "";
        if (action == "") {
            RUNLOG->error("Select an action to perform");
            return;
        } else if (action == "OPEN_CALL_LOGS") {
            invokeAction = "bb.action.OPEN";
            mimeType = "application/vnd.blackberry.calllog.id";
        } else if (action == "CALL_NUMBER") {
            invokeAction = "bb.action.DIAL";
            mimeType = "application/vnd.blackberry.phone.startcall";
            QVariantMap map;
            map.insert("number", QString::fromStdString(getParameter("NUMBER", state)));
            data = bb::PpsObject::encode(map, NULL);
        } else if (action == "CALL_EMERGENCY") {
            invokeAction = "bb.action.EMERGENCY_CALL";
            mimeType = "application/vnd.blackberry.phone.startcall";
        } else {
            LOG->error(SSTR("Error invoking, unknown desired action: " << action));
        }

        Invocation* invocation = new Invocation();
        bb::system::InvokeTargetReply* reply = invocation->invoke(targetId, invokeAction, mimeType,
                uri, data);
        if (!reply->isFinished()) {
            RUNLOG->error("Timed out invoking target");
            return;
        } else if (reply->error() != bb::system::InvokeReplyError::None) {
            RUNLOG->error(
                    "Target invoke failed: " + Invocation::errorMessageToString(reply->error()));
            return;
        }
    }

} /* namespace actiontrigger */
