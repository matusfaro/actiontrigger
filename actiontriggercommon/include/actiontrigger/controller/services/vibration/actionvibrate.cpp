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

#include "actiontrigger/controller/services/vibration/actionvibrate.hpp"

namespace actiontrigger
{

    std::string ActionVibrate::TYPE = "ACTION_VIBRATE";

    enum
    {
        INTENSITY, DURATION
    };

    std::vector<ParameterDefinition*>* ActionVibrate::parameterDefinitions = NULL;
    Logger* ActionVibrate::StatementLOG = LoggerFactory::getInstance("Vibrate");
    const StatementInfo ActionVibrate::info(false, "Vibrate", ActionVibrate::TYPE,
            "Vibrates the phone");

    const StatementInfo ActionVibrate::getInfo()
    {
        return ActionVibrate::info;
    }

    ActionVibrate::ActionVibrate(StatementModel* model) :
            Action(model)
    {
    }

    ActionVibrate::~ActionVibrate()
    {
        StatementLOG->trace("~Vibrate");
    }

    std::vector<ParameterDefinition*>* ActionVibrate::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("INTENSITY", "Intensity",
                            "The intensity at which the device should vibrate.", "1", "100", "1"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("DURATION", "Duration",
                            "The length of time the device should vibrate for in milliseconds. Use a value of zero to cancel current vibration.",
                            "0", "5000", "10"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("DURATION", "Duration",
                            "The length of time the device should vibrate for in milliseconds. Valid values are between zero and 5000 inclusive. Use a value of zero to cancel the vibration if the device is vibrating."));
        }
        return parameterDefinitions;
    }

    std::string ActionVibrate::getDefaultParameter(std::string key)
    {
        if (key == "INTENSITY") {
            return "100";
        } else if (key == "DURATION") {
            return "10";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionVibrate::getType() const
    {
        return ActionVibrate::TYPE;
    }

    void ActionVibrate::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        StatementLOG->trace("execute");
        if (BPS_SUCCESS
                == vibration_request(atoi(getParameter("INTENSITY").c_str()),
                        atoi(getParameter("DURATION").c_str()))) {
            RUNLOG->debug("Successfully requested vibration");
            StatementLOG->debug("Successfully requested vibration");
        } else if (errno != EOK) {
            RUNLOG->error(SSTR("Received error: BPS_FAILURE: " << strerror(errno)));
            StatementLOG->warning(
                    SSTR("vibration_request returned unsuccesful, errno: " << strerror(errno)));
        } else {
            RUNLOG->error("Received unknown error");
            StatementLOG->warning(
                    SSTR("vibration_request returned unsuccesful, errno: " << strerror(errno)));
        }
    }

} /* namespace actiontrigger */
