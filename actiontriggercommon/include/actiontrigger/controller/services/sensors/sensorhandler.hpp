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

#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <errno.h>
#include <sys/neutrino.h>
#include <sensor/libsensor.h>
#include "actiontrigger/controller/executionstate.hpp"

namespace actiontrigger
{

    class SensorHandler
    {
    public:
        static bool isSupported(sensor_type_e type);
        // Sets up events
        static SensorHandler* setupSensor(sensor_type_e type, ExecutionState* state,
                bool highPrecision = false);
        // retrieve single matching event
        sensor_event_t* retrieveEvent();
        // Stops events and frees resources
        virtual ~SensorHandler();

        sensor_t* getSensor();
        sensor_info_t* getSensorInfo();
    private:
        static const _Uint64t SENSOR_INITIALIZE_TIMEOUT_NS;
        static const _Int32t SENSOR_GET_EVENT_TIMEOUT_S;
        SensorHandler(sensor_type_e type, ExecutionState* state, sensor_t* sensor);
        sensor_type_e type;
        ExecutionState* state;
        sensor_t* sensor;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* SENSOR_HPP_ */
