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

#include "actiontrigger/controller/services/hub/actionsendmessage.hpp"

namespace actiontrigger
{

    std::string ActionSendMessage::TYPE = "ACTION_ActionSendMessage";

    std::vector<ParameterDefinition*>* ActionSendMessage::parameterDefinitions = NULL;
    Logger* ActionSendMessage::StatementLOG = LoggerFactory::getInstance("ActionSendMessage");
    const StatementInfo ActionSendMessage::info(false, "Send message", ActionSendMessage::TYPE,
            "Send a message from PIM account. Supported accounts include email accounts (including POP, IMAP, and so on), SMS messages, and MMS messages. Ensure you have given this application the proper permissions to access your Hub.");

    const StatementInfo ActionSendMessage::getInfo()
    {
        return ActionSendMessage::info;
    }

    ActionSendMessage::ActionSendMessage(StatementModel* model) :
            Action(model)
    {
    }

    ActionSendMessage::~ActionSendMessage()
    {
    }

    std::vector<ParameterDefinition*>* ActionSendMessage::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            ParameterDefinition* account =
                    ParameterDefinition::getDropdown("ACCOUNT_KEY", "Account",
                            "Account to send message through. If there are no accounts in the list, you may not have given this application permission to access the Hub.",
                            "Account");
            QList<bb::pim::account::Account> accounts = MessageService::listAccounts();
            for (QList<bb::pim::account::Account>::iterator iter = accounts.begin();
                    iter != accounts.end(); ++iter) {
                account->dropdownAddOption(
                        SSTR(
                                (*iter).displayName().toStdString() << " "
                                        << (*iter).provider().name().toStdString()
                                        << ((*iter).isEnterprise()
                                                == bb::pim::account::Property::Enterprise ?
                                                " Enterprise" : "")), SSTR((*iter).id()));
            }
            parameterDefinitions->push_back(account);

//        parameterDefinitions->push_back(
//                ParameterDefinition::getString("CONVERSATION", "Conversation Id",
//                        "Optionally pass a conversation Id if replying to a message, otherwise leave empty."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("RECIPIENT", "To",
                            "Recipient to deliver this message to."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("SUBJECT", "Subject", "Message content"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("CONTENT", "Body", "Message content"));
        }
        return parameterDefinitions;
    }

    std::string ActionSendMessage::getType() const
    {
        return ActionSendMessage::TYPE;
    }

    void ActionSendMessage::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        bb::pim::account::AccountKey accountKey;
        try {
            accountKey = Common::StringToNumber<bb::pim::account::AccountKey>(
                    getParameter("ACCOUNT_KEY", state));
        } catch (std::exception& e) {
            LOG->trace(SSTR("Could not extract account key: " << e.what()));
            RUNLOG->error("Please specify a valid account");
            throw ExecutionAbortedException();
        }

        switch (state->getRuntimeResources()->getMessageService(accountKey)->sendMessage(
                getParameter("RECIPIENT", state), getParameter("SUBJECT", state),
                getParameter("CONTENT", state),
                QString::fromStdString(getParameter("CONVERSATION", state)))) {
            case SEND_MESSAGE_SUCCESS:
                RUNLOG->debug("Message requested to be sent successfully");
                break;
            case SEND_MESSAGE_INVALID_RECIPIENT:
                RUNLOG->error("Cannot send message: Invalid recipient");
                break;
            default:
                RUNLOG->error("Cannot send message: Unknown error");
                break;
        }
    }

} /* namespace actiontrigger */
