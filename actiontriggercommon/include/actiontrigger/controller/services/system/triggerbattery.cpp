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

#include "actiontrigger/controller/services/system/triggerbattery.hpp"

namespace actiontrigger
{

    std::string TriggerBattery::TYPE = "TRIGGER_BATTERY";
    Logger* TriggerBattery::StatementLOG = LoggerFactory::getInstance("TriggerBattery");
    const StatementInfo TriggerBattery::info(true, "Battery", TriggerBattery::TYPE,
            "Check device battery information.");

    const StatementInfo TriggerBattery::getInfo()
    {
        return TriggerBattery::info;
    }

    TriggerBattery::TriggerBattery(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerBattery::getType() const
    {
        return TriggerBattery::TYPE;
    }

    TriggerBattery::~TriggerBattery()
    {
    }

    bool TriggerBattery::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerBattery::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("BATT_ALERT", "Battery Alert",
                        "Determine whether the battery alert is triggered.", "BOOL"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_REMAINING", "Battery Remaining",
                        "Retrieve the available energy remaining in the battery.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_AVERAGE_CURRENT",
                        "Battery avg current",
                        "Retrieve the battery average current reported in milliamperes (mA)..",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_AVERAGE_POWER", "Battery avg power",
                        "Retrieve the battery average power reported in milliwatts (mW).",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_CYCLE_COUNT", "Battery cycles",
                        "Retrieve the battery cycle count.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_DESIGN_CAPACITY",
                        "Battery design capacity", "Retrieve the battery design capacity.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_FULL_CAPACITY",
                        "Battery full capacity", "Retrieve the battery full available capacity.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_FULL_CHARGE_CAPACITY",
                        "Battery charge capacity", "Retrieve the battery full charge capacity.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_BATTERY_ID", "Battery ID",
                        "Retrieve the battery ID.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MAX_LOAD_CURRENT",
                        "Battery max load current", "Retrieve the battery maximum load current.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MAX_LOAD_TIME_TO_EMPTY",
                        "Battery max load to empty",
                        "Retrieve the battery maximum load time to empty.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_NAME", "Battery name",
                        "Retrieve the battery name.", "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_NOMINAL_AVAIL_CAPACITY",
                        "Battery nominal capacity",
                        "Retrieve the battery nominal available capacity.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_TEMPERATURE", "Battery temperature",
                        "Retrieve the battery temperature.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_TIME_TO_EMPTY",
                        "Battery time to empty",
                        "Retrieve the battery time to empty at constant power.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_VOLTAGE", "Battery voltage",
                        "Retrieve the battery voltage.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MAX_CHARGE_CURRENT",
                        "Battery max charge current",
                        "Retrieve the charger maximum charge current.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MAX_INPUT_CURRENT",
                        "Battery max input current", "Retrieve the charger maximum input current.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_CHARGER_NAME", "Battery charger name",
                        "Retrieve the charger name.", "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_DEVICE_NAME", "Battery device name",
                        "Retrieve the device name.", "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_CHARGE_PERCENT", "Battery charge %",
                        "Retrieve the state of charge measured as a percentage from 0 to 100.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_HEALTH_PERCENT", "Battery health %",
                        "Retrieve the state of health measured as a percentage from 0 to 100.",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_CURRENT", "Battery current",
                        "Retrieve the system charge current reported in milliamperes (mA).",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_STATE", "Battery state",
                        "Retrieve the system charging state. One of BATTERY_CHARGING_NOT_CHARGING, BATTERY_CHARGING_TRICKLE_CHARGING, BATTERY_CHARGING_CONSTANT_CURRENT, BATTERY_CHARGING_CONSTANT_VOLTAGE, BATTERY_CHARGING_DONE_CHARGING.",
                        "STRING"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_CURRENT_REAL", "Battery real current",
                        "Retrieve the actual current drawn from the charger reported in milliamperes (mA).",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MAX_VOLTAGE", "Battery max voltage",
                        "Retrieve the system maximum voltage.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_MIN_VOLTAGE", "Battery min voltage",
                        "Retrieve the system minimum voltage.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_SYS_VOLTAGE", "Battery sys voltage",
                        "Retrieve the system voltage.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_TIME_UNTIL_DISCHARGE",
                        "Battery time until discharge",
                        "Retrieve the amount of time before the battery is fully discharged (empty).",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_TIME_UNTIL_FULL_CHARGE",
                        "Battery time until charge",
                        "Retrieve the amount of time to fully charge the battery.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_API_VERSION", "Battery API version",
                        "Retrieve the Battery API version.", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_OK", "Battery OK?",
                        "Determine whether the battery is OK. If the battery is not OK, it may indicate a problem with the battery or that the battery is not ready",
                        "BOOL"));
        definition->push_back(
                new trigger_output_variable_definition("BATT_PRESENT", "Battery present",
                        "Retrieve whether the battery is present the device.", "BOOL"));
        return definition;
    }

    bool TriggerBattery::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        state->getRuntimeResources()->bpsInitialize();
        battery_request_events(0);
        bps_event_t* event = state->getRuntimeResources()->bpsGetEvent(battery_get_domain(),
                BATTERY_INFO);
        battery_stop_events(0);
        if (event) {
            RUNLOG->debug(SSTR("Retrieved battery event"));
            battery_info_t * info = battery_event_get_info(event);
            values->push_back(
                    /*BATT_ALERT*/SSTR(isValueTrue(battery_info_get_battery_alert(info))));
            values->push_back(
                    /*BATT_REMAINING*/SSTR(battery_info_get_battery_available_energy(info)));
            values->push_back(
                    /*BATT_AVERAGE_CURRENT*/SSTR(battery_info_get_battery_average_current(info)));
            values->push_back(
                    /*BATT_AVERAGE_POWER*/SSTR(battery_info_get_battery_average_power(info)));
            values->push_back(/*BATT_CYCLE_COUNT*/SSTR(battery_info_get_battery_cycle_count(info)));
            values->push_back(
                    /*BATT_DESIGN_CAPACITY*/SSTR(battery_info_get_battery_design_capacity(info)));
            values->push_back(
                    /*BATT_FULL_CAPACITY*/SSTR(
                            battery_info_get_battery_full_available_capacity(info)));
            values->push_back(
                    /*BATT_FULL_CHARGE_CAPACITY*/SSTR(
                            battery_info_get_battery_full_charge_capacity(info)));
            values->push_back(/*BATT_BATTERY_ID*/SSTR(battery_info_get_battery_id(info)));
            values->push_back(
                    /*BATT_MAX_LOAD_CURRENT*/SSTR(battery_info_get_battery_max_load_current(info)));
            values->push_back(
                    /*BATT_MAX_LOAD_TIME_TO_EMPTY*/SSTR(
                            battery_info_get_battery_max_load_time_to_empty(info)));
            values->push_back(
                    /*BATT_NAME*/SSTR(
                            (battery_info_get_battery_name(info) == NULL ? "N/A" : battery_info_get_battery_name(info))));
            values->push_back(
                    /*BATT_NOMINAL_AVAIL_CAPACITY*/SSTR(
                            battery_info_get_battery_nominal_available_capacity(info)));
            values->push_back(/*BATT_TEMPERATURE*/SSTR(battery_info_get_battery_temperature(info)));
            values->push_back(
                    /*BATT_TIME_TO_EMPTY*/SSTR(
                            battery_info_get_battery_time_to_empty_at_constant_power(info)));
            values->push_back(/*BATT_VOLTAGE*/SSTR(battery_info_get_battery_voltage(info)));
            values->push_back(
                    /*BATT_MAX_CHARGE_CURRENT*/SSTR(
                            battery_info_get_charger_max_charge_current(info)));
            values->push_back(
                    /*BATT_MAX_INPUT_CURRENT*/SSTR(
                            battery_info_get_charger_max_input_current(info)));
            values->push_back(
                    /*BATT_CHARGER_NAME*/SSTR(
                            (battery_info_get_charger_name(info) == NULL ? "N/A" : battery_info_get_charger_name(info))));
            values->push_back(
                    /*BATT_DEVICE_NAME*/SSTR(
                            (battery_info_get_device_name(info) == NULL ? "N/A" : battery_info_get_device_name(info))));
            values->push_back(/*BATT_CHARGE_PERCENT*/SSTR(battery_info_get_state_of_charge(info)));
            values->push_back(/*BATT_HEALTH_PERCENT*/SSTR(battery_info_get_state_of_health(info)));
            values->push_back(/*BATT_CURRENT*/SSTR(battery_info_get_system_charge_current(info)));
            values->push_back(
                    /*BATT_STATE*/SSTR(
                            ActionBattery::chargingStateToString(
                                    battery_info_get_system_charging_state(info))));
            values->push_back(
                    /*BATT_CURRENT_REAL*/SSTR(battery_info_get_system_input_current_monitor(info)));
            values->push_back(/*BATT_MAX_VOLTAGE*/SSTR(battery_info_get_system_max_voltage(info)));
            values->push_back(/*BATT_MIN_VOLTAGE*/SSTR(battery_info_get_system_min_voltage(info)));
            values->push_back(/*BATT_SYS_VOLTAGE*/SSTR(battery_info_get_system_voltage(info)));
            values->push_back(
                    /*BATT_TIME_UNTIL_DISCHARGE*/SSTR(battery_info_get_time_to_empty(info)));
            values->push_back(
                    /*BATT_TIME_UNTIL_FULL_CHARGE*/SSTR(battery_info_get_time_to_full(info)));
            values->push_back(/*BATT_API_VERSION*/SSTR(battery_info_get_version(info)));
            values->push_back(/*BATT_OK*/SSTR(isValueTrue(battery_info_is_battery_ok(info))));
            values->push_back(
                    /*BATT_PRESENT*/SSTR(isValueTrue(battery_info_is_battery_present(info))));
            return checkRequirements(state, values);
        } else {
            RUNLOG->error("Did not receive battery information");
            return false;
        }
    }

} /* namespace actiontrigger */
