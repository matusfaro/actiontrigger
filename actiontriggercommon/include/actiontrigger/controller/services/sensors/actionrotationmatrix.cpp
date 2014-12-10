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

#include "actiontrigger/controller/services/sensors/actionrotationmatrix.hpp"

namespace actiontrigger
{

    std::string ActionRotationMatrix::TYPE = "ACTION_ROTATION_MATRIX";
    std::vector<ParameterDefinition*>* ActionRotationMatrix::parameterDefinitions = NULL;
    Logger* ActionRotationMatrix::StatementLOG = LoggerFactory::getInstance("ActionRotationMatrix");
    const StatementInfo ActionRotationMatrix::info(false, "Rotation Matrix",
            ActionRotationMatrix::TYPE,
            "Retrieves rotation of the device. Sets three vectors A,B and C. C is the vector pointing to the sky, perpendicular to the ground. B is tangential to the ground at the device's current location. A is defined as the vector product of B and C.");

    bool ActionRotationMatrix::isSupported()
    {
        return SensorHandler::isSupported(SENSOR_TYPE_ROTATION_MATRIX);
    }

    const StatementInfo ActionRotationMatrix::getInfo()
    {
        return ActionRotationMatrix::info;
    }

    ActionRotationMatrix::ActionRotationMatrix(StatementModel* model) :
            Action(model)
    {
    }

    ActionRotationMatrix::~ActionRotationMatrix()
    {
    }

    std::string ActionRotationMatrix::getType() const
    {
        return ActionRotationMatrix::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRotationMatrix::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_A_X",
                        "x-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_A_Y",
                        "y-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_A_Z",
                        "z-axis of vector defined as the vector product of ROT_MAT_B and ROT_MAT_C",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_B_X",
                        "x-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_B_Y",
                        "y-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_B_Z",
                        "z-axis of vector tangential to the ground at the device's current location",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_C_X",
                        "x-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_C_Y",
                        "y-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("ROT_MAT_C_Z",
                        "z-axis of vector pointing to the sky, perpendicular to the ground",
                        "NUMBER"));
        return vars;
    }

    void ActionRotationMatrix::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        errno = EOK;
        SensorHandler* sensor = SensorHandler::setupSensor(SENSOR_TYPE_ROTATION_MATRIX, state);
        if (!sensor)
            return;
        sensor_event_t* event = sensor->retrieveEvent();
        delete sensor;
        if (NULL == event)
            return;
        RUNLOG->debug(
                SSTR(
                        "Retrieved: A_X=" << event->rotation_matrix[1] << " A_Y="
                                << event->rotation_matrix[2] << " A_Z=" << event->rotation_matrix[3]
                                << " B_X=" << event->rotation_matrix[4] << " B_Y="
                                << event->rotation_matrix[5] << " B_Z=" << event->rotation_matrix[6]
                                << " C_X=" << event->rotation_matrix[7] << " C_Y="
                                << event->rotation_matrix[8] << " C_Z="
                                << event->rotation_matrix[9]));
        state->setUserProperty("ROT_MAT_A_X", SSTR(event->rotation_matrix[1]));
        state->setUserProperty("ROT_MAT_A_Y", SSTR(event->rotation_matrix[2]));
        state->setUserProperty("ROT_MAT_A_Z", SSTR(event->rotation_matrix[3]));
        state->setUserProperty("ROT_MAT_B_X", SSTR(event->rotation_matrix[4]));
        state->setUserProperty("ROT_MAT_B_Y", SSTR(event->rotation_matrix[5]));
        state->setUserProperty("ROT_MAT_B_Z", SSTR(event->rotation_matrix[6]));
        state->setUserProperty("ROT_MAT_C_X", SSTR(event->rotation_matrix[7]));
        state->setUserProperty("ROT_MAT_C_Y", SSTR(event->rotation_matrix[8]));
        state->setUserProperty("ROT_MAT_C_Z", SSTR(event->rotation_matrix[9]));
        delete event;
    }

} /* namespace actiontrigger */
