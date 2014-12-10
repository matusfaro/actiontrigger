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

#ifndef ActionRecordVideo_HPP_
#define ActionRecordVideo_HPP_

#include <errno.h>
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include <camera/camera_api.h>

namespace actiontrigger
{

    class ActionRecordVideo: public Action
    {
    public:
        ActionRecordVideo(StatementModel* model);
        virtual ~ActionRecordVideo();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
        std::vector<ParameterDefinition*>* getParameterDefinitions();
        std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();

        // Shared logic between ActionRecordDesktop, sigh...
        static void executeCameraAction(ExecutionState* state, std::string action,
                std::string cameraUnitString, std::string appendTime, std::string outFile);
        static std::vector<ParameterDefinition*>* getParameterDefinitionsCamera(
                ParameterDefinition* cameraOpt);
        static std::vector<statement_output_variable_definition*>* getExposedVariableDefinitionsCamera();
    protected:
        std::string getDefaultParameter(std::string key);
        void execute(ExecutionState* state);
    private:
        static Logger* StatementLOG;
        static std::vector<ParameterDefinition*>* parameterDefinitions;
        static void viewFinderStatusCallback(camera_handle_t handle, camera_devstatus_t status,
                uint16_t value, void *context);
        static void videoRecordingStatusCallback(camera_handle_t handle, camera_devstatus_t status,
                uint16_t value, void *context);
    };

} /* namespace actiontrigger */
#endif /* ActionRecordVideo_HPP_ */
