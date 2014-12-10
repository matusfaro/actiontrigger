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

#include "actiontrigger/controller/services/hub/triggerreceiveopenmessage.hpp"

namespace actiontrigger
{

    std::string TriggerReceiveOpenMessage::TYPE = "TRIGGER_TriggerReceiveOpenMessage";
    std::vector<ParameterDefinition*>* TriggerReceiveOpenMessage::parameterDefinitions = NULL;
    Logger* TriggerReceiveOpenMessage::StatementLOG = LoggerFactory::getInstance(
            "TriggerReceiveOpenMessage");
    const StatementInfo TriggerReceiveOpenMessage::info(true, "Receive and open Message",
            TriggerReceiveOpenMessage::TYPE,
            "Wait until a message is received from specified account and makes the content available for use via variables. If you make a change to your accounts, you may need to restart the application for you to see the changes.");

    const StatementInfo TriggerReceiveOpenMessage::getInfo()
    {
        return TriggerReceiveOpenMessage::info;
    }

    TriggerReceiveOpenMessage::TriggerReceiveOpenMessage(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerReceiveOpenMessage::getType() const
    {
        return TriggerReceiveOpenMessage::TYPE;
    }

    TriggerReceiveOpenMessage::~TriggerReceiveOpenMessage()
    {
    }

    std::vector<ParameterDefinition*>* TriggerReceiveOpenMessage::getParameterDefinitions()
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
                                    "If checked, all the messages will be processed including messages received during action(s) execution. Useful if you want to process ALL incoming messages. If checked, you should not combine with other triggers or you may miss messages."
                                            + RichText::newline()
                                            + "If unchecked, only the messages that arrive while this trigger is executing will push the trigger. Useful if you need to be modified if a new message arrives such as a message delivery notification. It is safe to combine with other triggers.")));
        }
        return parameterDefinitions;
    }

    std::string TriggerReceiveOpenMessage::getDefaultParameter(std::string key)
    {
        if (key == "ACCOUNT_KEY") {
            return SSTR(MessageService::accountKeyAll);
        } else if (key == "ENABLE_QUEUE") {
            return "TRUE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::vector<statement_output_variable_definition*>* TriggerReceiveOpenMessage::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
//    vars->push_back(new statement_output_variable_definition("MSG_CONVO_ID", "Conversation identifier, can be used for replies.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_ACCOUNT_ID",
                        "Account identifier, can be used for replies.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_TIMESTAMP_DEVICE",
                        "Timestamp when message was received on device", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_TIMESTAMP_SERVER",
                        "Timestamp when message was delivered", "STRING"));

        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_RECIPIENT_COUNT",
                        "Recipient count", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_RECIPIENT_From_1_NAME",
                        "Sender's recipient name.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_RECIPIENT_{TYPE}_{NUM}_NAME",
                        "Recipient name. TYPE corresponds to one of To, Cc, Bcc, From, or ReplyTo. NUM is the recipient number for each TYPE category starting with 1.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_RECIPIENT_From_1_ADDRESS",
                        "Sender's recipient address.", "STRING"));

        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_RECIPIENT_{TYPE}_{NUM}_ADDRESS",
                        "Recipient address. TYPE corresponds to one of To, Cc, Bcc, From, or ReplyTo. NUM is the recipient number for each TYPE category starting with 1.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_SUBJECT", "Message subject",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_BODY",
                        "Message body in plain text.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_ATTACHMENT_COUNT",
                        "Number of attachments", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_ATTACHMENT_{NUM}_MIME",
                        "Attachment MIME type. NUM is the attachment number starting with 1.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_ATTACHMENT_{NUM}_NAME",
                        "Attachment name. NUM is the attachment number starting with 1.",
                        "STRING"));
//    vars->push_back(new VariableDefinition("MSG_RECV_ATTACHMENT_{NUM}_PATH", "Attachment location on device. Replace NUM with the attachment number.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_MIME", "Message MIME type",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("MSG_RECV_IS_PRIORITY",
                        "Is marked as priority inbox", "BOOL"));
        return vars;
    }

    bool TriggerReceiveOpenMessage::isActive(ExecutionState* state)
    {
        return trigger(state, false);
    }

    void TriggerReceiveOpenMessage::blockUntilActive(ExecutionState* state)
    {
        trigger(state, true);
    }

    bool TriggerReceiveOpenMessage::trigger(ExecutionState* state, bool block)
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

        if (block) {
            // Block until message is available
            do {
                nextMsg = messageService->getNextMessage();
                state->giveUpIfNotRunning();
            } while (!nextMsg);
        } else {
            // Check if there are new messages without waiting
            nextMsg = messageService->getNextMessage(0);
        }

        if (!isValueTrue(getParameter("ENABLE_QUEUE", state))) {
            // If queue is not enabled, stop receiving any new messages
            messageService->disconnectMsgRecv();
        }

        // Should only be null in case of non-blocking
        if (!nextMsg) {
            return false;
        }

        // TODO Ability to reply and check folders
//    nextMsg->conversationId();
//    nextMsg->folderId();

