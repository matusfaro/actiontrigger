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

#include "actiontrigger/controller/services/geolocation/actiongeolocationretrieve.hpp"

namespace actiontrigger
{

    std::string ActionGeoLocationRetrieve::TYPE = "ACTION_GeoLocationRetrieve";

    enum
    {
        TIMEOUT
    };

    std::vector<ParameterDefinition*>* ActionGeoLocationRetrieve::parameterDefinitions = NULL;
    Logger* ActionGeoLocationRetrieve::StatementLOG = LoggerFactory::getInstance(
            "GeoLocationRetrieve");
    const StatementInfo ActionGeoLocationRetrieve::info(false, "GPS Location",
            ActionGeoLocationRetrieve::TYPE,
            "Retrieve the device's GPS location. Includes latitude, longitude and altitude of current position");

    const StatementInfo ActionGeoLocationRetrieve::getInfo()
    {
        return ActionGeoLocationRetrieve::info;
    }

    ActionGeoLocationRetrieve::ActionGeoLocationRetrieve(StatementModel* model) :
            Action(model)
    {
    }

    ActionGeoLocationRetrieve::~ActionGeoLocationRetrieve()
    {
        StatementLOG->trace("~GeoLocationRetrieve");
    }

    std::vector<ParameterDefinition*>* ActionGeoLocationRetrieve::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("TIMEOUT", "Timeout",
                            "Amount of seconds to wait for a GPS signal", "1", "60", "1"));
        }
        return parameterDefinitions;
    }

    std::string ActionGeoLocationRetrieve::getDefaultParameter(std::string key)
    {
        if (key == "TIMEOUT") {
            return "10";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionGeoLocationRetrieve::getType() const
    {
        return ActionGeoLocationRetrieve::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGeoLocationRetrieve::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("GPS_LATITUDE",
                        "Current location's latitude.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("GPS_LONGITUDE",
                        "Current location's longitude.", "NUMBER"));
//    vars->push_back(new statement_output_variable_definition("GPS_ALTITUDE", "Current location's altitude", "NUMBER"));
//    vars->push_back(new statement_output_variable_definition("GPS_SPEED_HORIZONTAL", "Current horizontal speed (meter/second)", "NUMBER"));
//    vars->push_back(new statement_output_variable_definition("GPS_SPEED_VERTICAL", "Current vertical speed  (meter/second)", "NUMBER"));
//    vars->push_back(new statement_output_variable_definition("GPS_SPEED_MAGNITUDE", "Combination of horizontal and vertical speed  (meter/second)", "NUMBER"));
        return vars;
    }

    void ActionGeoLocationRetrieve::execute(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();

        state->getRuntimeResources()->bpsInitialize();

        errno = EOK;
        if (BPS_SUCCESS != geolocation_request_events(0)) {
            StatementLOG->warning(SSTR("Error requesting geolocation events: " << strerror(errno)));
            USERLOG->error(SSTR("Error requesting geolocation events: " << strerror(errno)));
            return;
        }

        geolocation_set_background(true);
        geolocation_set_period(0);

        int timeout = atoi(getParameter("TIMEOUT").c_str());
        timeout *= 1000;

        bps_event_t* event = state->getRuntimeResources()->bpsGetEvent(geolocation_get_domain(),
                GEOLOCATION_INFO, timeout);

        if (BPS_SUCCESS != geolocation_stop_events(0)) {
            StatementLOG->warning(SSTR("Unable to stop events: " << strerror(errno)));
        }

        if (event) {
            StatementLOG->debug(SSTR("GPS_LATITUDE: " << geolocation_event_get_latitude(event)));
            state->setUserProperty("GPS_LATITUDE", SSTR(geolocation_event_get_latitude(event)));

            StatementLOG->debug(SSTR("GPS_LONGITUDE: " << geolocation_event_get_longitude(event)));
            state->setUserProperty("GPS_LONGITUDE", SSTR(geolocation_event_get_longitude(event)));

//        StatementLOG->debug(SSTR("GPS_ALTITUDE: " << geolocation_event_get_altitude(event)));
//        state->setUserProperty("GPS_ALTITUDE", SSTR(geolocation_event_get_altitude(event)));
//
//        StatementLOG->debug(SSTR("GPS_SPEED_HORIZONTAL: " << geolocation_event_get_speed(event)));
//        state->setUserProperty("GPS_SPEED_HORIZONTAL", SSTR(geolocation_event_get_speed(event)));
//
//        StatementLOG->debug(SSTR("GPS_SPEED_VERTICAL: " << geolocation_event_get_vertical_speed(event)));
//        state->setUserProperty("GPS_SPEED_VERTICAL", SSTR(geolocation_event_get_vertical_speed(event)));
//
//        StatementLOG->debug(SSTR("GPS_SPEED_MAGNITUDE: " << std::hypot(geolocation_event_get_speed(event), geolocation_event_get_vertical_speed(event))));
//        state->setUserProperty("GPS_SPEED_MAGNITUDE", SSTR(std::hypot(geolocation_event_get_speed(event), geolocation_event_get_vertical_speed(event))));
        } else {
            USERLOG->error("Did not receive GPS information");
        }
    }

} /* namespace actiontrigger */
