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

#include "actiontrigger/controller/services/hub/triggerreceivemessage.hpp"

namespace actiontrigger
{

    std::string TriggerReceiveMessage::TYPE = "TRIGGER_TriggerReceiveMessage";
    std::vector<ParameterDefinition*>* TriggerReceiveMessage::parameterDefinitions = NULL;
    Logger* TriggerReceiveMessage::StatementLOG = LoggerFactory::getInstance(
            "TriggerReceiveMessage");
    const StatementInfo TriggerReceiveMessage::info(true, "Receive Message",
            TriggerReceiveMessage::TYPE,
            "Wait until a message is received from specified account. If you make a change to your accounts, you may need to restart the application for you to see the changes.");

    const StatementInfo TriggerReceiveMessage::getInfo()
    {
        return TriggerReceiveMessage::info;
    }

    TriggerReceiveMessage::TriggerReceiveMessage(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerReceiveMessage::getType() const
    {
        return TriggerReceiveMessage::TYPE;
    }

    TriggerReceiveMessage::~TriggerReceiveMessage()
    {
    }

    std::vector<ParameterDefinition*>* TriggerReceiveMessage::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* account =
                    ParameterDefinition::getDropdown("ACCOUNT_KEY", "Account",
                            "Account to retrieve messages from. If there are no accounts in the list, you may not have given this application permission to access the Hub.",
                            "Account");
            account->dropdownAddOption("All", SSTR(MessageService::accountKeyAll));
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
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("ENABLE_QUEUE", "Queue messages",
                            RichText::html(
                                    "If checked, all the messages will be processed including messages received during action(s) execution. Useful if you want to process ALL incoming messages."
                                            + RichText::newline()
                                            + "If unchecked, all previous messages that occurred before this trigger is executed will not be considered.")));
        }
        return parameterDefinitions;
    }

    std::string TriggerReceiveMessage::getDefaultParameter(std::string key)
    {
        if (key == "ACCOUNT_KEY") {
            return SSTR(MessageService::accountKeyAll);
        } else if (key == "ENABLE_QUEUE") {
            return "TRUE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::vector<statement_output_variable_definition*>* TriggerReceiveMessage::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    bool TriggerReceiveMessage::isActive(ExecutionState* state)
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

        MessageService* messageService = state->getRuntimeResources()->getMessageService(
                accountKey);

        // Check if there are new messages without waiting
        bb::pim::message::Message* nextMsg = messageService->getNextMessage(0);

        if (!isValueTrue(getParameter("ENABLE_QUEUE", state))) {
            // If queue is not enabled, stop receiving any new messages
            messageService->disconnectMsgRecv();
        }

        if (!nextMsg) {
            return false;
        }

        return true;
    }

    void TriggerReceiveMessage::blockUntilActive(ExecutionState* state)
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

        MessageService* messageService = state->getRuntimeResources()->getMessageService(
                accountKey);
        // Continuously check if there are new messages
        bb::pim::message::Message* nextMsg;
        do {
            nextMsg = messageService->getNextMessage();
            state->giveUpIfNotRunning();
        } while (!nextMsg);

        if (!isValueTrue(getParameter("ENABLE_QUEUE", state))) {
            // If queue is not enabled, stop receiving any new messages
            messageService->disconnectMsgRecv();
        }
    }

    bool TriggerReceiveMessage::isEventOnlyTrigger()
    {
        return !isValueTrue(getParameter("ENABLE_QUEUE"));
    }

} /* namespace actiontrigger */
