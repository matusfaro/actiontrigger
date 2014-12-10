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

#include "actiontrigger/controller/services/invocation/invocation.hpp"

namespace actiontrigger
{

    Logger* Invocation::LOG = LoggerFactory::getInstance("Invocation");

    Invocation::Invocation() :
            queryResults(NULL), targetResult(NULL), msg(NULL)
    {
        LOG->trace("Invocation");
        // Setting up callbacks
        connect(&invokeManager, SIGNAL(childCardDone(const bb::system::CardDoneMessage &)), this,
                SLOT(receiveData(const bb::system::CardDoneMessage&)));
    }

    Invocation::~Invocation()
    {
        LOG->trace("~Invocation");
    }

    bb::system::InvokeQueryTargetsReply* Invocation::query(std::string action, std::string mime,
            std::string uri, long timeout)
    {
        LOG->trace("query");
        bb::system::InvokeQueryTargetsRequest request;
        if (!action.empty())
            request.setAction(QString::fromStdString(action));
        if (!mime.empty())
            request.setMimeType(QString::fromStdString(mime));
        if (!uri.empty())
            request.setUri(QUrl(QString::fromStdString(uri)));
        queryResults = invokeManager.queryTargets(request);
        do {
            if (!queryResults) {
                LOG->warning("Could not query targets: Invoke message could not be sent");
                break;
            }
            SlotWait wait;
            connect(queryResults, SIGNAL(finished()), &wait, SLOT(quit()));
            connect(queryResults, SIGNAL(finished()), this, SLOT(targetInvokeFinished()));
            if (!wait.wait(timeout)) {
                LOG->warning("Could not query targets: timed out");
                break;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, static_cast<int>(timeout));
            if (!queryResults->isFinished()) {
                LOG->warning(
                        "Could not query targets: " + errorMessageToString(queryResults->error()));
                break;
            }
        } while (false);
        return queryResults;
    }

    bb::system::InvokeTargetReply* Invocation::invoke(std::string target, std::string action,
            std::string mime, std::string url, std::string data, long timeout)
    {
        return invoke(target, action, mime, url, QString::fromStdString(data).toUtf8(), timeout);
    }

    bb::system::InvokeTargetReply* Invocation::invoke(std::string target, std::string action,
            std::string mime, std::string url, QByteArray data, long timeout)
    {
        LOG->trace("invoke");
        msg = NULL;
        bb::system::InvokeRequest request;
        if (!target.empty())
            request.setTarget(QString::fromStdString(target));
        if (!action.empty())
            request.setAction(QString::fromStdString(action));
        if (!mime.empty())
            request.setMimeType(QString::fromStdString(mime));
        if (!url.empty())
            request.setUri(QUrl(QString::fromStdString(url)));
        if (data.size() == 0)
            request.setData(data);
        targetResult = invokeManager.invoke(request);
        do {
            if (!targetResult) {
                LOG->warning("Could not invoke targets: Invoke message could not be sent");
                break;
            }
            SlotWait wait;
            connect(targetResult, SIGNAL(finished()), &wait, SLOT(quit()));
            connect(targetResult, SIGNAL(finished()), this, SLOT(targetInvokeFinished()));
            if (!wait.wait(timeout)) {
                LOG->warning("Could not invoke targets: timed out");
                break;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, static_cast<int>(timeout));
            if (!targetResult->isFinished()) {
                LOG->warning(
                        "Could not invoke targets: " + errorMessageToString(targetResult->error()));
                break;
            }
        } while (false);
        return targetResult;
    }

    bb::system::CardDoneMessage* Invocation::getCardResponse(long timeout)
    {
        LOG->trace("getCardResponse");
        do {
            if (msg)
                break;
            SlotWait wait;
            connect(&invokeManager, SIGNAL(childCardDone(const bb::system::CardDoneMessage&)),
                    &wait, SLOT(quit()));
            if (!wait.wait(timeout)) {
                LOG->warning("Could not retrieve card response: timed out");
                break;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents, static_cast<int>(timeout));
            if (msg == NULL) {
                LOG->warning("Could not retrieve card response: timed out");
                break;
            }
        } while (false);
        return msg;
    }

    void Invocation::queryInvokeFinished()
    {
        LOG->trace("queryInvokeFinished");
        queryResults = qobject_cast<bb::system::InvokeQueryTargetsReply*>(sender());
    }

    void Invocation::targetInvokeFinished()
    {
        LOG->trace("targetInvokeFinished");
        targetResult = qobject_cast<bb::system::InvokeTargetReply*>(sender());
    }

    void Invocation::receiveData(const bb::system::CardDoneMessage& msg)
    {
        LOG->trace("receiveData");
        this->msg = new bb::system::CardDoneMessage(msg);
    }

    std::string Invocation::errorMessageToString(bb::system::InvokeReplyError::Type error)
    {
        LOG->trace("errorMessageToString");
        switch (error) {
            case bb::system::InvokeReplyError::None:
                return "Invoke Succeeded";
            case bb::system::InvokeReplyError::BadRequest:
                return "Reply error: The request could not be parsed.";
            case bb::system::InvokeReplyError::Internal:
                return "Reply error: The invocation service failed as a result of an internal error.";
            case bb::system::InvokeReplyError::NoTarget:
                return "Reply error: No target was found.";
            case bb::system::InvokeReplyError::TargetNotOwned:
                return "Reply error: An attempt to set or retrieve a target's filter list failed because the requesting entity does not own the target.";
            case bb::system::InvokeReplyError::InsufficientPrivileges:
                return "Reply error: A request failed because the requesting entity lacks sufficient privileges.";
            case bb::system::InvokeReplyError::Target:
                return "Reply error: The target failed or otherwise misbehaved during invocation, causing the request to fail.";
            default:
                return "Invoke Succeeded";
        }
    }

} /* namespace actiontrigger */
