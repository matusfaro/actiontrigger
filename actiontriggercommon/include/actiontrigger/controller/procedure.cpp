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

#include "actiontrigger/controller/procedure.hpp"
#include <actiontrigger/controller/headlessconnector.hpp>

namespace actiontrigger
{

    Logger* Procedure::LOG = LoggerFactory::getInstance("Procedure");

    Procedure::Procedure(std::string name) :
            QObject(0)
    {
        LOG->trace("Procedure");
        ProcedureModel* model = ProcedureDBModel::create(READ_CACHE, name, NULL);
        this->init(model);
    }

    Procedure::Procedure(ProcedureModel* model) :
            QObject(0)
    {
        LOG->trace("Procedure");
        this->init(model);
    }

    Procedure::Procedure(QVariant dataVariant) :
            QObject(0)
    {
        LOG->trace("Procedure");

        // Create model
        ProcedureModel* model = ProcedureDBModel::create(READ_CACHE, dataVariant);

        // Construct
        this->init(model);
    }

    void Procedure::init(ProcedureModel* model)
    {
        LOG->trace("init");
        pendingExecutionState = NULL;
        currentExecutionState = NULL;

        this->currentExecutionNumber = 0;

        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&deadbolt, &mattr);

        this->model = model;
        if (this->model->getStatement()) {
            this->action = ServiceManagement::getInstance()->loadAction(
                    this->model->getStatement());
        } else {
            this->action = NULL;
        }

