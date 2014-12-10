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

#include "actiontrigger/view/cascades/statementcascadesview.hpp"

namespace actiontrigger
{

    Logger* StatementCascadesView::LOG = LoggerFactory::getInstance("StatementCascadesView");

    StatementCascadesView::StatementCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    StatementCascadesView::StatementCascadesView(Procedure* parentProcedure, Statement* statement,
            std::string branchName, Statement* parentStatement) :
            parentProcedure(parentProcedure), statementController(statement), branchName(
                    branchName), parentStatement(parentStatement), variableList(
            NULL)
    {
        LOG->trace("StatementCascadesView(statement)");
        variables = new std::map<std::string, KeyValueCascades*>();
        this->connect(statement, SIGNAL(notifyChangePosition()), this,
                SLOT(propagateChangePosition()));
    }

    StatementCascadesView::~StatementCascadesView()
    {
    }

    qint32 StatementCascadesView::getId() const
    {
        LOG->trace("getId");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return 0;
        }
        return (qint32) this->statementController->getId();
    }

    QString StatementCascadesView::getName() const
    {
        LOG->trace("getName");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return "";
        }
        return QString::fromStdString(this->statementController->getInfo().name);
    }

    QString StatementCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return "";
        }
        return QString::fromStdString(this->statementController->getInfo().description);
    }

    QString StatementCascadesView::getType() const
    {
        LOG->trace("getType");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return "";
        }
        return QString::fromStdString(this->statementController->getInfo().type);
    }

    void StatementCascadesView::refreshPosition()
    {
        emit notifyChangePosition();
        emit notifyChangeIsFirst();
        emit notifyChangeIsLast();
    }

    qint32 StatementCascadesView::getPosition()
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return 0;
        }
        return (qint32) this->statementController->getPosition();
    }

    QObjectList StatementCascadesView::getParameters()
    {
        LOG->trace("getParameters");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return *new QObjectList();
        }
        return UserInterfaceFactory::getInstance()->displayParameters(
                (Statement*) statementController, false);
    }

    bool StatementCascadesView::validate()
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return this->statementController->checkParameterValidity();
    }

    bool StatementCascadesView::moveUp()
    {
        bool success = false;
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        if (parentStatement == NULL) {
            // Procedure is our immediate parent
            success = parentProcedure->moveUp(statementController);
        } else {
            // We have a statement as our immediate parent, assuming it's a branch
            success = ((Branch*) parentStatement)->moveUp(branchName, statementController);
        }
        if (success) {
            refreshPosition();
        }
        return success;
    }

    bool StatementCascadesView::moveDown()
    {
        bool success = false;
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        if (parentStatement == NULL) {
            // Procedure is our immediate parent
            success = parentProcedure->moveDown(statementController);
        } else {
            // We have a statement as our immediate parent, assuming it's a branch
            success = ((Branch*) parentStatement)->moveDown(branchName, statementController);
        }
        if (success) {
            refreshPosition();
        }
        return success;
    }

    bool StatementCascadesView::getIsFirst() const
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return NULL == this->statementController->getModel()->getPreviousStatement();
    }

    bool StatementCascadesView::getIsLast() const
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return NULL == this->statementController->getModel()->getNextStatement();
    }

    void StatementCascadesView::propagateChangePosition()
    {
        refreshPosition();
    }

    QObjectList StatementCascadesView::getBranches()
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return *new QObjectList();
        }
        return UserInterfaceFactory::getInstance()->displayBranches(
                statementController->getBranchInfo());
    }

    QObjectList StatementCascadesView::getBranch(QString key, QString type)
    {
        LOG->trace("getBranch");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return *new QObjectList();
        }
        QObjectList* qobjectList = new QObjectList();
        Statement* statement = statementController->getBranch(key.toStdString(),
                type.toStdString());
        if (statement != NULL) {
            LOG->trace("Loading branch: " + statement->getInfo().name);
            do {
                qobjectList->push_back(
                        UserInterfaceFactory::getInstance()->displayStatement(parentProcedure,
                                statement, key.toStdString(), statementController));
                statement = statement->getNextStatement();
            } while (statement != NULL);
        }
        return *qobjectList;
    }

    QObject* StatementCascadesView::getNext()
    {
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        if (!statementController->getNextStatement())
            return NULL;
        return UserInterfaceFactory::getInstance()->displayStatement(parentProcedure,
                statementController->getNextStatement(), branchName, parentStatement);
    }

    bool StatementCascadesView::start(bb::cascades::ArrayDataModel* executionLog,
            bb::cascades::GroupDataModel* variableList)
    {
        LOG->trace("start");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        // Disconnect previous variable data model
        if (this->variableList) {
            this->variableList->disconnect(this, SIGNAL(signalAddVariable(QObject*)),
                    this->variableList, SLOT(insert(QObject*)));
        }
        // Connect to new variable data model
        this->variableList = variableList;
        this->variableList->connect(this, SIGNAL(signalAddVariable(QObject*)), this->variableList,
                SLOT(insert(QObject*)));
//    for (std::map<std::string, KeyValueCascades*>::iterator iter = variables->begin(); iter != variables->end(); ++iter) {
//        delete iter->second;
//    }
        this->variables->clear();
        executionLog->clear();
        ConnectExecutionModel* connectExecutionModel = new ConnectExecutionModel(executionLog);
        connectExecutionModel->connect(connectExecutionModel,
                SIGNAL(signalAddVariable(QString, QString)), this,
                SLOT(displayExecutionVariable(QString, QString)), Qt::QueuedConnection);
        // Prepare start, ensure no other procedure is about to start
        if (this->statementController->prepareStart(connectExecutionModel, true)) {
            Work* work = new Work((UnitOfWork*) new StatementRun(statementController));
            work->start();
            return true;
        }
        return false;
    }

    void StatementCascadesView::displayExecutionVariable(QString key, QString value)
    {
        if (variables->find(key.toStdString()) == variables->end()) {
            KeyValueCascades* keyValue = new KeyValueCascades(key, value);
            emit signalAddVariable((QObject*) keyValue);
            variables->insert(
                    std::pair<std::string, KeyValueCascades*>(key.toStdString(), keyValue));
        } else {
            variables->find(key.toStdString())->second->setValue(value);
        }
    }

    void StatementCascadesView::stop()
    {
        LOG->trace("stop");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        statementController->stop();
        // TODO Error handling
    }

    QObject* StatementCascadesView::newBranchChild(QString branchName, QString type)
    {
        LOG->trace("newBranchChild");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return (QObject*) UserInterfaceFactory::getInstance()->displayStatement(
                this->parentProcedure,
                this->statementController->newBranchChild(branchName.toStdString(),
                        type.toStdString()), branchName.toStdString(), this->statementController);
    }

    QObject* StatementCascadesView::pasteBranchChild(QString branchName)
    {
        LOG->trace("pasteBranchChild");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return (QObject*) UserInterfaceFactory::getInstance()->displayStatement(
                this->parentProcedure,
                this->statementController->pasteBranchChild(branchName.toStdString()),
                branchName.toStdString(), this->statementController);
    }

    void StatementCascadesView::copy()
    {
        LOG->trace("copy");
        if (statementController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        GlobalResources::get()->clipboardStore(
                this->statementController->getModel()->toQVariant(true),
                this->statementController->getInfo().isTrigger);
    }

    void StatementCascadesView::remove()
    {
        Statement* deleteStatement = statementController;
        statementController = NULL;
        if (parentStatement == NULL) {
            // Procedure is our immediate parent
            parentProcedure->removeChild(deleteStatement);
        } else {
            // We have a statement as our immediate parent, assuming it's a branch
            ((Branch*) parentStatement)->removeChild(branchName, deleteStatement);
        }
    }

} /* namespace actiontrigger */