//    state->setUserProperty("MSG_CONVO_ID", SSTR(nextMsg->conversationId().toStdString()));
        state->setUserProperty("MSG_ACCOUNT_ID", SSTR(nextMsg->accountId()));
        state->setUserProperty("MSG_RECV_TIMESTAMP_DEVICE",
                nextMsg->deviceTimestamp().toString().toStdString());
        state->setUserProperty("MSG_RECV_TIMESTAMP_SERVER",
                nextMsg->serverTimestamp().toString().toStdString());

        // Get contacts
        std::list<bb::pim::message::MessageContact> contacts;
        short to = 0, cc = 0, bcc = 0, from = 0, replyto = 0, unknown = 0;
        int contactsSize = 0;

        // Get contacts from conversation
        bb::pim::message::MessageService bbMessageService;
        bb::pim::message::Conversation convo = bbMessageService.conversation(nextMsg->accountId(),
                nextMsg->conversationId());
        QList<bb::pim::message::MessageContact> convoRecipients = convo.participants();
        for (int i = 0; i < convoRecipients.size(); ++i) {
            LOG->trace(
                    SSTR(
                            "Incoming message contact in convo: "
                                    << convoRecipients.at(i).displayableName().toStdString()));
            if (nextMsg->mimeType() == bb::pim::message::MimeTypes::Sms
                    || nextMsg->mimeType() == bb::pim::message::MimeTypes::Mms) {
                // For SMS/MMS, FROM contact is embedded in convo (But shows up as a TO contact)
                from++;
                contactsSize++;
                state->setUserProperty(SSTR("MSG_RECV_RECIPIENT_From_" << from << "_NAME"),
                        convoRecipients.at(i).name().toStdString());
                state->setUserProperty(SSTR("MSG_RECV_RECIPIENT_From_" << from << "_ADDRESS"),
                        convoRecipients.at(i).address().toStdString());
            } else {
                contacts.push_back(convoRecipients.at(i));
            }
        }
        // Get contacts from message (Superseded by convo recipients)
//        for (int i = 0; i < nextMsg->recipientCount(); ++i) {
//            LOG->trace(SSTR("Incoming message contact in msg: " << nextMsg->recipientAt(i).displayableName().toStdString()));
//            contacts.push_back(nextMsg->recipientAt(i));
//        }

        contactsSize += contacts.size();
        state->setUserProperty("MSG_RECV_RECIPIENT_COUNT", SSTR(contactsSize));

        for (std::list<bb::pim::message::MessageContact>::iterator it = contacts.begin();
                it != contacts.end(); ++it) {
            std::string typeStr;
            switch ((*it).type()) {
                case bb::pim::message::MessageContact::To:
                    typeStr = SSTR("To_" << ++to);
                    break;
                case bb::pim::message::MessageContact::Cc:
                    typeStr = SSTR("Cc_" << ++cc);
                    break;
                case bb::pim::message::MessageContact::Bcc:
                    typeStr = SSTR("Bcc_" << ++bcc);
                    break;
                case bb::pim::message::MessageContact::From:
                    typeStr = SSTR("From_" << ++from);
                    break;
                case bb::pim::message::MessageContact::ReplyTo:
                    typeStr = SSTR("ReplyTo_" << ++replyto);
                    break;
                default:
                    typeStr = SSTR("Unknown_" << ++unknown);
                    break;
            }
            state->setUserProperty(SSTR("MSG_RECV_RECIPIENT_" << typeStr << "_NAME"),
                    (*it).name().toStdString());
            state->setUserProperty(SSTR("MSG_RECV_RECIPIENT_" << typeStr << "_ADDRESS"),
                    (*it).address().toStdString());
        }

        state->setUserProperty("MSG_RECV_SUBJECT", nextMsg->subject().toStdString());
        // Combine both plaintext and html since one of them will be empty
        state->setUserProperty("MSG_RECV_BODY",
                nextMsg->body(bb::pim::message::MessageBody::PlainText).plainText().toStdString()
                        + nextMsg->body(bb::pim::message::MessageBody::Html).plainText().toStdString());
        state->setUserProperty("MSG_RECV_ATTACHMENT_COUNT", SSTR(nextMsg->attachmentCount()));
        for (int i = 0; i < nextMsg->attachmentCount(); ++i) {
            state->setUserProperty(SSTR("MSG_RECV_ATTACHMENT_" << i << "_MIME"),
                    nextMsg->attachmentAt(i).mimeType().toStdString());
            state->setUserProperty(SSTR("MSG_RECV_ATTACHMENT_" << i << "_NAME"),
                    nextMsg->attachmentAt(i).name().toStdString());
//        state->setUserProperty(SSTR("MSG_RECV_ATTACHMENT_" << i << "_PATH"), nextMsg->attachmentAt(i).path().toString().toStdString());
        }
        state->setUserProperty("MSG_RECV_MIME", nextMsg->mimeType().toStdString());
        state->setUserProperty("MSG_RECV_IS_PRIORITY",
                nextMsg->isPriorityInbox() ? "TRUE" : "FALSE");

        return true;
    }

    bool TriggerReceiveOpenMessage::isEventOnlyTrigger()
    {
        return !isValueTrue(getParameter("ENABLE_QUEUE"));
    }

} /* namespace actiontrigger */
