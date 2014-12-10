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

#include "actiontrigger/controller/services/system/actionbattery.hpp"

namespace actiontrigger
{

    std::string ActionBattery::TYPE = "ACTION_BATTERY";
    std::vector<ParameterDefinition*>* ActionBattery::parameterDefinitions = NULL;
    Logger* ActionBattery::StatementLOG = LoggerFactory::getInstance("ActionBattery");
    const StatementInfo ActionBattery::info(false, "Battery", ActionBattery::TYPE,
            "Retrieves device battery information.");

    const StatementInfo ActionBattery::getInfo()
    {
        return ActionBattery::info;
    }

    ActionBattery::ActionBattery(StatementModel* model) :
            Action(model)
    {
    }

    ActionBattery::~ActionBattery()
    {
    }

    std::string ActionBattery::getType() const
    {
        return ActionBattery::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionBattery::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("BATT_ALERT",
                        "Determine whether the battery alert is triggered.", "BOOL"));
        vars->push_back(
                new statement_output_variable_definition("BATT_REMAINING",
                        "Retrieve the available energy remaining in the battery.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_AVERAGE_CURRENT",
                        "Retrieve the battery average current reported in milliamperes (mA).",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_AVERAGE_POWER",
                        "Retrieve the battery average power reported in milliwatts (mW).",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_CYCLE_COUNT",
                        "Retrieve the battery cycle count.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_DESIGN_CAPACITY",
                        "Retrieve the battery design capacity.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_FULL_CAPACITY",
                        "Retrieve the battery full available capacity.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_FULL_CHARGE_CAPACITY",
                        "Retrieve the battery full charge capacity.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_BATTERY_ID",
                        "Retrieve the battery ID.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MAX_LOAD_CURRENT",
                        "Retrieve the battery maximum load current.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MAX_LOAD_TIME_TO_EMPTY",
                        "Retrieve the battery maximum load time to empty.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_NAME", "Retrieve the battery name.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("BATT_NOMINAL_AVAIL_CAPACITY",
                        "Retrieve the battery nominal available capacity.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_TEMPERATURE",
                        "Retrieve the battery temperature.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_TIME_TO_EMPTY",
                        "Retrieve the battery time to empty at constant power.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_VOLTAGE",
                        "Retrieve the battery voltage.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MAX_CHARGE_CURRENT",
                        "Retrieve the charger maximum charge current.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MAX_INPUT_CURRENT",
                        "Retrieve the charger maximum input current.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_CHARGER_NAME",
                        "Retrieve the charger name.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("BATT_DEVICE_NAME",
                        "Retrieve the device name.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("BATT_CHARGE_PERCENT",
                        "Retrieve the state of charge measured as a percentage from 0 to 100.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_HEALTH_PERCENT",
                        "Retrieve the state of health measured as a percentage from 0 to 100.",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_CURRENT",
                        "Retrieve the system charge current reported in milliamperes (mA).",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_STATE",
                        "Retrieve the system charging state. One of BATTERY_CHARGING_NOT_CHARGING, BATTERY_CHARGING_TRICKLE_CHARGING, BATTERY_CHARGING_CONSTANT_CURRENT, BATTERY_CHARGING_CONSTANT_VOLTAGE, BATTERY_CHARGING_DONE_CHARGING.",
                        "STRING"));
        vars->push_back(
                new statement_output_variable_definition("BATT_CURRENT_REAL",
                        "Retrieve the actual current drawn from the charger reported in milliamperes (mA).",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MAX_VOLTAGE",
                        "Retrieve the system maximum voltage.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_MIN_VOLTAGE",
                        "Retrieve the system minimum voltage.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_SYS_VOLTAGE",
                        "Retrieve the system voltage.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_TIME_UNTIL_DISCHARGE",
                        "Retrieve the amount of time before the battery is fully discharged (empty).",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_TIME_UNTIL_FULL_CHARGE",
                        "Retrieve the amount of time to fully charge the battery.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_API_VERSION",
                        "Retrieve the Battery API version.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("BATT_OK",
                        "Determine whether the battery is OK. If the battery is not OK, it may indicate a problem with the battery or that the battery is not ready",
                        "BOOL"));
        vars->push_back(
                new statement_output_variable_definition("BATT_PRESENT",
                        "Retrieve whether the battery is present the device.", "BOOL"));
        return vars;
    }

    void ActionBattery::execute(ExecutionState* state)
    {
        state->getRuntimeResources()->bpsInitialize();
        battery_request_events(0);
        bps_event_t* event = state->getRuntimeResources()->bpsGetEvent(battery_get_domain(),
                BATTERY_INFO);
        battery_stop_events(0);
        if (event) {
            battery_info_t * info = battery_event_get_info(event);
            state->setUserProperty("BATT_ALERT",
                    SSTR(isValueTrue(battery_info_get_battery_alert(info))));
            state->setUserProperty("BATT_REMAINING",
                    SSTR(battery_info_get_battery_available_energy(info)));
            state->setUserProperty("BATT_AVERAGE_CURRENT",
                    SSTR(battery_info_get_battery_average_current(info)));
            state->setUserProperty("BATT_AVERAGE_POWER",
                    SSTR(battery_info_get_battery_average_power(info)));
            state->setUserProperty("BATT_CYCLE_COUNT",
                    SSTR(battery_info_get_battery_cycle_count(info)));
            state->setUserProperty("BATT_DESIGN_CAPACITY",
                    SSTR(battery_info_get_battery_design_capacity(info)));
            state->setUserProperty("BATT_FULL_CAPACITY",
                    SSTR(battery_info_get_battery_full_available_capacity(info)));
            state->setUserProperty("BATT_FULL_CHARGE_CAPACITY",
                    SSTR(battery_info_get_battery_full_charge_capacity(info)));
            state->setUserProperty("BATT_BATTERY_ID", SSTR(battery_info_get_battery_id(info)));
            state->setUserProperty("BATT_MAX_LOAD_CURRENT",
                    SSTR(battery_info_get_battery_max_load_current(info)));
            state->setUserProperty("BATT_MAX_LOAD_TIME_TO_EMPTY",
                    SSTR(battery_info_get_battery_max_load_time_to_empty(info)));
            state->setUserProperty("BATT_NAME",
                    SSTR(
                            (battery_info_get_battery_name(info) == NULL ? "N/A" : battery_info_get_battery_name(info))));
            state->setUserProperty("BATT_NOMINAL_AVAIL_CAPACITY",
                    SSTR(battery_info_get_battery_nominal_available_capacity(info)));
            state->setUserProperty("BATT_TEMPERATURE",
                    SSTR(battery_info_get_battery_temperature(info)));
            state->setUserProperty("BATT_TIME_TO_EMPTY",
                    SSTR(battery_info_get_battery_time_to_empty_at_constant_power(info)));
            state->setUserProperty("BATT_VOLTAGE", SSTR(battery_info_get_battery_voltage(info)));
            state->setUserProperty("BATT_MAX_CHARGE_CURRENT",
                    SSTR(battery_info_get_charger_max_charge_current(info)));
            state->setUserProperty("BATT_MAX_INPUT_CURRENT",
                    SSTR(battery_info_get_charger_max_input_current(info)));
            state->setUserProperty("BATT_CHARGER_NAME",
                    SSTR(
                            (battery_info_get_charger_name(info) == NULL ? "N/A" : battery_info_get_charger_name(info))));
            state->setUserProperty("BATT_DEVICE_NAME",
                    SSTR(
                            (battery_info_get_device_name(info) == NULL ? "N/A" : battery_info_get_device_name(info))));
            state->setUserProperty("BATT_CHARGE_PERCENT",
                    SSTR(battery_info_get_state_of_charge(info)));
            state->setUserProperty("BATT_HEALTH_PERCENT",
                    SSTR(battery_info_get_state_of_health(info)));
            state->setUserProperty("BATT_CURRENT",
                    SSTR(battery_info_get_system_charge_current(info)));
            state->setUserProperty("BATT_STATE",
                    SSTR(chargingStateToString(battery_info_get_system_charging_state(info))));
            state->setUserProperty("BATT_CURRENT_REAL",
                    SSTR(battery_info_get_system_input_current_monitor(info)));
            state->setUserProperty("BATT_MAX_VOLTAGE",
                    SSTR(battery_info_get_system_max_voltage(info)));
            state->setUserProperty("BATT_MIN_VOLTAGE",
                    SSTR(battery_info_get_system_min_voltage(info)));
            state->setUserProperty("BATT_SYS_VOLTAGE", SSTR(battery_info_get_system_voltage(info)));
            state->setUserProperty("BATT_TIME_UNTIL_DISCHARGE",
                    SSTR(battery_info_get_time_to_empty(info)));
            state->setUserProperty("BATT_TIME_UNTIL_FULL_CHARGE",
                    SSTR(battery_info_get_time_to_full(info)));
            state->setUserProperty("BATT_API_VERSION", SSTR(battery_info_get_version(info)));
            state->setUserProperty("BATT_OK", SSTR(isValueTrue(battery_info_is_battery_ok(info))));
            state->setUserProperty("BATT_PRESENT",
                    SSTR(isValueTrue(battery_info_is_battery_present(info))));
        } else {
            state->getLogger()->error("Did not receive battery information");
        }
    }

    std::string ActionBattery::chargingStateToString(int state)
    {
        switch (state) {
            case BATTERY_CHARGING_NOT_CHARGING:
                return "BATTERY_CHARGING_NOT_CHARGING";
            case BATTERY_CHARGING_TRICKLE_CHARGING:
                return "BATTERY_CHARGING_TRICKLE_CHARGING";
            case BATTERY_CHARGING_CONSTANT_CURRENT:
                return "BATTERY_CHARGING_CONSTANT_CURRENT";
            case BATTERY_CHARGING_CONSTANT_VOLTAGE:
                return "BATTERY_CHARGING_CONSTANT_VOLTAGE";
            case BATTERY_CHARGING_DONE_CHARGING:
                return "BATTERY_CHARGING_DONE_CHARGING";
            default:
                return "UNKNOWN";
        }
    }

} /* namespace actiontrigger */