        variablesDataModel.connect(this, SIGNAL(signalAddVariable(QObject*)), &variablesDataModel,
                SLOT(insert(QObject*)));
        connectExecutionModel.connect(&connectExecutionModel,
                SIGNAL(signalAddVariable(QString, QString)), this,
                SLOT(displayExecutionVariable(QString, QString)), Qt::QueuedConnection);
        variables = new std::map<std::string, KeyValueCascades*>();
    }

    Procedure::~Procedure()
    {
        LOG->trace("~Procedure");
        pthread_mutex_destroy(&deadbolt);
        pthread_mutexattr_destroy(&mattr);
        connectExecutionModel.clear();
        delete variables;
    }

    ConnectExecutionModel* Procedure::getConnectExecutionModel()
    {
        return &connectExecutionModel;
    }

    void Procedure::alreadyRunningHeadless()
    {
        pthread_mutex_lock(&deadbolt);

        if (this->currentExecutionState) {
            if (this->currentExecutionState->getIsRemote()) {
                // Nothing to do, Already started
                return;
            } else {
                // Stop currently running execution
                this->currentExecutionState->stop();
            }
        }

        // Increase execution number
        int myExecutionNumber = ++currentExecutionNumber;
        emit notifyChangeExecNum();

        this->currentExecutionState = new ExecutionState(myExecutionNumber,
                new DataModelLogger(&connectExecutionModel, NONE), getUuid(), true, getId());

        pendingExecutionState = NULL;

        pthread_mutex_unlock(&deadbolt);
    }

    ExecutionState* Procedure::prepareStart(bool debug, bool forceRunLocal)
    {
        LogLevel loglevel = debug ? TRACE : DEFAULT_LOGLEVEL;
        pthread_mutex_lock(&deadbolt);
        if (pendingExecutionState == NULL) {
            // Increase execution number
            int myExecutionNumber = ++currentExecutionNumber;
            emit notifyChangeExecNum();

            // Setup logging
            DataModelLogger* logger = new DataModelLogger(&connectExecutionModel, loglevel);
            connectExecutionModel.clear();

            // Setup variables
            variablesDataModel.clear();
            for (std::map<std::string, KeyValueCascades*>::iterator iter = variables->begin();
                    iter != variables->end(); ++iter) {
                delete iter->second;
            }
            variables->clear();

            // Prepare execution
            bool runInBackground =
                    HeadlessConnector::HEADLESS_ENABLED && !forceRunLocal
                            && getSetting("BACKGROUNDED").compare("TRUE") == 0 ? true : false;
            ExecutionState* newExecutionState = new ExecutionState(myExecutionNumber, logger,
                    getUuid(), runInBackground, getId());
            newExecutionState->setExecutionProperty("RUN_ONCE", this->getSetting("RUN_ONCE"));
            pendingExecutionState = newExecutionState;

            pthread_mutex_unlock(&deadbolt);
            return newExecutionState;
        } else {
            pthread_mutex_unlock(&deadbolt);
            return NULL;
        }
    }

    void Procedure::start()
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
        emit notifyChangeIsRunning();

        // Get execution number of this thread
        myExecutionNumber = myState->getExecutionNumber();

        // Setup logging
        DataModelLogger* RUNLOG = myState->getLogger();

        if (myState->getIsRemote()) {
            RUNLOG->debug("Execution queued to start in background, suppressing all logging...");
            HeadlessConnector::get()->procedureStart(getId());
        } else {
            RUNLOG->info("Execution started");
            try {
                // Check if we have action(s)
                if (!this->action) {
                    RUNLOG->debug("There are no actions defined");
                }

                // Check if we should continue
                if (!myState->isRunning())
                    throw ExecutionAbortedException();

                // Validate parameters
                if (!checkParameterValidity(myState)) {
                    RUNLOG->error("Parameter check failed");
//                throw ExecutionAbortedException();
                }

                // Loop until execution expires
                bool runOnce = myState->getExecutionProperty("RUN_ONCE") == "TRUE" ? true : false;
                do {
                    // Check if we should continue
                    if (!myState->isRunning())
                        throw ExecutionAbortedException();

                    // Execute Actions
                    LOG->trace(
                            SSTR("Thread #" << myExecutionNumber << " >>> " << "Execute Actions"));
                    if (this->action)
                        this->action->run(myState);

                } while (!runOnce);
                // This should only happen if runOnce is true, otherwise
                RUNLOG->info(SSTR("Finished execution"));
            } catch (const ExecutionAbortedException &e) {
                RUNLOG->popAllLayers();
                RUNLOG->info(SSTR("Execution stopped"));
            } catch (const LoopContinueException &e) {
                RUNLOG->popAllLayers();
                RUNLOG->debug(SSTR("Caught loop continue, stopping procedure"));
            } catch (const LoopBreakException &e) {
                RUNLOG->popAllLayers();
                RUNLOG->debug(SSTR("Caught loop break, stopping procedure"));
            } catch (const std::exception &e) {
                RUNLOG->error(SSTR("Unexpected error: " << e.what()));
                LOG->error(
                        SSTR(
                                "Thread #" << myExecutionNumber << " >>> " << "Unhandled exception caught: " << e.what()));
            } catch (...) {
                RUNLOG->error(SSTR("Unexpected unidentified error"));
                LOG->error(
                        SSTR(
                                "Thread #" << myExecutionNumber << " >>> " << "Unhandled unknown exception caught!"));
            }
            // Shut it all down
            pthread_mutex_lock(&deadbolt);
            // If this execution is not overlapping clear the execution
            if (currentExecutionState == myState) {
                currentExecutionState = NULL;
            }
            pthread_mutex_unlock(&deadbolt);
            // Notify of running change regardless if it was changed (otherwise the UI misses the change)
            emit notifyChangeIsRunning();
            // Safe to cleanup now
            myState->cleanup();
            delete myState;
        }
    }

    bool Procedure::checkParameterValidity(ExecutionState* state)
    {
        return (!action || action->checkParameterValidityAndAllChildren(state));
    }

    void Procedure::stop()
    {
        LOG->trace("stop");
        pthread_mutex_lock(&deadbolt);
        ExecutionState* state = this->currentExecutionState;
        if (state) {
            state->stop();
            this->connectExecutionModel.clear();

            this->currentExecutionState = NULL;
        }
        pthread_mutex_unlock(&deadbolt);
        emit notifyChangeIsRunning();
    }

    int Procedure::getExecNum()
    {
        return this->currentExecutionNumber;
    }

    ProcedureModel* Procedure::getModel()
    {
        return this->model;
    }

    std::string Procedure::getLogLevel()
    {
        LogLevel loglevel = DEFAULT_LOGLEVEL;
        pthread_mutex_lock(&deadbolt);
        if (this->currentExecutionState) {
            loglevel = this->currentExecutionState->getLogger()->getLoglevel();
        }
        pthread_mutex_unlock(&deadbolt);
        return Logger::parseLogLevelName(loglevel);
    }

    void Procedure::setLogLevel(std::string loglevel)
    {
        pthread_mutex_lock(&deadbolt);
        if (this->currentExecutionState) {
            this->currentExecutionState->getLogger()->setLoglevel(Logger::parseLogLevel(loglevel));
        }
        pthread_mutex_unlock(&deadbolt);
    }

    bool Procedure::isRunning(int executionNumber)
    {
        LOG->trace("isRunning");
        bool isRunning = false;
        pthread_mutex_lock(&deadbolt);
        // Pending executions count as running
        if (this->pendingExecutionState) {
            if (executionNumber == 0) {
                isRunning = true;
            } else {
                isRunning = this->pendingExecutionState->getExecutionNumber() == executionNumber;
            }
        }
        // If no pending executions, check current execution state
        else if (this->currentExecutionState) {
            if (executionNumber == 0) {
                isRunning = this->currentExecutionState->isRunning();
            } else {
                isRunning = this->pendingExecutionState->getExecutionNumber() == executionNumber
                        && this->currentExecutionState->isRunning();
            }
        }
        pthread_mutex_unlock(&deadbolt);
        LOG->trace(SSTR("Running state: " << (isRunning ? "running" : "stopped")));
        return isRunning;
    }

    void Procedure::remove()
    {
        LOG->trace("remove");
        std::vector<Procedure*>* procedures = GlobalResources::get()->getProcedures();
        procedures->erase(std::remove(procedures->begin(), procedures->end(), this),
                procedures->end());
        this->model->remove();
    }

    void Procedure::removeChild(Statement* statement)
    {
        LOG->trace("removeChild");
        if (this->action == NULL) {
            LOG->critical("Cannot remove action, does not have any actions in procedure");
            return;
        } else if (this->action->getId() == statement->getId()) {
            if (NULL == action->getNextAction()) {
                unsetAction();
            } else {
                setAction(action->getNextAction());
            }
        } else {
            Action* previousAction = this->action;
            Action* currentAction = this->action->getNextAction();
            if (currentAction == NULL) {
                LOG->critical("Cannot remove action, does not exist as part of this procedure");
                return;
            }
            while (statement->getId() != currentAction->getId()) {
                previousAction = currentAction;
                currentAction = currentAction->getNextAction();
                if (currentAction == NULL) {
                    LOG->critical("Cannot remove action, does not exist as part of this procedure");
                    return;
                }
            }
            if (NULL == currentAction->getNextAction()) {
                previousAction->unsetNextStatement();
            } else {
                previousAction->setNextStatement(currentAction->getNextStatement());
            }
        }
        statement->remove();
    }

    int Procedure::getId() const
    {
        LOG->trace("getId");
        return model->getId();
    }

    std::string Procedure::getUuid()
    {
        LOG->trace("getUuid");
        std::string uuid = model->getSetting("UUID");
        if (uuid == "") {
            // Generate and set UUID
            uuid = Common::genUuidStr();
            model->setSetting("UUID", uuid);
        }
        return uuid;
    }

    std::string Procedure::getName() const
    {
        LOG->trace("getName");
        return model->getName();
    }

    void Procedure::setName(std::string name)
    {
        LOG->trace("setName");
        model->setName(name);
    }

    std::string Procedure::getGroup() const
    {
        LOG->trace("getGroup");
        return model->getSetting("GROUP");
    }

    void Procedure::setGroup(std::string group)
    {
        LOG->trace("setGroup");
        model->setSetting("GROUP", group);
    }

    std::string Procedure::getPriority() const
    {
        LOG->trace("getPriority");
        std::string priority = model->getSetting("PRIORITY");
        return priority == "" ? "NORMAL" : priority;
    }

    void Procedure::setPriority(std::string priority)
    {
        LOG->trace("setPriority");
        model->setSetting("PRIORITY", priority);
    }

    std::string Procedure::getDescription() const
    {
        LOG->trace("getDescription");
        return model->getSetting("DESCRIPTION");
    }

    void Procedure::setDescription(std::string description)
    {
        LOG->trace("setDescription");
        model->setSetting("DESCRIPTION", description);
    }

    std::vector<Action*>* Procedure::getActions() const
    {
        LOG->trace("getActions");
        std::vector<Action*>* actions = new std::vector<Action*>();
        Action* nextAction = this->getAction();
        while (nextAction) {
            LOG->trace("action: " + nextAction->getInfo().name);
            actions->push_back(nextAction);
            nextAction = nextAction->getNextAction();
        }
        return actions;
    }

    Action* Procedure::getAction() const
    {
        LOG->trace("getAction");
        return this->action;
    }

    void Procedure::setAction(Action* action)
    {
        LOG->trace("setAction");
        this->action = action;
        this->model->setStatement(action->getModel());
    }

    void Procedure::unsetAction()
    {
        LOG->trace("unsetAction");
        this->action = NULL;
        this->model->unsetStatement();
    }

    std::string Procedure::getSetting(std::string name) const
    {
        std::string value = model->getSetting(name);
        if (value.empty()) {
            value = getSettingDefaultValue(name);
            LOG->trace(SSTR("getSetting defaulted " << name << "=" << value));
        } else {
            LOG->trace(SSTR("getSetting retrieved " << name << "=" << value));
        }
        return value;
    }

    std::string Procedure::getSettingDefaultValue(std::string name) const
    {
        LOG->trace("getSettingDefaultValue");
        if (name.compare("RUN_ONCE") == 0) {
            return DEFAULT_VALUE_RUN_ONCE;
        } else if (name.compare("AUTO_START") == 0) {
            return DEFAULT_VALUE_AUTO_START;
        } else if (name.compare("FAVOURITE") == 0) {
            return DEFAULT_VALUE_FAVOURITE;
        }
        return "";
    }

    void Procedure::setSetting(std::string name, std::string value)
    {
        LOG->trace("setSetting");
        model->setSetting(name, value);
    }

    Action* Procedure::newAction(std::string type)
    {
        return appendAction(StatementDBModel::create(READ_CACHE, type));
    }

    Action* Procedure::pasteAction()
    {
        return appendAction(
                StatementDBModel::create(READ_CACHE, GlobalResources::get()->clipboardRetrieve()));
    }

    Action* Procedure::appendAction(StatementModel* newModel)
    {
        Action* newAction = ServiceManagement::getInstance()->loadAction(newModel);
        if (NULL == this->action) {
            setAction(newAction);
        } else {
            this->action->getLastStatement()->setNextStatement(newAction);
        }
        return newAction;
    }

    bool Procedure::moveDown(Statement* moveDownStatement)
    {
        return moveDownStatement->getNextStatement() == NULL ?
                false : moveUp(moveDownStatement->getNextStatement());
    }

    bool Procedure::moveUp(Statement* statement)
    {
        if (this->action->getId() == statement->getId()) {
            LOG->error("Statement already at the top");
            return false;
        }
        if (this->action->getNextStatement() == NULL) {
            LOG->error("Statement not found");
            return false;
        }

        // Holds pointers to four consecutive actions
        Statement* previousStatement = NULL; // Needs to be found
        Statement* moveDownStatement = (Statement*) this->action; // Needs to be found
        Statement* moveUpStatement = statement;
        Statement* nextStatement = moveUpStatement->getNextStatement();

        // Find the previousStatement and moveDownStatement
        while (moveDownStatement->getNextStatement()->getId() != moveUpStatement->getId()) {
            previousStatement = moveDownStatement;
            moveDownStatement = moveDownStatement->getNextStatement();
            if (moveDownStatement == NULL) {
                LOG->error("Statement not found");
                return false;
            }
        }

        // Perform the swap
        if (NULL == previousStatement) {
            // Is our child
            this->setAction((Action*) moveUpStatement);
        } else {
            previousStatement->setNextStatement(moveUpStatement);
        }
        moveUpStatement->setNextStatement(moveDownStatement);
        if (NULL == nextStatement) {
            moveDownStatement->unsetNextStatement();
        } else {
            moveDownStatement->setNextStatement(nextStatement);
        }

        return true;
    }

    bb::cascades::GroupDataModel* Procedure::getVariablesDataModel()
    {
        LOG->trace("getVariablesDataModel");
        return static_cast<bb::cascades::GroupDataModel*>(&variablesDataModel);
    }

    void Procedure::displayExecutionVariable(QString key, QString value)
    {
        if (variables->find(key.toStdString()) == variables->end()) {
            KeyValueCascades* keyValue = new KeyValueCascades(key, value, this);
            emit signalAddVariable((QObject*) keyValue);
            variables->insert(
                    std::pair<std::string, KeyValueCascades*>(key.toStdString(), keyValue));
        } else {
            variables->find(key.toStdString())->second->setValue(value);
        }
    }

    QVariant Procedure::toQVariant() const
    {
        return this->model->toQVariant();
    }

} /* namespace actiontrigger */
