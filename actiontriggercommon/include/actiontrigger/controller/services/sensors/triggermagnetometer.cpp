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

#include "actiontrigger/controller/services/sensors/triggermagnetometer.hpp"

namespace actiontrigger
{

    std::string TriggerMagnetometer::TYPE = "TRIGGER_MAGNETOMETER";
    Logger* TriggerMagnetometer::StatementLOG = LoggerFactory::getInstance("TriggerMagnetometer");
    const StatementInfo TriggerMagnetometer::info(true, "Magnetometer", TriggerMagnetometer::TYPE,
            "Triggers on current magnetic field. Sets x, y and z axis magnetic field strength in uT (micro Tesla). Can be used as metal detector.");

    bool TriggerMagnetometer::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_MAGNETOMETER);
    }

    const StatementInfo TriggerMagnetometer::getInfo()
    {
        return TriggerMagnetometer::info;
    }

    TriggerMagnetometer::TriggerMagnetometer(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerMagnetometer::getType() const
    {
        return TriggerMagnetometer::TYPE;
    }

    TriggerMagnetometer::~TriggerMagnetometer()
    {
    }

    bool TriggerMagnetometer::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerMagnetometer::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("MAGNET_X", "Magnetic field x-axis",
                        "Magnetic field strength on x-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("MAGNET_Y", "Magnetic field y-axis",
                        "Magnetic field strength on y-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("MAGNET_Z", "Magnetic field z-axis",
                        "Magnetic field strength on z-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("MAGNET_MAGNITUDE",
                        "Magnetic field magnitude", "Magnetic field strength magnitude in m/s/s",
                        "NUMBER"));
        return definition;
    }

    bool TriggerMagnetometer::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_MAGNETOMETER, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        float magnitude = sqrt(
                event->motion.dsp.x * event->motion.dsp.x
                        + event->motion.dsp.y * event->motion.dsp.y
                        + event->motion.dsp.z * event->motion.dsp.z);
        RUNLOG->debug(
                SSTR(
                        "X=" << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                << event->motion.dsp.z << " Magnitude=" << magnitude));
        values->push_back(/*MAGNET_X*/SSTR(event->motion.dsp.x));
        values->push_back(/*MAGNET_Y*/SSTR(event->motion.dsp.y));
        values->push_back(/*MAGNET_Z*/SSTR(event->motion.dsp.z));
        values->push_back(/*MAGNET_MAGNITUDE*/SSTR(magnitude));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerMagnetometer::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_MAGNETOMETER, state,
                isValueTrue(getParameter("HIGH_PRECISION", state)));
        if (!sensor)
            throw ExecutionAbortedException();
        sensor_event_t* event;
        std::vector<std::string>* values;
        for (;;) {
            // Check if we are still running
            if (!state->isRunning())
                throw ExecutionAbortedException();
            // Retrieve event with values
            event = sensor->retrieveEvent();
            // Check if event was successfully retrieved
            if (NULL == event)
                continue;
            // Check returned values
            values = new std::vector<std::string>();
            float magnitude = sqrt(
                    event->motion.dsp.x * event->motion.dsp.x
                            + event->motion.dsp.y * event->motion.dsp.y
                            + event->motion.dsp.z * event->motion.dsp.z);
            RUNLOG->debug(
                    SSTR(
                            "X=" << event->motion.dsp.x << " Y=" << event->motion.dsp.y << " Z="
                                    << event->motion.dsp.z << " Magnitude=" << magnitude));
            values->push_back(/*MAGNET_X*/SSTR(event->motion.dsp.x));
            values->push_back(/*MAGNET_Y*/SSTR(event->motion.dsp.y));
            values->push_back(/*MAGNET_Z*/SSTR(event->motion.dsp.z));
            values->push_back(/*MAGNET_MAGNITUDE*/SSTR(magnitude));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
