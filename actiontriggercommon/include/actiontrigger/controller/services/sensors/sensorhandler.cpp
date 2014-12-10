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

#include "actiontrigger/controller/services/sensors/sensorhandler.hpp"

namespace actiontrigger
{

    Logger* SensorHandler::LOG = LoggerFactory::getInstance("SensorHandler");
    const _Uint64t SensorHandler::SENSOR_INITIALIZE_TIMEOUT_NS = 5000000000ULL;
    const _Int32t SensorHandler::SENSOR_GET_EVENT_TIMEOUT_S = 5;

    SensorHandler::SensorHandler(sensor_type_e type, ExecutionState* state, sensor_t* sensor) :
            type(type), state(state), sensor(sensor)
    {
    }

    sensor_t* SensorHandler::getSensor()
    {
        LOG->trace("getSensor");
        return this->sensor;
    }

    sensor_info_t* SensorHandler::getSensorInfo()
    {
        sensor_info_t* info = new sensor_info_t();
        sensor_get_info(this->sensor, info);
        return info;
    }

    bool SensorHandler::isSupported(sensor_type_e type)
    {
        return sensor_exists(type);
    }

    SensorHandler* SensorHandler::setupSensor(sensor_type_e type, ExecutionState* state,
            bool highPrecision)
    {
        LOG->trace("setupSensor");
        Logger* RUNLOG = state->getLogger();

        // Check if sensor is valid
        if (!isSupported(type)) {
            RUNLOG->error(SSTR("Sensor is not supported on this device"));
            return NULL;
        }

        // Connect to channel
        if (state->getRuntimeResources()->getChannelConnection() == -1) {
            RUNLOG->error(SSTR("Cannot receive message from sensor, connect attach failed"));
            return NULL;
        }

        // Initialize sensor
        errno = EOK;
        sensor_t* sensor = sensor_new(type);
        if (!sensor) {
            RUNLOG->error(SSTR("Cannot initiliaze sensor: " << strerror(errno)));
            return NULL;
        }

        // Initialize sigevent
        sigevent* ev = new sigevent();
        SIGEV_PULSE_INIT(ev, state->getRuntimeResources()->getChannelConnection(),
                SIGEV_PULSE_PRIO_INHERIT, /*MY_SENSOR_PULSE_CODE*/type, sensor);

        // Set up notification for new events.
        errno = EOK;
        int rcvid = sensor_event_notify(sensor, ev);
        if (rcvid < 0) {
            RUNLOG->error(SSTR("Cannot setup sensor notifications: " << strerror(errno)));
            delete ev;
            return NULL;
        }

        // Set sensor attributes.
        errno = EOK;
        rcvid = sensor_set_background(sensor, true);
        if (rcvid < 0) {
            LOG->warning(
                    SSTR(
                            "Cannot setup sensor to run in background (as expected): " << strerror(errno)));
            // EAT THIS ERROR, FUCK YOU BLACKBERRY
//        RUNLOG->error(SSTR("Cannot setup sensor to run in background: " << strerror(errno)));
//        delete ev;
//        return NULL;
        }
        errno = EOK;
        rcvid = sensor_set_reduced_reporting(sensor, !highPrecision);
        if (rcvid < 0) {
            RUNLOG->warning(SSTR("Cannot setup sensor to reduce reporting: " << strerror(errno)));
        }
        errno = EOK;
        uint32_t delay;
        rcvid = sensor_get_delay(sensor, &delay);
        if (rcvid < 0) {
            RUNLOG->error(SSTR("Cannot retrieve sensor granularity: " << strerror(errno)));
            delete ev;
            return NULL;
        }
        RUNLOG->trace(SSTR("Sampling interval of sensor in microseconds: " << delay));

        // Receive notification
        _pulse pulse;
        do {
            ev->sigev_notify = SIGEV_UNBLOCK;
            errno = EOK;
            TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_RECEIVE, ev, &SENSOR_INITIALIZE_TIMEOUT_NS,
                    NULL);
            rcvid = MsgReceive(state->getRuntimeResources()->getChannel(), &pulse, sizeof(pulse),
                    NULL);
            if (rcvid < 0) {
                if (errno == EINTR) {
                    // Let's not warn the user
                    LOG->trace(SSTR("Sensor initialize was interrupted by signal, retrying..."));
                    continue;
                } else if (errno == ETIMEDOUT) {
                    RUNLOG->error(SSTR("Timed out waiting for sensor to initialize."));
                } else {
                    RUNLOG->error(SSTR("Cannot receive message from sensor: " << strerror(errno)));
                }
                // Failed to initialize sensor
                delete ev;
                return NULL;
            }
        } while (pulse.code != type);

        delete ev;
        return new SensorHandler(type, state, sensor);
    }

    sensor_event_t* SensorHandler::retrieveEvent()
    {
        LOG->trace("retrieveEvent");
        Logger* RUNLOG = this->state->getLogger();
        timeval timeout;
        timeout.tv_sec = SENSOR_GET_EVENT_TIMEOUT_S;
        timeout.tv_usec = 0;
        sensor_event_t* event = new sensor_event_t();
        int rcvid;
        do {
            errno = EOK;
            rcvid = sensor_wait_event(sensor, event, &timeout);
            if (rcvid != EOK) {

                if (errno == EINTR) {
                    // Let's not warn the user
                    LOG->trace(SSTR("Sensor initialize was interrupted by signal, retrying..."));
                    continue;
                } else if (errno == ETIMEDOUT) {
                    // Do nothing as this may be expected from some sensors
                } else {
                    RUNLOG->error(
                            SSTR("Failed to retrieve event from sensor: " << strerror(errno)));
                }
                event = NULL;
                break;
            }
            if (event->type != this->type) {
                LOG->trace(
                        SSTR(
                                "Received uninteresting event with type=" << event->type
                                        << " expected type: " << this->type));
                event = NULL;
                break;
            }
        } while (false);
        rcvid = sensor_event_notify_rearm(sensor);
        if (rcvid != EOK) {
            RUNLOG->error(SSTR("Re-arming sensor failed: " << strerror(errno)));
        }
        return event;
    }

    SensorHandler::~SensorHandler()
    {
        LOG->trace("~SensorHandler");
        sensor_delete(&this->sensor);
    }

} /* namespace actiontrigger */
