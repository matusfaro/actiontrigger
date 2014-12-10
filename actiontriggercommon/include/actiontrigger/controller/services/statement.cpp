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

#include "actiontrigger/controller/services/statement.hpp"

namespace actiontrigger
{

    Logger* Statement::LOG = LoggerFactory::getInstance("Statement");

    Statement::Statement(StatementModel* model) :
            model(model), currentExecutionNumber(0), pendingExecutionState(NULL), currentExecutionState(
                    NULL), nextStatement(NULL)
    {
        LOG->trace("Statement(model)");
        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&deadbolt, &mattr);
    }

    Statement::~Statement()
    {
        LOG->trace("~Statement");
        pthread_mutex_destroy(&deadbolt);
        pthread_mutexattr_destroy(&mattr);
    }

    void Statement::remove()
    {
        this->model->remove();
    }

    ExecutionState* Statement::prepareStart(ConnectExecutionModel* dataModel, bool debug)
    {
        LogLevel loglevel = debug ? DEBUG : DEFAULT_LOGLEVEL;
        pthread_mutex_lock(&deadbolt);
        if (pendingExecutionState == NULL) {
            int myExecutionNumber = ++currentExecutionNumber;
            DataModelLogger* logger = new DataModelLogger(dataModel, loglevel);
            ExecutionState* newExecutionState = new ExecutionState(myExecutionNumber, logger,
                    Common::genUuidStr());
            newExecutionState->setExecutionProperty("RUN_ONCE", "TRUE");
            pendingExecutionState = newExecutionState;
            pthread_mutex_unlock(&deadbolt);
            return newExecutionState;
        } else {
            pthread_mutex_unlock(&deadbolt);
            return NULL;
        }
    }

    void Statement::start()
    {
        LOG->trace("start");
        ExecutionState* myState;
        int myExecutionNumber;

        pthread_mutex_lock(&deadbolt);
        // Set new execution
        if (this->pendingExecutionState) {
            // Stop currently running execution
            if (this->currentExecutionState)
                this->currentExecutionState->stop();
            // Pointing to latest state
            currentExecutionState = pendingExecutionState;
            // State used in this execution
            myState = currentExecutionState;
            // Clear pending state
            pendingExecutionState = NULL;
            pthread_mutex_unlock(&deadbolt);
        } else {
            LOG->error("Start called without prepareStart");
            pthread_mutex_unlock(&deadbolt);
            return;
        }
        // Get execution number of this thread
        myExecutionNumber = myState->getExecutionNumber();

        DataModelLogger* RUNLOG = myState->getLogger();
        RUNLOG->info("Execution started");

        try {
            // Validate parameters
            if (!checkParameterValidity(myState)) {
                RUNLOG->error("Parameter check failed");
                throw ExecutionAbortedException();
            }

            // Check if we should continue
            if (!myState->isRunning())
                throw ExecutionAbortedException();

            // Run statement
            execute(myState);

            // This should only happen if runOnce is true, otherwise
            RUNLOG->info(SSTR("Finished execution"));
        } catch (const ExecutionAbortedException &e) {
            RUNLOG->popAllLayers();
            RUNLOG->info(SSTR("Execution stopped"));
        } catch (const std::exception &e) {
            RUNLOG->error(SSTR("Unexpected error: " << e.what()));
            LOG->warning(
                    SSTR(
                            "Thread #" << myExecutionNumber << " >>> "
                                    << "Unhandled exception caught: " << e.what()));
        } catch (...) {
            RUNLOG->error(SSTR("Unexpected unidentified error"));
            LOG->warning(
                    SSTR(
                            "Thread #" << myExecutionNumber << " >>> "
                                    << "Unhandled unknown exception caught!"));
        }
        // Shut it all down
        pthread_mutex_lock(&deadbolt);
        if (currentExecutionState == myState) {
            currentExecutionState = NULL;
        }
        pthread_mutex_unlock(&deadbolt);
        myState->cleanup();
        delete myState;
    }

    void Statement::stop()
    {
        LOG->trace("stop");
        pthread_mutex_lock(&deadbolt);
        if (this->currentExecutionState) {
            this->currentExecutionState->stop();
        }
        pthread_mutex_unlock(&deadbolt);
    }

    int Statement::getId()
    {
        return this->model->getId();
    }

    int Statement::getPosition()
    {
        return this->model->getPosition();
    }

    std::string Statement::getType()
    {
        return this->getInfo().type;
    }

    int Statement::getParameterDefinitionCount()
    {
        return getParameterDefinitions()->size();
    }

    std::vector<ParameterDefinition*>* Statement::getParameterDefinitions()
    {
        return new std::vector<ParameterDefinition*>();
    }

    bool Statement::isValueTrue(std::string value)
    {
        return checkRegexMatch(value, "^(on|1|t(rue)?|y(es)?)$", false);
    }

    std::string Statement::isValueTrue(bool value)
    {
        return value ? "TRUE" : "FALSE";
    }

    bool Statement::checkParameterValidity(ExecutionState* state)
    {
        LOG->trace("checkParameterValidity()");
        std::vector<ParameterDefinition*>* parameters = getParameterDefinitions();
        for (std::vector<ParameterDefinition*>::iterator it = parameters->begin();
                it != parameters->end(); ++it) {
            LOG->trace("Checking parameter validity. key: '" + (*it)->key + "'");
            if (!checkRegexMatch(getParameter((*it)->key), (*it)->validation)) {
                if (NULL != state) {
                    state->getLogger()->warning(
                            SSTR(
                                    "Failed parameter check for " << (*it)->name << " with value '"
                                            << (*it)->key << "'"));
                }
                return false;
            }
        }
        return true;
    }

    bool Statement::checkRegexMatch(std::string value, std::string validation, bool caseSensitive)
    {
        if (validation == "") {
            return true;
        }
// TODO Take better consideration that value can have parameters.
        validation += SSTR("|" << VAR_BEGIN_CHAR << ".+" << VAR_END_CHAR);
        regex_t exp;
        int flags = caseSensitive ? REG_EXTENDED : REG_EXTENDED | REG_ICASE;
        if (0 != regcomp(&exp, validation.c_str(), flags)) {
            LOG->trace("Error compiling regular expression.");
            return false;
        }
        if (0 != regexec(&exp, value.c_str(), 0, NULL, 0)) {
            return false;
        }
        return true;
    }

    StatementModel* Statement::getModel() const
    {
        LOG->trace("getModel");
        return this->model;
    }

    std::string Statement::getParameter(std::string key, ExecutionState* state)
    {
        if (this->model->hasParameter(key)) {
            if (state == NULL)
                return this->model->getParameter(key);
            else
                return this->expandVariables(this->getParameter(key), state);
        } else {
            return getDefaultParameter(key);
        }
    }

    std::string Statement::getDefaultParameter(std::string key)
    {
        (void) key;
        return "";
    }

    std::string Statement::getSubParameter(std::string key, std::string subkey,
            ExecutionState* state)
    {
        LOG->trace("getSubParameter");
        return this->getParameter(this->combine(key, subkey), state);
    }

    void Statement::setParameter(std::string key, std::string value)
    {
        LOG->trace("setParameter");
        this->model->setParameter(key, value);
    }

    void Statement::setSubParameter(std::string key, std::string subkey, std::string value)
    {
        LOG->trace("setParameter");
        this->setParameter(combine(key, subkey), value);
    }

    std::string Statement::combine(std::string key, std::string subkey)
    {
        return key + "_" + subkey;
    }

