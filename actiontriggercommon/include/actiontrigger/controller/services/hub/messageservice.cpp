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

#include "actiontrigger/controller/services/hub/messageservice.hpp"

namespace actiontrigger
{

    const bb::pim::account::AccountKey MessageService::accountKeyAll = -1;
    Logger* MessageService::LOG = LoggerFactory::getInstance("MessageService");

    QList<bb::pim::account::Account> MessageService::listAccounts()
    {
        bb::pim::account::AccountService accountService;
        return accountService.accounts(bb::pim::account::Service::Messages);
    }

    MessageService::MessageService(bb::pim::account::AccountKey accountKey) :
            accountKey(accountKey), isReceiveConnected(false), messageService(
                    new bb::pim::message::MessageService((QObject*) NULL))
    {
        connectMsgRecv();
    }

    void MessageService::connectMsgRecv()
    {
        LOG->trace("connectMsgRecv");
        if (!isReceiveConnected) {
            connect(messageService,
                    SIGNAL(
                            messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)),
                    this,
                    SLOT(
                            receive(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)));
            connect(messageService,
                    SIGNAL(
                            messagesAdded(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)),
                    this,
                    SLOT(
                            receiveAll(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)));
            connect(messageService,
                    SIGNAL(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)),
                    this,
                    SLOT(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)));
            connect(messageService,
                    SIGNAL(
                            messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)),
                    &receiveWait, SLOT(quit()));
            connect(messageService,
                    SIGNAL(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)),
                    &receiveWait, SLOT(quit()));
            connect(messageService,
                    SIGNAL(
                            messagesAdded(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)),
                    &receiveWait, SLOT(quit()));
            isReceiveConnected = true;
        }
    }

    void MessageService::disconnectMsgRecv()
    {
        LOG->trace("disconnectMsgRecv");
        // Disconnect
        if (isReceiveConnected) {
            disconnect(messageService,
                    SIGNAL(
                            messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)),
                    this,
                    SLOT(
                            receive(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)));
            disconnect(messageService,
                    SIGNAL(
                            messagesAdded(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)),
                    this,
                    SLOT(
                            receiveAll(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)));
            disconnect(messageService,
                    SIGNAL(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)),
                    this,
                    SLOT(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)));
            disconnect(messageService,
                    SIGNAL(
                            messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey)),
                    &receiveWait, SLOT(quit()));
            disconnect(messageService,
                    SIGNAL(
                            bodyDownloaded(bb::pim::account::AccountKey, bb::pim::message::MessageKey)),
                    &receiveWait, SLOT(quit()));
            disconnect(messageService,
                    SIGNAL(
                            messagesAdded(bb::pim::account::AccountKey, QList<bb::pim::message::ConversationKey>, QList<bb::pim::message::MessageKey>)),
                    &receiveWait, SLOT(quit()));
            isReceiveConnected = false;
            // Clear queue
            std::queue<bb::pim::message::Message> emptyQueue;
            std::swap(receiveQueue, emptyQueue);
        }
    }

    bb::pim::message::Message* MessageService::getNextMessage(long timeout_ms)
    {
        LOG->trace("getNextMessage");
        connectMsgRecv();
        bb::pim::message::Message* nextMessage;
        do {
            if (!receiveQueue.empty()) {
                // Already had message in queue
                nextMessage = new bb::pim::message::Message(receiveQueue.front());
                receiveQueue.pop();
                break;
            }
            // Wait until message is received or timeout
            if (timeout_ms > 0)
                receiveWait.wait(timeout_ms);
            // Process events
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            if (!receiveQueue.empty()) {
                // Received next message
                nextMessage = new bb::pim::message::Message(receiveQueue.front());
                receiveQueue.pop();
                break;
            }
            // Timeout or next message was filtered (not inbound)
            nextMessage = NULL;
        } while (false);
        return nextMessage;
    }

    send_message_response MessageService::sendMessage(std::string recipient, std::string subject,
            std::string content, bb::pim::message::ConversationKey conversationKey)
    {
        LOG->trace("sendMessage");
        bb::pim::account::AccountService accountService;
        bb::pim::account::Account account = accountService.account(accountKey);

        bb::pim::message::ConversationBuilder* conversationBuilder =
                bb::pim::message::ConversationBuilder::create();
        conversationBuilder->accountId(accountKey);

        // Message builder
        bb::pim::message::MessageBuilder* messageBuilder = bb::pim::message::MessageBuilder::create(
                accountKey);

        // Create To contact if available
        if (!recipient.empty()) {
            // Adjust recipient from format: "name" <address>
            QString recipientQ = QString::fromStdString(recipient);
            recipientQ = recipientQ.replace(QRegExp("\".+\" *<(.+)>"), "\\1");
            LOG->trace(
                    SSTR(
                            "Recipient replaced '" << recipient << "' with '" << recipientQ.toStdString() << "'"));

            bb::pim::message::MessageContact rto = bb::pim::message::MessageContact(-1,
                    bb::pim::message::MessageContact::To, QString::fromStdString(recipient),
                    recipientQ);

            // Create conversation if not available
            if (conversationKey.isEmpty()) {
                QList<bb::pim::message::MessageContact> participants;
                participants.append(rto);
                conversationBuilder->participants(participants);
                bb::pim::message::Conversation conversation = *conversationBuilder;
                conversationKey = messageService->save(accountKey, conversation);
            }

            // Message To recipient
            messageBuilder->addRecipient(rto);
        }

        // Conversation key
        messageBuilder->conversationId(conversationKey);

        // Attach subject and body
        if (account.provider().id().contains("sms-mms")) {
            // SMS API requires body to be passed as attachment
            LOG->trace("Sending message as SMS");
            messageBuilder->addAttachment(
                    bb::pim::message::Attachment("text/plain", "body.txt",
                            QString::fromStdString(subject + content)));
        } else {
            LOG->trace("Sending message as regular");
            if (!subject.empty())
                messageBuilder->subject(QString::fromStdString(subject));
            messageBuilder->body(bb::pim::message::MessageBody::PlainText,
                    QString::fromStdString(content).toUtf8());
        }

        // Send message
        bb::pim::message::Message message = *messageBuilder;
        messageService->send(accountKey, message);

        return SEND_MESSAGE_SUCCESS;
    }

    MessageService::~MessageService()
    {
        LOG->trace("~MessageService");
        if (isReceiveConnected) {
            disconnectMsgRecv();
        }
        // Leak messageService
    }

    void MessageService::receive(bb::pim::account::AccountKey accountKey,
            bb::pim::message::ConversationKey convoKey, bb::pim::message::MessageKey messageKey)
    {
        (void) convoKey;

        LOG->trace(SSTR("Received message: accountKey=" << accountKey << " msgKey=" << messageKey));

        // Ensure this message is for our account of interest
        if (this->accountKey != accountKeyAll && this->accountKey != accountKey) {
            LOG->trace(SSTR("Message not of interest accountKey=" << accountKey));
            return;
        }

        // Retrieve message
        bb::pim::message::Message message = messageService->message(accountKey, messageKey);

        // Message must be valid
        if (!message.isValid()) {
            LOG->trace(
                    SSTR(
                            "Received invalid message: accountKey=" << accountKey << "  msgKey=" << messageKey));
            return;
        }

        // Message must be inbound only
        if (!message.isInbound()) {
            LOG->trace(
                    SSTR(
                            "Received non-inbound message: accountKey=" << accountKey << " msgKey=" << messageKey));
            return;
        };
        if (message.mimeType() != bb::pim::message::MimeTypes::Sms
                && message.mimeType() != bb::pim::message::MimeTypes::Mms
                && message.body(bb::pim::message::MessageBody::PlainText).plainText().isEmpty()
                && message.body(bb::pim::message::MessageBody::Html).plainText().isEmpty()) {
            // Download rest of message content
            LOG->trace(
                    SSTR(
                            "Downloading rest of message: accountKey=" << accountKey << " msgKey=" << messageKey));
            messageService->downloadMessage(accountKey, messageKey);
        } else {
            // Push onto queue
            receiveQueue.push(*new bb::pim::message::Message(message));
        }
    }

    void MessageService::receiveAll(bb::pim::account::AccountKey accountKey,
            QList<bb::pim::message::ConversationKey> convoKeys,
            QList<bb::pim::message::MessageKey> messageKeys)
    {
        for (int i = 0; i < convoKeys.size(); ++i) {
            receive(accountKey, convoKeys.at(i), messageKeys.at(i));
        }
    }

    void MessageService::bodyDownloaded(bb::pim::account::AccountKey accountKey,
            bb::pim::message::MessageKey messageKey)
    {
        // Ensure this message is for our account of interest
        if (this->accountKey != accountKeyAll && this->accountKey != accountKey) {
            return;
        }

        LOG->trace(
                SSTR(
                        "Received message body: accountKey=" << accountKey << " msgKey=" << messageKey));

        // Retrieve message
        bb::pim::message::Message message = messageService->message(accountKey, messageKey);

        // Message must be valid
        if (!message.isValid()) {
            LOG->trace(
                    SSTR(
                            "Received invalid message: accountKey=" << accountKey << "  msgKey=" << messageKey));
            return;
        }
        // Message must be inbound only
        if (!message.isInbound()) {
            LOG->trace(
                    SSTR(
                            "Received non-inbound message: accountKey=" << accountKey << " msgKey=" << messageKey));
            return;
        }

        // Push onto queue
        receiveQueue.push(*new bb::pim::message::Message(message));
    }

    std::string MessageService::recipientType(bb::pim::message::MessageContact::Type type)
    {
        switch (type) {
            case bb::pim::message::MessageContact::To:
                return "To";
            case bb::pim::message::MessageContact::Cc:
                return "Cc";
            case bb::pim::message::MessageContact::Bcc:
                return "Bcc";
            case bb::pim::message::MessageContact::From:
                return "From";
            case bb::pim::message::MessageContact::ReplyTo:
                return "ReplyTo";
            default:
                return "";
        }
    }

} /* namespace actiontrigger */
