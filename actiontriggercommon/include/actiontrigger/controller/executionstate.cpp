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

#include "actiontrigger/controller/executionstate.hpp"
#include <actiontrigger/controller/headlessconnector.hpp>

namespace actiontrigger
{

    Logger* ExecutionState::LOG = LoggerFactory::getInstance("ExecutionState");

    ExecutionState::ExecutionState(int executionNumber, DataModelLogger* userLogger,
            std::string uuid, bool isRemote, int id) :
            isRemote(isRemote), id(id), executionNumber(executionNumber), state(true), USERLOG(
                    userLogger), uuid(uuid)
    {
        this->executionProperties = new std::map<std::string, std::string>();
        this->userProperties = new std::map<std::string, std::string>();
        this->resources = isRemote ? NULL : new ExecutionResources(this->USERLOG);
        this->populateDefaultUserProperties();
    }

    ExecutionState::~ExecutionState()
    {
        delete this->executionProperties;
        delete this->userProperties;
    }

    int ExecutionState::getExecutionNumber()
    {
        return this->executionNumber;
    }

    bool ExecutionState::isRunning()
    {
        return this->state;
    }

    bool ExecutionState::getIsRemote()
    {
        return this->isRemote;
    }

    void ExecutionState::stop()
    {
        this->state = false;
        if (this->resources)
            this->resources->notifyStop();
        // Shutdown remote
        if (isRemote)
            HeadlessConnector::get()->procedureStop(id);
    }

    void ExecutionState::cleanup()
    {
        if (this->resources)
            delete this->resources;
    }

    void ExecutionState::giveUpIfNotRunning()
    {
        if (!isRunning())
            throw ExecutionAbortedException();
    }

    std::string ExecutionState::getExecutionProperty(std::string property)
    {
        std::string value;
        if (this->executionProperties->find(property) != this->executionProperties->end()) {
            value = this->executionProperties->operator[](property);
            LOG->trace(
                    SSTR("Retrieve Execution property: " << property << " = \"" << value << "\""));
        } else {
            value = "";
            LOG->trace(SSTR("Retrieve Execution property: " << property << " not found"));
        }
        return value;
    }

    void ExecutionState::setExecutionProperty(std::string property, std::string value)
    {
        LOG->trace(SSTR("Set Execution property: " << property << " = \"" << value << "\""));
        this->executionProperties->operator[](property) = value;
    }

    std::string ExecutionState::getUserProperty(std::string property)
    {
        std::string value;
        // Check for dynamic default properties
        if (hasDefaultUserProperty(property)) {
            return getDefaultUserProperty(property);
        }
        if (this->userProperties->find(property) != this->userProperties->end()) {
            value = this->userProperties->operator[](property);
            LOG->trace(SSTR("Retrieve User property: " << property << " = \"" << value << "\""));
        } else {
            value = "";
            LOG->trace(SSTR("Retrieve User property: " << property << " not found"));
        }
        return value;
    }

    void ExecutionState::setUserProperty(std::string property, std::string value)
    {
        LOG->trace(SSTR("Set User property: " << property << " = \"" << value << "\""));
        this->USERLOG->trace(SSTR("Set " << property << " = " << value));
        this->userProperties->operator[](property) = value;
        getLogger()->addVariable(property, value);
    }

    DataModelLogger* ExecutionState::getLogger()
    {
        return this->USERLOG;
    }

    ExecutionResources* ExecutionState::getRuntimeResources()
    {
        return this->resources;
    }

    GlobalResources* ExecutionState::getGlobalResources()
    {
        return GlobalResources::get();
    }

    bool ExecutionState::hasDefaultUserProperty(std::string property)
    {
        (void) property;
        return false;
    }

    std::string ExecutionState::getDefaultUserProperty(std::string property)
    {
        // Any values here cannot be modified by user
//    if (property == "LOGLEVEL")
//        return Logger::parseLogLevelName((LogLevel) USERLOG->getLoglevel());
        LOG->error(
                SSTR(
                        "Retrieve Default User property: " << property << " not found, this should never happen"));
        return "";
    }

    void ExecutionState::populateDefaultUserProperties()
    {
        // Any values here can be modified by user on runtime
        //this->userProperties->operator[]("EXAMPLE") = "VALUE";
    }

    std::vector<statement_output_variable_definition*>* ExecutionState::getDefaultExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        // Holds all available variables
//    vars->push_back(new statement_output_variable_definition("LOGLEVEL", "Read only string form of current log level", "STRING"));
        return vars;
    }

    std::string ExecutionState::getProcedureUuid()
    {
        return uuid;
    }

} /* namespace actiontrigger */
