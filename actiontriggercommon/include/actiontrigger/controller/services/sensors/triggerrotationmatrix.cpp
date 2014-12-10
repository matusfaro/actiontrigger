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

#include "actiontrigger/controller/services/sensors/triggerrotationmatrix.hpp"

namespace actiontrigger
{

    std::string TriggerRotationMatrix::TYPE = "TRIGGER_ROTATION_MATRIX";
    Logger* TriggerRotationMatrix::StatementLOG = LoggerFactory::getInstance(
            "TriggerRotationMatrix");
    const StatementInfo TriggerRotationMatrix::info(true, "Rotation Matrix",
            TriggerRotationMatrix::TYPE,
            "Triggers on rotation of the device. Sets three vectors A,B and C. C is the vector pointing to the sky, perpendicular to the ground. B is tangential to the ground at the device's current location. A is defined as the vector product of B and C.");

    bool TriggerRotationMatrix::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ROTATION_MATRIX);
    }

    const StatementInfo TriggerRotationMatrix::getInfo()
    {
        return TriggerRotationMatrix::info;
    }

    TriggerRotationMatrix::TriggerRotationMatrix(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerRotationMatrix::getType() const
    {
        return TriggerRotationMatrix::TYPE;
    }

    TriggerRotationMatrix::~TriggerRotationMatrix()
    {
    }

    bool TriggerRotationMatrix::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerRotationMatrix::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_A_X",
                        "Vector product of sky and device x-axis",
                        "x-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_A_Y",
                        "Vector product of sky and device y-axis",
                        "y-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_A_Z",
                        "Vector product of sky and device z-axis",
                        "z-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_B_X", "Vector from device x-axis",
                        "x-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_B_Y", "Vector from device y-axis",
                        "y-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_B_Z", "Vector from device z-axis",
                        "z-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_C_X", "Vector to sky x-axis",
                        "x-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_C_Y", "Vector to sky y-axis",
                        "y-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        definition->push_back(
                new trigger_output_variable_definition("ROT_MAT_C_Z", "Vector to sky z-axis",
                        "z-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        return definition;
    }

    bool TriggerRotationMatrix::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ROTATION_MATRIX, state);
        if (!sensor)
            return false;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return false;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: A_X=" << event->rotation_matrix[1] << " A_Y="
                                << event->rotation_matrix[2] << " A_Z=" << event->rotation_matrix[3]
                                << " B_X=" << event->rotation_matrix[4] << " B_Y="
                                << event->rotation_matrix[5] << " B_Z=" << event->rotation_matrix[6]
                                << " C_X=" << event->rotation_matrix[7] << " C_Y="
                                << event->rotation_matrix[8] << " C_Z="
                                << event->rotation_matrix[9]));
        values->push_back(/*ROT_MAT_A_X*/SSTR(event->rotation_matrix[1]));
        values->push_back(/*ROT_MAT_A_Y*/SSTR(event->rotation_matrix[2]));
        values->push_back(/*ROT_MAT_A_Z*/SSTR(event->rotation_matrix[3]));
        values->push_back(/*ROT_MAT_B_X*/SSTR(event->rotation_matrix[4]));
        values->push_back(/*ROT_MAT_B_Y*/SSTR(event->rotation_matrix[5]));
        values->push_back(/*ROT_MAT_B_Z*/SSTR(event->rotation_matrix[6]));
        values->push_back(/*ROT_MAT_C_X*/SSTR(event->rotation_matrix[7]));
        values->push_back(/*ROT_MAT_C_Y*/SSTR(event->rotation_matrix[8]));
        values->push_back(/*ROT_MAT_C_Z*/SSTR(event->rotation_matrix[9]));
        delete event;
        return checkRequirements(state, values);
    }

    void TriggerRotationMatrix::blockUntilActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ROTATION_MATRIX, state,
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
            RUNLOG->debug(
                    SSTR(
                            "Retrieved: A_X=" << event->rotation_matrix[1] << " A_Y="
                                    << event->rotation_matrix[2] << " A_Z="
                                    << event->rotation_matrix[3] << " B_X="
                                    << event->rotation_matrix[4] << " B_Y="
                                    << event->rotation_matrix[5] << " B_Z="
                                    << event->rotation_matrix[6] << " C_X="
                                    << event->rotation_matrix[7] << " C_Y="
                                    << event->rotation_matrix[8] << " C_Z="
                                    << event->rotation_matrix[9]));
            values->push_back(/*ROT_MAT_A_X*/SSTR(event->rotation_matrix[1]));
            values->push_back(/*ROT_MAT_A_Y*/SSTR(event->rotation_matrix[2]));
            values->push_back(/*ROT_MAT_A_Z*/SSTR(event->rotation_matrix[3]));
            values->push_back(/*ROT_MAT_B_X*/SSTR(event->rotation_matrix[4]));
            values->push_back(/*ROT_MAT_B_Y*/SSTR(event->rotation_matrix[5]));
            values->push_back(/*ROT_MAT_B_Z*/SSTR(event->rotation_matrix[6]));
            values->push_back(/*ROT_MAT_C_X*/SSTR(event->rotation_matrix[7]));
            values->push_back(/*ROT_MAT_C_Y*/SSTR(event->rotation_matrix[8]));
            values->push_back(/*ROT_MAT_C_Z*/SSTR(event->rotation_matrix[9]));
            delete event;
            if (checkRequirements(state, values))
                break;
        }
        delete sensor;
    }

} /* namespace actiontrigger */
