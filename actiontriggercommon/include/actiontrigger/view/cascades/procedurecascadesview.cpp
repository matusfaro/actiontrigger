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

#include "actiontrigger/view/cascades/procedurecascadesview.hpp"

namespace actiontrigger
{

    Logger* ProcedureCascadesView::LOG = LoggerFactory::getInstance("ProcedureCascadesView");

    ProcedureCascadesView::ProcedureCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    ProcedureCascadesView::ProcedureCascadesView(Procedure* procedure) :
            QObject(procedure), procedureController(procedure), work(NULL)
    {
        LOG->trace("ProcedureCascadesView(procedure)");
        this->connect(procedure, SIGNAL(notifyChangeExecNum()), this,
                SLOT(propagateChangeExecNum()));
        this->connect(procedure, SIGNAL(notifyChangeIsRunning()), this,
                SLOT(propagateChangeIsRunning()));
    }

    ProcedureCascadesView::~ProcedureCascadesView()
    {
        LOG->trace("~ProcedureCascadesView");
    }

    Procedure* ProcedureCascadesView::getController() const
    {
        return this->procedureController;
    }

    QString ProcedureCascadesView::getUuid() const
    {
        LOG->trace("getUuid");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return "";
        }
        return QString::fromStdString(procedureController->getUuid());
    }

    QString ProcedureCascadesView::getFlag() const
    {
        LOG->trace("getFlag");
        return flag;
    }

    QString ProcedureCascadesView::getName() const
    {
        LOG->trace("getName");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return QString::fromStdString(procedureController->getName());
    }

    QString ProcedureCascadesView::getGroup() const
    {
        LOG->trace("getGroup");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return QString::fromStdString(procedureController->getGroup());
    }

    QString ProcedureCascadesView::getPriority() const
    {
        LOG->trace("getPriority");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return QString::fromStdString(procedureController->getPriority());
    }

    QString ProcedureCascadesView::getLogLevel() const
    {
        LOG->trace("getLogLevel");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return QString::fromStdString(procedureController->getLogLevel());
    }

    QString ProcedureCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return QString::fromStdString(procedureController->getDescription());
    }

    void ProcedureCascadesView::setFlag(QString flag)
    {
        LOG->trace("setFlag");
        this->flag = flag;
        emit notifyChangeFlag();
    }

    void ProcedureCascadesView::setName(QString name)
    {
        LOG->trace("setName");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setName(name.toStdString());
        emit notifyChangeName();
    }

    void ProcedureCascadesView::setGroup(QString group)
    {
        LOG->trace("setGroup");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setGroup(group.toStdString());
        emit notifyChangeGroup();
    }

    void ProcedureCascadesView::setPriority(QString priority)
    {
        LOG->trace("setPriority");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setPriority(priority.toStdString());
        emit notifyChangePriority();
    }

    void ProcedureCascadesView::setLogLevel(QString level)
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        procedureController->setLogLevel(level.toStdString());
        emit notifyChangeLogLevel();
    }

    void ProcedureCascadesView::setDescription(QString description)
    {
        LOG->trace("setDescription");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setDescription(description.toStdString());
        emit notifyChangeDescription();
    }

    bool ProcedureCascadesView::getFav() const
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return procedureController->getSetting("FAVOURITE").compare("TRUE") == 0;
    }

    void ProcedureCascadesView::setFav(bool value)
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setSetting("FAVOURITE", value ? "TRUE" : "FALSE");
        emit notifyChangeFav();
    }

    bool ProcedureCascadesView::getAutoStart() const
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return procedureController->getSetting("AUTO_START").compare("TRUE") == 0;
    }

    void ProcedureCascadesView::setAutoStart(bool value)
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setSetting("AUTO_START", value ? "TRUE" : "FALSE");
        emit notifyChangeAutoStart();
    }

    bool ProcedureCascadesView::getRunOnce() const
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return procedureController->getSetting("RUN_ONCE").compare("TRUE") == 0;
    }

    void ProcedureCascadesView::setRunOnce(bool value)
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setSetting("RUN_ONCE", value ? "TRUE" : "FALSE");
        emit notifyChangeRunOnce();
    }

    bb::cascades::ArrayDataModel* ProcedureCascadesView::getExecutionLog() const
    {
        LOG->trace("getExecutionLog");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return this->procedureController->getConnectExecutionModel()->getLogOutDataModel();
    }

    bb::cascades::GroupDataModel* ProcedureCascadesView::getVariableList() const
    {
        LOG->trace("getVariableList");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return this->procedureController->getVariablesDataModel();
    }

    QObjectList ProcedureCascadesView::getStatements()
    {
        LOG->trace("getActions");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return QObjectList();
        }
        return QList<QObject*>::fromVector(
                QVector<QObject*>::fromStdVector(
                        *UserInterfaceFactory::getInstance()->displayActions(procedureController,
                                procedureController->getActions())));
    }

    QObject* ProcedureCascadesView::newAction(QString type)
    {
        LOG->trace("newAction");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return UserInterfaceFactory::getInstance()->displayStatement(this->procedureController,
                this->procedureController->newAction(type.toStdString()));
    }

    QObject* ProcedureCascadesView::pasteAction()
    {
        LOG->trace("newAction");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return NULL;
        }
        return UserInterfaceFactory::getInstance()->displayStatement(this->procedureController,
                this->procedureController->pasteAction());
    }

    int ProcedureCascadesView::getExecNum() const
    {
        LOG->trace("getExecNum");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return 0;
        }
        return this->procedureController->getExecNum();
    }

    bool ProcedureCascadesView::getIsRunning(int executionNumber) const
    {
        LOG->trace("getIsRunning");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return this->procedureController->isRunning(executionNumber);
    }

    QString ProcedureCascadesView::getIsRunningAsString() const
    {
        return getIsRunning() ? "1" : "2";
    }

    bool ProcedureCascadesView::getBackgrounded() const
    {
        LOG->trace("getBackgrounded");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return procedureController->getSetting("BACKGROUNDED").compare("TRUE") == 0;
    }

    QString ProcedureCascadesView::getBackgroundedAsString() const
    {
        return getBackgrounded() ? "1" : "2";
    }

    void ProcedureCascadesView::setBackgrounded(bool value)
    {
        LOG->trace("setBackgrounded");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->setSetting("BACKGROUNDED", value ? "TRUE" : "FALSE");
        emit notifyChangeBackgrounded();
    }

    void ProcedureCascadesView::refreshStatementPosition()
    {
        LOG->trace("stop");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        // Retrieve position for all children. If position changed, this forces an emit to occur.
        std::vector<StatementCascadesView*>* actions =
                (std::vector<StatementCascadesView*>*) UserInterfaceFactory::getInstance()->displayActions(
                        procedureController, procedureController->getActions());
        for (std::vector<StatementCascadesView*>::iterator it = actions->begin();
                it != actions->end(); ++it) {
            (*it)->refreshPosition();
        }
    }

    bool ProcedureCascadesView::start(bool debug)
    {
        LOG->trace("start");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        // Prepare start, ensure no other procedure is about to start
        if (this->procedureController->prepareStart(debug)) {
            work = new ProcedureWork(procedureController);
            work->start();
            return true;
        }
        return false;
    }

    void ProcedureCascadesView::propagateChangeExecNum()
    {
        emit notifyChangeExecNum();
    }

    void ProcedureCascadesView::propagateChangeIsRunning()
    {
        emit notifyChangeIsRunning();
    }

    void ProcedureCascadesView::stop()
    {
        LOG->trace("stop");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        procedureController->stop();
        work = NULL;
    }

    void ProcedureCascadesView::remove()
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return;
        }
        this->procedureController->remove();
        procedureController = NULL;
    }

    bool ProcedureCascadesView::filter(QString filter)
    {
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return false;
        }
        return filter.indexOf(QString::fromStdString(procedureController->getName()), 0,
                Qt::CaseInsensitive);
    }

    QObjectList ProcedureCascadesView::getExposedVariableDefinitions()
    {
        LOG->trace("getExposedVariableDefinitions");
        if (procedureController == NULL) {
            LOG->warning("Attempted to access deleted object");
            return QObjectList();
        }
        std::list<std::string> variableNames;
        QObjectList* qobjectList = new QObjectList();
        // Get global variables
        std::vector<statement_output_variable_definition*>* varsList =
                ExecutionState::getDefaultExposedVariableDefinitions();
        if (NULL != varsList) {
            for (std::vector<statement_output_variable_definition*>::iterator it =
                    varsList->begin(); it != varsList->end(); ++it) {
                if (std::find(variableNames.begin(), variableNames.end(), (*it)->name)
                        == variableNames.end()) {
                    variableNames.push_back((*it)->name);
                    qobjectList->push_back(
                            (QObject*) new VariableDefinitionCascades("Global", *it));
                }
            }
        }

        // Recursively get variables from statements
        if (this->procedureController->getAction())
            appendExposedVariableDefinitions(qobjectList, variableNames,
                    this->procedureController->getAction());

        return *qobjectList;
    }

    void ProcedureCascadesView::appendExposedVariableDefinitions(QObjectList* qobjectList,
            std::list<std::string> variableNames, Statement* statement)
    {
        // Iterate statement chain
        while (statement) {
            std::string name = statement->getInfo().name;
            std::vector<statement_output_variable_definition*>* varsList = NULL;

            // Get vars from statement
            varsList = statement->getExposedVariableDefinitions();
            if (varsList) {
                for (std::vector<statement_output_variable_definition*>::iterator it =
                        varsList->begin(); it != varsList->end(); ++it) {
                    if (std::find(variableNames.begin(), variableNames.end(), (*it)->name)
                            == variableNames.end()) {
                        variableNames.push_back((*it)->name);
                        qobjectList->push_back(
                                (QObject*) new VariableDefinitionCascades(name, *it));
                    }
                }
                delete varsList;
            }

            // Get vars from branches
            std::vector<branch_info> branchInfo = statement->getBranchInfo();
            for (std::vector<branch_info>::iterator it = branchInfo.begin(); it != branchInfo.end();
                    ++it) {
                Statement* branchStatement = statement->getBranch((*it).key, (*it).type);
                // Recursively iterate branch chains
                if (branchStatement)
                    appendExposedVariableDefinitions(qobjectList, variableNames, branchStatement);
            }

            statement = statement->getNextStatement();
        }
    }

} /* namespace actiontrigger */
