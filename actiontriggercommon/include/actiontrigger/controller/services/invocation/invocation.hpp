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

#ifndef INVOCATION_HPP_
#define INVOCATION_HPP_

#include <bb/system/CardDoneMessage>
#include <bb/system/InvokeAction>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeReplyError>
#include <string>
#include <QObject>
#include "actiontrigger/controller/executionstate.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "unistd.h"
#include "actiontrigger/tools/slotwait.hpp"

#define INVOCATION_DEFAULT_TIMEOUT_MS 5000UL
namespace actiontrigger
{

    class Invocation: public QObject
    {
    Q_OBJECT

    public:
        Invocation();
        virtual ~Invocation();
        bb::system::InvokeQueryTargetsReply* query(std::string action, std::string mime,
                std::string uri, long timeout = INVOCATION_DEFAULT_TIMEOUT_MS);
        bb::system::InvokeTargetReply* invoke(std::string target, std::string action,
                std::string mime, std::string url, QByteArray data, long timeout =
                INVOCATION_DEFAULT_TIMEOUT_MS);
        bb::system::InvokeTargetReply* invoke(std::string target, std::string action,
                std::string mime, std::string url, std::string data = "", long timeout =
                INVOCATION_DEFAULT_TIMEOUT_MS);
        bb::system::CardDoneMessage* getCardResponse(long timeout);
        static std::string errorMessageToString(bb::system::InvokeReplyError::Type error);

    public Q_SLOTS:
        void queryInvokeFinished();
        void targetInvokeFinished();
        void receiveData(const bb::system::CardDoneMessage& msg);
    private:
        bb::system::InvokeManager invokeManager;
        bb::system::InvokeQueryTargetsReply* queryResults;
        bb::system::InvokeTargetReply* targetResult;
        bb::system::CardDoneMessage* msg;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* INVOCATION_HPP_ */
