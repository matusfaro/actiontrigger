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

#include "actiontrigger/controller/services/phone/phoneservice.hpp"

namespace actiontrigger
{

    Logger* PhoneService::LOG = LoggerFactory::getInstance("PhoneService");

    PhoneService::PhoneService() :
            isConnected(false)
    {
        connectReceiver();
    }

    void PhoneService::connectReceiver()
    {
        LOG->trace("connectReceiver");
        if (!isConnected) {
            connect(&phone, SIGNAL(callUpdated(const bb::system::phone::Call&)), this,
                    SLOT(callUpdated(const bb::system::phone::Call&)));
            connect(&phone, SIGNAL(callUpdated(const bb::system::phone::Call&)), &receiveWait,
                    SLOT(quit()));
            isConnected = true;
        }
    }

    void PhoneService::disconnectReceiver()
    {
        LOG->trace("disconnectReceiver");
        // Disconnect
        if (isConnected) {
            disconnect(&phone, SIGNAL(callUpdated(const bb::system::phone::Call&)), this,
                    SLOT(callUpdated(const bb::system::phone::Call&)));
            disconnect(&phone, SIGNAL(callUpdated(const bb::system::phone::Call&)), &receiveWait,
                    SLOT(quit()));
            isConnected = false;
            // Clear queue
            std::queue<const bb::system::phone::Call*> emptyQueue;
            std::swap(receiveQueue, emptyQueue);
        }
    }

    const bb::system::phone::Call* PhoneService::getNext(long timeout_ms)
    {
        LOG->trace("getNext");
        connectReceiver();
        const bb::system::phone::Call* nextUpdate = NULL;
        do {
            if (!receiveQueue.empty()) {
                // Already had message in queue
                nextUpdate = receiveQueue.front();
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
                nextUpdate = receiveQueue.front();
                receiveQueue.pop();
                LOG->trace(SSTR("Received update"));
                break;
            }
            LOG->trace(SSTR("Not received update within timeout"));
        } while (false);
        return nextUpdate;
    }

    PhoneService::~PhoneService()
    {
        LOG->trace("~PhoneService");
        if (isConnected) {
            disconnectReceiver();
        }
    }

    void PhoneService::callUpdated(const bb::system::phone::Call& call)
    {
        if (!call.isValid())
            return;

        // Push onto queue
        receiveQueue.push(new bb::system::phone::Call(call));
    }

    std::string PhoneService::lineTypeToString(bb::system::phone::LineType::Type type)
    {
        switch (type) {
            case bb::system::phone::LineType::Cellular:
                return "Cellular";
            case bb::system::phone::LineType::VideoChat:
                return "VideoChat";
            case bb::system::phone::LineType::MVS:
                return "MVS";
            case bb::system::phone::LineType::Invalid:
                return "Invalid";
            default:
                return "UNKNOWN";
        }
    }

    bb::system::phone::LineType::Type PhoneService::stringToLineType(std::string type)
    {
        if (type == "CELL") {
            return bb::system::phone::LineType::Cellular;
        } else if (type == "VIDEO") {
            return bb::system::phone::LineType::VideoChat;
        } else if (type == "MVS") {
            return bb::system::phone::LineType::MVS;
        } else {
            return bb::system::phone::LineType::Invalid;
        }
    }

    std::string PhoneService::callTypeToString(bb::system::phone::CallType::Type type)
    {
        switch (type) {
            case bb::system::phone::CallType::Invalid:
                return "Invalid";
            case bb::system::phone::CallType::Incoming:
                return "Incoming";
            case bb::system::phone::CallType::Outgoing:
                return "Outgoing";
            case bb::system::phone::CallType::MultiParty:
                return "MultiParty";
            case bb::system::phone::CallType::Missed:
                return "Missed";
            case bb::system::phone::CallType::Command:
                return "Command";
            case bb::system::phone::CallType::Emergency:
                return "Emergency";
            default:
                return "Unknown";
        }
    }

    bb::system::phone::CallType::Type PhoneService::stringToCallType(std::string type)
    {
        if (type == "Invalid") {
            return bb::system::phone::CallType::Invalid;
        } else if (type == "Incoming") {
            return bb::system::phone::CallType::Incoming;
        } else if (type == "Outgoing") {
            return bb::system::phone::CallType::Outgoing;
        } else if (type == "MultiParty") {
            return bb::system::phone::CallType::MultiParty;
        } else if (type == "Missed") {
            return bb::system::phone::CallType::Missed;
        } else if (type == "Command") {
            return bb::system::phone::CallType::Command;
        } else if (type == "Emergency") {
            return bb::system::phone::CallType::Emergency;
        } else {
            return bb::system::phone::CallType::Invalid;
        }
    }

    std::string PhoneService::callStateToString(bb::system::phone::CallState::Type state)
    {
        switch (state) {
            case bb::system::phone::CallState::Invalid:
                return "Invalid";
            case bb::system::phone::CallState::Incoming:
                return "Incoming";
            case bb::system::phone::CallState::Connecting:
                return "Connecting";
            case bb::system::phone::CallState::RemoteRinging:
                return "RemoteRinging";
            case bb::system::phone::CallState::Connected:
                return "Connected";
            case bb::system::phone::CallState::Disconnected:
                return "Disconnected";
            case bb::system::phone::CallState::Failed:
                return "Failed";
            case bb::system::phone::CallState::Held:
                return "Held";
            case bb::system::phone::CallState::MultiPartyMember:
                return "MultiPartyMember";
            case bb::system::phone::CallState::Unknown:
            default:
                return "Unknown";
        }
    }

    bb::system::phone::CallState::Type PhoneService::stringToCallState(std::string state)
    {
        if (state == "Invalid") {
            return bb::system::phone::CallState::Invalid;
        } else if (state == "Incoming") {
            return bb::system::phone::CallState::Incoming;
        } else if (state == "Connecting") {
            return bb::system::phone::CallState::Connecting;
        } else if (state == "RemoteRinging") {
            return bb::system::phone::CallState::RemoteRinging;
        } else if (state == "Connected") {
            return bb::system::phone::CallState::Connected;
        } else if (state == "Disconnected") {
            return bb::system::phone::CallState::Disconnected;
        } else if (state == "Failed") {
            return bb::system::phone::CallState::Failed;
        } else if (state == "Held") {
            return bb::system::phone::CallState::Held;
        } else if (state == "MultiPartyMember") {
            return bb::system::phone::CallState::MultiPartyMember;
        } else {
            return bb::system::phone::CallState::Unknown;
        }
    }

} /* namespace actiontrigger */
