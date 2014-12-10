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

#include "actiontrigger/controller/services/clock/actiontime.hpp"

namespace actiontrigger
{

    std::string ActionTime::TYPE = "ACTION_TIME";
    std::vector<ParameterDefinition*>* ActionTime::parameterDefinitions = NULL;
    Logger* ActionTime::StatementLOG = LoggerFactory::getInstance("ActionTime");
    const StatementInfo ActionTime::info(false, "Time", ActionTime::TYPE,
            "Retrieves current time unless you specify the input time and input time format. Optionally, the output can be custom formatted as well if the output format is specified.");

    const StatementInfo ActionTime::getInfo()
    {
        return ActionTime::info;
    }

    ActionTime::ActionTime(StatementModel* model) :
            Action(model)
    {
    }

    ActionTime::~ActionTime()
    {
    }

    std::vector<ParameterDefinition*>* ActionTime::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getLabel("",
                            "<html>The format can be constructed using the following expressions:\n"
                                    "\n<b>d</b> day as number without a leading zero (1 to 31)"
                                    "\n<b>dd</b> day as number with a leading zero (01 to 31)"
                                    "\n<b>ddd</b> abbreviated localized day name (e.g. 'Mon' to 'Sun')"
                                    "\n<b>dddd</b> long localized day name (e.g. 'Monday' to 'Qt::Sunday')"
                                    "\n<b>M</b> month as number without a leading zero (1-12)"
                                    "\n<b>MM</b> month as number with a leading zero (01-12)"
                                    "\n<b>MMM</b> abbreviated localized month name (e.g. 'Jan' to 'Dec')"
                                    "\n<b>MMMM</b> long localized month name (e.g. 'January' to 'December')"
                                    "\n<b>yy</b> year as two digit number (00-99)"
                                    "\n<b>yyyy</b> the year as four digit number"
                                    "\n<b>h</b> hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)"
                                    "\n<b>hh</b> hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)"
                                    "\n<b>m</b> minute without a leading zero (0 to 59)"
                                    "\n<b>mm</b> minute with a leading zero (00 to 59)"
                                    "\n<b>s</b> second without a leading zero (0 to 59)"
                                    "\n<b>ss</b> second with a leading zero (00 to 59)"
                                    "\n<b>z</b> milliseconds without leading zeroes (0 to 999)"
                                    "\n<b>zzz</b> milliseconds with leading zeroes (000 to 999)"
                                    "\n<b>AP</b> AP will be replaced by either AM or PM."
                                    "\n<b>ap</b> ap will be replaced by either am or pm."
                                    "</html>"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("IN_FORMAT", "Input format",
                            "Optionally, specify a datetime format to input the date and time. Use the expressions to specify the format."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("DATETIME", "Input datetime",
                            "Optionally, specify the time to be used as input instead. If left empty, current local time will be used. The input format will be used to parse the input datetime."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("OUT_FORMAT", "Output format",
                            "Optionally, specify a datetime format to ouput the date and time as custom text into the variable DATETIME_CUSTOM. Use the expressions to construct your datetime."));
        }
        return parameterDefinitions;
    }

