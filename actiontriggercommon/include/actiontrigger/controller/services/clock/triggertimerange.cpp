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

#include "actiontrigger/controller/services/clock/triggertimerange.hpp"

namespace actiontrigger
{

    std::vector<ParameterDefinition*>* TriggerTimeRange::parameterDefinitions = NULL;
    std::string TriggerTimeRange::TYPE = "TRIGGER_TIME_RANGE";
    Logger* TriggerTimeRange::StatementLOG = LoggerFactory::getInstance("TriggerTimeRange");
    const StatementInfo TriggerTimeRange::info(true, "Time range", TriggerTimeRange::TYPE,
            "Triggers only during specific time range with second precision. Keep in mind a time range may spill over to the next day. For example when a start time is set to 22:00:00 on Monday with a 4:00:00 duration, time range will spill over to Tuesday until 2:00:00.");

    const StatementInfo TriggerTimeRange::getInfo()
    {
        return TriggerTimeRange::info;
    }

    TriggerTimeRange::TriggerTimeRange(StatementModel* model) :
            Trigger(model)
    {
        StatementLOG->trace("ConsoleOut(model,childAction)");
    }

    std::string TriggerTimeRange::getType() const
    {
        return TriggerTimeRange::TYPE;
    }

    TriggerTimeRange::~TriggerTimeRange()
    {
    }

    std::vector<ParameterDefinition*>* TriggerTimeRange::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getTime("TIME_START", "Start time",
                            "Start time of time interval"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getTime("DURATION", "Duration",
                            "Duration of time interval"));
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

    std::string TriggerTimeRange::getDefaultParameter(std::string key)
    {
        if (key == "TIME_START") {
            return "00:00:00";
        } else if (key == "DURATION") {
            return "00:05:00";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    int TriggerTimeRange::getTotalSeconds(QTime time)
    {
        return (time.hour() * 3600 + time.minute() * 60 + time.second());
    }

    bool TriggerTimeRange::isEventOnlyTrigger()
    {
        return false;
    }

    bool TriggerTimeRange::checkParameterValidity(ExecutionState* state)
    {
        if (!Statement::checkParameterValidity(state))
            return false;
        LOG->trace("checkParameterValidity()");
        // Ensure start time is not empty
        if (getParameter("TIME_START").empty()) {
            if (state != NULL)
                state->getLogger()->error("Start time cannot be empty.");
            return false;
        }
        // Ensure duration is not empty
        if (getParameter("DURATION").empty()) {
            if (state != NULL)
                state->getLogger()->error("Duration cannot be empty.");
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

    bool TriggerTimeRange::isActive(ExecutionState* state)
    {
        QTime startTime = QTime::fromString(
                QString::fromStdString(getParameter("TIME_START", state)));
        if (!startTime.isValid()) {
            state->getLogger()->error("Cannot parse start time, aborting");
            throw ExecutionAbortedException();
        }
        QTime duration = QTime::fromString(QString::fromStdString(getParameter("DURATION", state)));
        if (!duration.isValid()) {
            state->getLogger()->error("Cannot parse duration, aborting");
            throw ExecutionAbortedException();
        }
        QDateTime now = QDateTime::currentDateTime();

        return isActive(startTime, duration, now, state);
    }

    void TriggerTimeRange::blockUntilActive(ExecutionState* state)
    {
        QTime startTime = QTime::fromString(
                QString::fromStdString(getParameter("TIME_START", state)));
        if (!startTime.isValid()) {
            state->getLogger()->error("Cannot parse start time, aborting");
            throw ExecutionAbortedException();
        }
        QTime duration = QTime::fromString(QString::fromStdString(getParameter("DURATION", state)));
        if (!duration.isValid()) {
            state->getLogger()->error("Cannot parse duration, aborting");
            throw ExecutionAbortedException();
        }
        QDateTime now = QDateTime::currentDateTime();

        // Check if already active
        if (isActive(startTime, duration, now, state)) {
            return;
        }
        // Wait until active
        blockUntilActive(startTime, now, state);
        return;
    }

    bool TriggerTimeRange::isActive(QTime startTime, QTime duration, QDateTime now,
            ExecutionState* state)
    {
        int difference = startTime.secsTo(now.time());
        if (difference < 0) {
            // start time is ahead of us today, let's check if previous date hasn't spilled over to cover current time
            if (isValueTrue(
                    getParameter(now.date().addDays(-1).toString("dddd").toStdString(), state))
                    && (difference + 24 * 60 * 60 <= getTotalSeconds(duration))) {
                return true;
            }
        } else {
            // We already passed start time, let's check if duration is long enough to cover current time
            if (isValueTrue(getParameter(now.date().toString("dddd").toStdString(), state))
                    && (difference <= getTotalSeconds(duration))) {
                return true;
            }
        }
        return false;
    }

// Does not check spill overs from previous day. This should have already been checked with isActive()
    void TriggerTimeRange::blockUntilActive(QTime startTime, QDateTime now, ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        int secsToAlarm = now.time().secsTo(startTime);
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
