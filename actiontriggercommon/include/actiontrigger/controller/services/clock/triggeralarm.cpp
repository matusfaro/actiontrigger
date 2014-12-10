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

#include "actiontrigger/controller/services/clock/triggeralarm.hpp"

namespace actiontrigger
{

    std::vector<ParameterDefinition*>* TriggerAlarm::parameterDefinitions = NULL;
    std::string TriggerAlarm::TYPE = "TRIGGER_ALARM";
    Logger* TriggerAlarm::StatementLOG = LoggerFactory::getInstance("TriggerAlarm");
    const StatementInfo TriggerAlarm::info(true, "Alarm clock", TriggerAlarm::TYPE,
            "Trigger at a specified date and time with second precision.");

    const StatementInfo TriggerAlarm::getInfo()
    {
        return TriggerAlarm::info;
    }

    TriggerAlarm::TriggerAlarm(StatementModel* model) :
            Trigger(model)
    {
        StatementLOG->trace("ConsoleOut(model,childAction)");
    }

    std::string TriggerAlarm::getType() const
    {
        return TriggerAlarm::TYPE;
    }

    TriggerAlarm::~TriggerAlarm()
    {
    }

    std::vector<ParameterDefinition*>* TriggerAlarm::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getTime("TIME", "Alarm",
                            "Time at which alarm will trigger every day"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Monday", "Monday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Tuesday", "Tuesday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Wednesday", "Wednesday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Thursday", "Thursday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Friday", "Friday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Saturday", "Saturday", ""));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("Sunday", "Sunday", ""));
        }
        return parameterDefinitions;
    }

    std::string TriggerAlarm::getDefaultParameter(std::string key)
    {
        if (key == "TIME") {
            return "00:00:00";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    bool TriggerAlarm::isActive(ExecutionState* state)
    {
        (void) state;
        return false;
    }

    bool TriggerAlarm::isEventOnlyTrigger()
    {
        return true;
    }

    bool TriggerAlarm::checkParameterValidity(ExecutionState* state)
    {
        if (!Statement::checkParameterValidity(state))
            return false;
        LOG->trace("checkParameterValidity()");
        // Ensure alarm time is not empty
        if (getParameter("TIME").empty()) {
            if (state != NULL)
                state->getLogger()->error("Alarm time cannot be empty.");
            return false;
        }
        // Ensure at least one day is checked
        if (getParameter("Monday").empty() && getParameter("Tuesday").empty()
                && getParameter("Wednesday").empty() && getParameter("Thursday").empty()
                && getParameter("Friday").empty() && getParameter("Saturday").empty()
                && getParameter("Sunday").empty()) {
            if (state != NULL)
                state->getLogger()->error("At least one day needs to be selected.");
            return false;
        }
        return true;
    }

    void TriggerAlarm::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        QTime alarmTime = QTime::fromString(QString::fromStdString(getParameter("TIME", state)));
        if (!alarmTime.isValid()) {
            USERLOG->error("Cannot parse alarm time, aborting");
            throw ExecutionAbortedException();
        }
        QDateTime now = QDateTime::currentDateTime();
        int secsToAlarm = now.time().secsTo(alarmTime);
        int alarmDayCount = 0;
        // If the alarm already passed today or is very close to now, push alarm for tomorrow
        QDate alarmDate = now.date();
        if (secsToAlarm < 2) {
            alarmDate = alarmDate.addDays(1);
            secsToAlarm += ONE_DAY_IN_SEC;
            alarmDayCount++;
        }
        // Continuously loop over all the days to determine how long to sleep for.
        while (!isValueTrue(getParameter(alarmDate.toString("dddd").toStdString(), state))) {
            // Break out of infinite loop in case no recurring days were set
            if (alarmDayCount == 7) {
                USERLOG->error("No days are selected, aborting");
                throw ExecutionAbortedException();
            }
            alarmDate = alarmDate.addDays(1);
            secsToAlarm += ONE_DAY_IN_SEC;
            alarmDayCount++;
        }
        LOG->trace(SSTR("secsToAlarm: " << secsToAlarm));
        int seconds = secsToAlarm % 60;
        int minutes = (secsToAlarm / 60) % 60;
        int hours = secsToAlarm / 3600;
        USERLOG->debug(
                SSTR(
                        "Will go off in " << hours << ":" << std::setfill('0') << std::setw(2)
                                << minutes << ":" << std::setfill('0') << std::setw(2) << seconds));
        if (state->getRuntimeResources()->sleepSafe(((unsigned long) secsToAlarm * 1000))) {
            LOG->trace("Forcefully woken up");
        }

        return;
    }

} /* namespace actiontrigger */
