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

#ifndef MESSAGESERVICE_HPP_
#define MESSAGESERVICE_HPP_

#include <QObject>
#include <QRegExp>
#include <bb/pim/message/MimeTypes>
#include <bb/pim/message/Attachment>
#include <bb/pim/message/MessageBody>
#include <bb/pim/message/MessageContact>
#include <bb/pim/message/MessageBuilder>
#include <bb/pim/message/MessageService>
#include <bb/pim/message/Message>
#include <bb/pim/message/MessageContact>
#include <bb/pim/message/ConversationBuilder>
#include <bb/pim/message/Conversation>
#include <bb/pim/message/Attachment>
#include <bb/pim/account/AccountService>
#include <bb/pim/account/Account>
#include <bb/pim/account/Service>
#include <bb/pim/message/Conversation>
#include <bb/pim/account/Provider>
#include <QByteArray>
#include <QList>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/tools/slotwait.hpp"
#include <queue>

#define ACCOUNT_KEY_ALL -1
#define RECEIVE_MESSAGES_DEFAULT_TIMEOUT 5000

namespace actiontrigger
{

    typedef enum
    {
        SEND_MESSAGE_SUCCESS = 0, SEND_MESSAGE_INVALID_RECIPIENT = 1
    } send_message_response;

    class MessageService: public QObject
    {
    Q_OBJECT

    public:
        static const bb::pim::account::AccountKey accountKeyAll;
        static QList<bb::pim::account::Account> listAccounts();
        static std::string recipientType(bb::pim::message::MessageContact::Type type);

        MessageService(bb::pim::account::AccountKey accountKey = ACCOUNT_KEY_ALL);
        virtual ~MessageService();

        /**
         * Stops receiving all messages and clears the current queue.
         */
        void disconnectMsgRecv();

        /**
         * Gets next message in queue or waits for a message within timeout.
         * If you have disconnected message receive, this will connect it again automatically.
         * If timeout is zero, does not wait for new messages.
         */
        bb::pim::message::Message* getNextMessage(
                long timeout_ms = RECEIVE_MESSAGES_DEFAULT_TIMEOUT);

        /**
         * Sends message.
         */
        send_message_response sendMessage(std::string recipient, std::string subject,
                std::string content, bb::pim::message::ConversationKey conversationKey = "");

    public slots:
        void receive(bb::pim::account::AccountKey accountKey,
                bb::pim::message::ConversationKey convoKey,
                bb::pim::message::MessageKey messageKey);
        void receiveAll(bb::pim::account::AccountKey accountKey,
                QList<bb::pim::message::ConversationKey> convoKeys,
                QList<bb::pim::message::MessageKey> messageKeys);
        void bodyDownloaded(bb::pim::account::AccountKey accountKey,
                bb::pim::message::MessageKey messageKey);

    private:
        bb::pim::account::AccountKey accountKey;

        std::queue<bb::pim::message::Message> receiveQueue;
        SlotWait receiveWait;

        static Logger* LOG;
        bool isReceiveConnected;
        bb::pim::message::MessageService* messageService;

        /**
         * Starts receiving messages after a call to disconnect().
         * This is called automatically in the constructor and getNextMessage.
         */
        void connectMsgRecv();
    };

} /* namespace actiontrigger */
#endif /* MESSAGESERVICE_HPP_ */