    std::string ActionTime::getType() const
    {
        return ActionTime::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionTime::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("DATETIME_CUSTOM",
                        "Custom formatted datetime.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("DATETIME",
                        "Date and time given in standard format", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("DATE", "Date given in standard format",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("TIME", "Time given in standard format",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("EPOCH_MS",
                        "The number of milliseconds that have passed since 1970-01-01T00:00:00.000, Coordinated Universal Time",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAY",
                        "Returns the day of the month (1 to 31) of this date.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAY_OF_WEEK",
                        "Returns the weekday (1 = Monday to 7 = Sunday) for this date.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAY_OF_YEAR",
                        "Returns the day of the year (1 to 365 or 366 on leap years) for this date.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAYS_IN_MONTH",
                        "Returns the number of days in the month (28 to 31) for this date.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAYS_IN_YEAR",
                        "Returns the number of days in the year (365 or 366) for this date.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_MONTH",
                        "Returns the number corresponding to the month of the date", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("DATE_YEAR",
                        "Returns the year of this date. Negative numbers indicate years before 1 A.D. = 1 C.E., such that year -44 is 44 B.C.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_WEEK_NUMBER",
                        "Returns the week number (1 to 53)", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("DATE_IS_LEAP_YEAR",
                        "Returns true if the specified year is a leap year; otherwise returns false.",
                        "BOOLEAN"));
        vars->push_back(
                new statement_output_variable_definition("DATE_DAY_NAME",
                        "Returns the name of the weekday.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("DATE_MONTH_NAME",
                        "Returns the name of the month.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("TIME_HOURS",
                        "Returns the hour part (0 to 23) of the time.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("TIME_MINUTE",
                        "Returns the minute part (0 to 59) of the time.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("TIME_SECOND",
                        "Returns the second part (0 to 59) of the time.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("TIME_MILLISECOND",
                        "Returns the millisecond part (0 to 999) of the time.", "NUMBER"));
        return vars;
    }

    void ActionTime::execute(ExecutionState* state)
    {
        QDateTime datetime;
        std::string customDatetime = getParameter("DATETIME", state);
        if (customDatetime.empty())
            datetime = QDateTime::currentDateTime();
        else
            datetime = QDateTime::fromString(QString::fromStdString(customDatetime),
                    QString::fromStdString(getParameter("IN_FORMAT", state)));
        if (!datetime.isValid())
            state->getLogger()->error("Unable to parse current date and time.");
        std::string customFormat = getParameter("OUT_FORMAT", state);
        if (!customFormat.empty())
            state->setUserProperty("DATETIME_CUSTOM",
                    SSTR(datetime.toString(QString::fromStdString(customFormat)).toStdString()));
        state->setUserProperty("EPOCH_MS", SSTR(datetime.toMSecsSinceEpoch()));
        state->setUserProperty("DATETIME", SSTR(datetime.toString().toStdString()));
        state->setUserProperty("DATE_DAY", SSTR(datetime.date().day()));
        state->setUserProperty("DATE_DAY_OF_WEEK", SSTR(datetime.date().dayOfWeek()));
        state->setUserProperty("DATE_DAY_OF_YEAR", SSTR(datetime.date().dayOfYear()));
        state->setUserProperty("DATE_DAYS_IN_MONTH", SSTR(datetime.date().daysInMonth()));
        state->setUserProperty("DATE_DAYS_IN_YEAR", SSTR(datetime.date().daysInYear()));
        state->setUserProperty("DATE_MONTH", SSTR(datetime.date().month()));
        state->setUserProperty("DATE_YEAR", SSTR(datetime.date().year()));
        state->setUserProperty("DATE_WEEK_NUMBER", SSTR(datetime.date().weekNumber()));
        state->setUserProperty("DATE_IS_LEAP_YEAR",
                SSTR((QDate::isLeapYear(datetime.date().year()) ? "TRUE" : "FALSE")));
        state->setUserProperty("DATE_DAY_NAME",
                SSTR(QDate::longDayName(datetime.date().day()).toStdString()));
        state->setUserProperty("DATE_MONTH_NAME",
                SSTR(QDate::longMonthName(datetime.date().month()).toStdString()));
        state->setUserProperty("DATE", SSTR(datetime.date().toString().toStdString()));
        state->setUserProperty("TIME_HOURS", SSTR(datetime.time().hour()));
        state->setUserProperty("TIME_MINUTE", SSTR(datetime.time().minute()));
        state->setUserProperty("TIME_SECOND", SSTR(datetime.time().second()));
        state->setUserProperty("TIME_MILLISECOND", SSTR(datetime.time().msec()));
        state->setUserProperty("TIME", SSTR(datetime.time().toString().toStdString()));
    }

} /* namespace actiontrigger */
