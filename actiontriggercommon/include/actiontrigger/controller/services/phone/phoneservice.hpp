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

#ifndef PhoneService_HPP_
#define PhoneService_HPP_

#include <QObject>
#include <bb/system/phone/Phone>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/tools/slotwait.hpp"
#include <queue>

#define RECEIVE_NEXT_DEFAULT_TIMEOUT 5000

namespace actiontrigger
{

    class PhoneService: public QObject
    {
    Q_OBJECT

    public:
        static std::string lineTypeToString(bb::system::phone::LineType::Type type);
        static bb::system::phone::LineType::Type stringToLineType(std::string type);
        static std::string callTypeToString(bb::system::phone::CallType::Type type);
        static bb::system::phone::CallType::Type stringToCallType(std::string type);
        static std::string callStateToString(bb::system::phone::CallState::Type state);
        static bb::system::phone::CallState::Type stringToCallState(std::string state);

        PhoneService();
        virtual ~PhoneService();

        /**
         * Stops receiving all messages and clears the current queue.
         */
        void disconnectReceiver();

        /**
         * Gets next update in queue or timeout.
         * If you have disconnected receive, this will connect it again automatically.
         * If timeout is zero, does not wait for new updates.
         */
        const bb::system::phone::Call* getNext(long timeout_ms = RECEIVE_NEXT_DEFAULT_TIMEOUT);

    public slots:
        void callUpdated(const bb::system::phone::Call& call);

    private:
        std::queue<const bb::system::phone::Call*> receiveQueue;
        SlotWait receiveWait;

        static Logger* LOG;
        bool isConnected;
        bb::system::phone::Phone phone;

        /**
         * Starts receiving messages after a call to disconnect().
         * This is called automatically in the constructor and getNextMessage.
         */
        void connectReceiver();
    };

} /* namespace actiontrigger */
#endif /* PhoneService_HPP_ */
