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

#include "actiontrigger/controller/services/sensors/triggergyroscope.hpp"

namespace actiontrigger
{

    std::string TriggerGyroscope::TYPE = "TRIGGER_GYROSCOPE";
    Logger* TriggerGyroscope::StatementLOG = LoggerFactory::getInstance("TriggerGyroscope");
    const StatementInfo TriggerGyroscope::info(true, "Gyroscope", TriggerGyroscope::TYPE,
            "Trigger on magnetic field from Gyroscope sensor. Sets x, y and z axis magnetic field strength in uT (micro Tesla).");

    bool TriggerGyroscope::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_GYROSCOPE);
    }

    const StatementInfo TriggerGyroscope::getInfo()
    {
        return TriggerGyroscope::info;
    }

    TriggerGyroscope::TriggerGyroscope(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerGyroscope::getType() const
    {
        return TriggerGyroscope::TYPE;
    }

    TriggerGyroscope::~TriggerGyroscope()
    {
    }

    bool TriggerGyroscope::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerGyroscope::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("GYRO_X", "Gyroscope x-axis",
                        "Gyroscope reading on x-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("GYRO_Y", "Gyroscope y-axis",
                        "Gyroscope reading on y-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("GYRO_Z", "Gyroscope z-axis",
                        "Gyroscope reading on z-axis in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("GYRO_MAGNITUDE", "Gyroscope magnitude",
                        "Gyroscope magnitude reading in m/s/s", "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("GYRO_TEMP", "Gyroscope temperature",
                        "Temperature reading of Gyroscope sensor in degrees Celsius", "NUMBER"));
        return definition;
    }

    bool TriggerGyroscope::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_GYROSCOPE, state);
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
        values->push_back(/*GYRO_X*/SSTR(event->motion.dsp.x));
        values->push_back(/*GYRO_Y*/SSTR(event->motion.dsp.y));
        values->push_back(/*GYRO_Z*/SSTR(event->motion.dsp.z));
        values->push_back(/*GYRO_MAGNITUDE*/SSTR(magnitude));
        values->push_back(/*GYRO_TEMP*/SSTR(event->motion.gyro.temperature));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerGyroscope::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_GYROSCOPE, state,
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
            values->push_back(/*GYRO_X*/SSTR(event->motion.dsp.x));
            values->push_back(/*GYRO_Y*/SSTR(event->motion.dsp.y));
            values->push_back(/*GYRO_Z*/SSTR(event->motion.dsp.z));
            values->push_back(/*GYRO_MAGNITUDE*/SSTR(magnitude));
            values->push_back(/*GYRO_TEMP*/SSTR(event->motion.gyro.temperature));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