// Finds variables within input and replaces with content.
    std::string Statement::expandVariables(std::string value, ExecutionState* state) const
    {
        // Since we are iterating from end to start, we are searching for the ending first
        // Initialize end position to be non-existent
        int endPosition = -1;
        for (int currentPosition = value.length() - 1; 0 <= currentPosition; currentPosition--) {
            switch (value.at(currentPosition)) {
                case VAR_END_CHAR:
                    // Found ending of variable
                    // If previous character was also an ending, assume this was escaped.
                    // Otherwise, let's discard any previous findings and record this one
                    if (endPosition == currentPosition + 1) {
                        endPosition = -1;
                    } else {
                        endPosition = currentPosition;
                    }
                    break;
                case VAR_BEGIN_CHAR:
                    // We just found a variable beginning.
                    // If we previously found an ending and the content is not empty, let's replace it.
                    if (endPosition != -1 && endPosition != currentPosition + 1) {
                        value.replace(currentPosition, endPosition - currentPosition + 1,
                                state->getUserProperty(
                                        value.substr(currentPosition + 1,
                                                endPosition - currentPosition - 1)));
                        endPosition = -1;
                    }
                    break;
                default:
                    // Nothing of interest
                    break;
            }
        }
        return value;
    }

    std::vector<statement_output_variable_definition*>* Statement::getExposedVariableDefinitions()
    {
        return NULL;
    }

    Statement* Statement::newBranchChild(std::string branchName, std::string type)
    {
        LOG->critical(
                "Called unimplemented method newBranchChild(" + branchName + "," + type + ")");
        // Implemented in derived class
        return NULL;
    }

    Statement* Statement::pasteBranchChild(std::string branchName)
    {
        LOG->critical("Called unimplemented method pasteBranchChild(" + branchName + ")");
        // Implemented in derived class
        return NULL;
    }

    std::vector<branch_info> Statement::getBranchInfo()
    {
        return this->branchInfo;
    }

    Statement* Statement::getBranch(std::string key, std::string type)
    {
        (void) key;
        (void) type;

        // Implemented in derived class
        return NULL;
    }

    Statement* Statement::getNextStatement() const
    {
        return nextStatement;
    }

    Statement* Statement::getLastStatement() const
    {
        if (NULL == this->getNextStatement()) {
            return (Statement*) this;
        } else {
            return this->getNextStatement()->getLastStatement();
        }
    }

    void Statement::initNextStatement(Statement* nextStatement)
    {
        this->nextStatement = nextStatement;
    }

    void Statement::setNextStatement(Statement* nextStatement)
    {
        this->model->setNextStatement(nextStatement->model);
        this->nextStatement = nextStatement;
        emit notifyChangePosition();
    }

    void Statement::unsetNextStatement()
    {
        this->model->unsetNextStatement();
        this->nextStatement = NULL;
        emit notifyChangePosition();
    }

    void Statement::unsetPreviousStatement()
    {
        this->model->unsetPreviousStatement();
        emit notifyChangePosition();
    }

    useconds_t Statement::getGranularity(ExecutionState* state)
    {
        useconds_t granularity = DEFAULT_TRIGGER_GRANULARITY_MICROSEC;
        std::string granularityString = getParameter("GRANULARITY", state);
        if (!granularityString.empty()) {
            try {
                // Parse and convert ms to us
                granularity = Common::StringToNumber<useconds_t>(granularityString) * 1000U;
            } catch (std::exception& e) {
                LOG->trace(SSTR("Could not extract granularity: " << e.what()));
                granularity = DEFAULT_TRIGGER_GRANULARITY_MICROSEC;
            }
        }
        // Maximum allowed granularity is 3600 seconds
        if (3600000000U < granularity)
            granularity = DEFAULT_TRIGGER_GRANULARITY_MICROSEC;
        return granularity;
    }

} /* namespace actiontrigger */
