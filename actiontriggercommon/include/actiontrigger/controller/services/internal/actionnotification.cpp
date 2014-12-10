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

#include "actiontrigger/controller/services/internal/actionnotification.hpp"

namespace actiontrigger
{

    std::string ActionNotification::TYPE = "ACTION_ActionNotification";

    std::vector<ParameterDefinition*>* ActionNotification::parameterDefinitions = NULL;
    Logger* ActionNotification::StatementLOG = LoggerFactory::getInstance("ActionNotification");
    const StatementInfo ActionNotification::info(false, "Notification", ActionNotification::TYPE,
            "Sends a custom application notification.");

    const StatementInfo ActionNotification::getInfo()
    {
        return ActionNotification::info;
    }

    ActionNotification::ActionNotification(StatementModel* model) :
            Action(model)
    {
    }

    ActionNotification::~ActionNotification()
    {
    }

    std::vector<ParameterDefinition*>* ActionNotification::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("TITLE", "Title", "Notification title"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("BODY", "Body", "Notification body"));
        }
        return parameterDefinitions;
    }

    std::string ActionNotification::getDefaultParameter(std::string key)
    {
        if (key == "TITLE") {
            return "Hello";
        } else if (key == "BODY") {
            return "How are you?";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionNotification::getType() const
    {
        return ActionNotification::TYPE;
    }

    void ActionNotification::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        bb::platform::Notification* notification = new bb::platform::Notification();
        notification->setTitle(QString::fromStdString(getParameter("TITLE", state)));
        notification->setBody(QString::fromStdString(getParameter("BODY", state)));

        bb::system::InvokeRequest invokeRequest;
        invokeRequest.setTarget("com.actiontrigger");

        notification->setInvokeRequest(invokeRequest);
        switch (notification->notify()) {
            case bb::platform::NotificationError::None:
                RUNLOG->debug("Notification response: success");
                break;
            case bb::platform::NotificationError::Unknown:
                RUNLOG->error("Notification response: An unknown error has occurred");
                break;
            case bb::platform::NotificationError::InvalidRequest:
                RUNLOG->error("Notification response: Cannot interpret notification request");
                break;
            case bb::platform::NotificationError::NoButtons:
                RUNLOG->error("Notification response: Notification buttons are not included");
                break;
            case bb::platform::NotificationError::ServiceUnavailable:
                RUNLOG->error("Notification response: Notification service is unavailable");
                break;
        }
    }

} /* namespace actiontrigger */
