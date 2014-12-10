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

#include "actiontrigger/controller/services/invocation/actioninvokecustom.hpp"

namespace actiontrigger
{

    std::string ActionInvokeCustom::TYPE = "ACTION_ActionInvokeCustom";

    std::vector<ParameterDefinition*>* ActionInvokeCustom::parameterDefinitions = NULL;
    Logger* ActionInvokeCustom::StatementLOG = LoggerFactory::getInstance("ActionInvokeCustom");
    const StatementInfo ActionInvokeCustom::info(false, "Custom invocation",
            ActionInvokeCustom::TYPE,
            RichText::html(
                    "Invokes external application, service or card." + RichText::newline()
                            + RichText::color("red",
                                    "Warning: Due to Blackberry restrictions, invocation will not work if app is minimized.")));

    const StatementInfo ActionInvokeCustom::getInfo()
    {
        return ActionInvokeCustom::info;
    }

    ActionInvokeCustom::ActionInvokeCustom(StatementModel* model) :
            Action(model)
    {
    }

    ActionInvokeCustom::~ActionInvokeCustom()
    {
    }

    std::vector<ParameterDefinition*>* ActionInvokeCustom::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            /*// TODO show available invokations
             ParameterDefinition* state = ParameterDefinition::getDropdown("ACTION", "Actions", "Attempt to turn Wifi on or off", "Desired state");
             Invocation invocation = new Invocation();
             bb::system::InvokeQueryTargetsReply* actions = invocation.query();
             state->dropdownAddOption("On", "on");
             state->dropdownAddOption("Off", "off");
             parameterDefinitions->push_back(state);
             */
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("ACTION", "Action",
                            "Action to invoke in target. Valid values are target-specific. If omitted, the invocation service or target will choose an appropriate action based on the MIME type. Examples: bb.action.OPEN, bb.action.SET, bb.action.SHARE"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("TARGET", "Target",
                            "The identity of the receiver as stated in its BAR manifest file. If omitted, the invocation service performs brokering to find the most appropriate target for the action and MIME type in this invocation request."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("URI", "URI",
                            "The uniform resource identifier (URI) sent to the target. If omitted, the MIME type, action, and/or data are sufficient for the target to do its work."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("MIME", "Mime Type",
                            "Format of the data sent to the target. It must be a valid Internet media type, such as image/png. If empty, an attempt will be made to deduce the type."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("DATA", "Data", "Optional data to send."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("GET_RESPONSE", "Wait for response",
                            "If enabled, the action will wait until the invoked target returns a response. If response is not sent as part of the target action, this will have to timeout. Response is set to variable INVOCATION_REPLY_DATA with type of INVOCATION_REPLY_DATA_TYPE"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("TIMEOUT", "Timeout",
                            "Amount of time (milliseconds) to wait for a response before giving up. Only used if wait for response is true",
                            "1000", "60000", "500"));
        }
        return parameterDefinitions;
    }

    std::string ActionInvokeCustom::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "bb.action.OPEN";
        } else if (key == "URI") {
            return "https://developer.blackberry.com/native/documentation/cascades/device_platform/invocation/invoking_core_apps.html";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionInvokeCustom::getType() const
    {
        return ActionInvokeCustom::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionInvokeCustom::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_TYPE",
                        "Type of data received from invocation.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_DATA",
                        "Data received from invocation.", "STRING"));
        return vars;
    }

    void ActionInvokeCustom::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        long timeout;
        try {
            timeout = Common::StringToNumber<useconds_t>(getParameter("TIMEOUT", state));
        } catch (std::exception& e) {
            LOG->trace(SSTR("Could not extract timeout: " << e.what()));
            timeout = 1000L;
        }
        timeout = timeout < 1000L ? 1000L : timeout;

        Invocation* invocation = new Invocation();
        bb::system::InvokeTargetReply* reply = invocation->invoke(getParameter("TARGET", state),
                getParameter("ACTION", state), getParameter("MIME", state),
                getParameter("URI", state), getParameter("DATA", state));
        if (!reply->isFinished()) {
            RUNLOG->error("Timed out invoking target");
            return;
        } else if (reply->error() != bb::system::InvokeReplyError::None) {
            RUNLOG->error(
                    "Target invoke failed: " + Invocation::errorMessageToString(reply->error()));
            return;
        }
        if (isValueTrue(getParameter("GET_RESPONSE", state))) {
            const bb::system::CardDoneMessage* msg = invocation->getCardResponse(timeout);
            if (msg == NULL) {
                RUNLOG->error("Timed out waiting for data from target");
                return;
            }
            state->setUserProperty("INVOCATION_REPLY_TYPE", msg->dataType().toStdString());
            state->setUserProperty("INVOCATION_REPLY_DATA", msg->data().toStdString());
        }
    }

} /* namespace actiontrigger */
