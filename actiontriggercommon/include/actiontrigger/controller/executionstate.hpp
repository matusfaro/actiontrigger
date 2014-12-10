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

#ifndef EXECUTIONSTATE_HPP_
#define EXECUTIONSTATE_HPP_

#include <QObject>
#include <string>
#include <map>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/model/logger/blackberry/datamodellogger.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/exception.hpp"
#include "actiontrigger/controller/executionresources.hpp"
#include "actiontrigger/controller/globalresources.hpp"

namespace actiontrigger
{

    class ExecutionState
    {

    public:
        ExecutionState(int executionNumber, DataModelLogger* userLogger, std::string uuid,
                bool isRemote = false, int id = 0);
        virtual ~ExecutionState();

        int getExecutionNumber();

        // Used for checking the status and stopping execution
        bool isRunning();
        void stop();
        void cleanup();
        bool getIsRemote();

        // Used to throw exception if execution has been stopped
        void giveUpIfNotRunning();

        // Used for getting and setting environment properties
        std::string getExecutionProperty(std::string property);
        void setExecutionProperty(std::string property, std::string value);
        std::string getUserProperty(std::string property);
        void setUserProperty(std::string property, std::string value);
        DataModelLogger* getLogger();
        ExecutionResources* getRuntimeResources();
        GlobalResources* getGlobalResources();

        void populateDefaultUserProperties();
        bool hasDefaultUserProperty(std::string property);
        std::string getDefaultUserProperty(std::string property);
        static std::vector<statement_output_variable_definition*>* getDefaultExposedVariableDefinitions();

        std::string getProcedureUuid();

    private:
        bool isRemote;
        int id;

        int executionNumber;
        // Since we are not using mutexes, use volatile and cross fingers
        volatile bool state;

        // Environment variables
        std::map<std::string, std::string>* executionProperties;
        std::map<std::string, std::string>* userProperties;
        ExecutionResources* resources;

        // User-worthy execution log
        DataModelLogger* USERLOG;

        std::string uuid;

        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* EXECUTIONSTATE_HPP_ */
