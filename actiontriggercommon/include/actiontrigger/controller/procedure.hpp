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

#ifndef PROCEDURE_HPP_
#define PROCEDURE_HPP_

#include <QObject>
#include "actiontrigger/model/logger/blackberry/connectexecutionmodel.hpp"
#include <QVariant>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/databasefactory.hpp"
#include "actiontrigger/model/logger/blackberry/datamodellogger.hpp"
#include "actiontrigger/model/database/proceduremodel.hpp"
#include "actiontrigger/controller/servicemanagement.hpp"
#include "actiontrigger/controller/exception.hpp"
#include "actiontrigger/controller/executionstate.hpp"
#include <pthread.h>
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/database/databasecommon.hpp"
#include "actiontrigger/model/database/sqlite/proceduredbmodel.hpp"
#include "actiontrigger/view/cascades/keyvaluecascades.hpp"
#include <bb/cascades/GroupDataModel>
#include "actiontrigger/view/cascades/slidingdatamodel.hpp"
#include <bb/cascades/ArrayDataModel>
#include "actiontrigger/controller/globalresources.hpp"

#define DEFAULT_VALUE_RUN_ONCE "TRUE"
#define DEFAULT_VALUE_AUTO_START "FALSE"
#define DEFAULT_VALUE_FAVOURITE "FALSE"

namespace actiontrigger
{

    class Procedure: public QObject
    {
    Q_OBJECT

    public:
        explicit Procedure(std::string name);
        Procedure(ProcedureModel* model);
        explicit Procedure(QVariant dataVariant);
        virtual ~Procedure();

        ExecutionState* createExecution();
        ExecutionState* prepareStart(bool debug = false, bool forceRunLocal = false);
        void start(); // Need to call prepareStart() first
        // Used to create dummy execution to signify this procedure is already running headless
        void alreadyRunningHeadless();
        ConnectExecutionModel* getConnectExecutionModel(); // execution log data model

        bool checkParameterValidity(ExecutionState* state);

        void remove();
        void removeChild(Statement* statement);

        int getId() const;
        std::string getUuid();
        std::string getName() const;
        std::string getGroup() const;
        std::string getPriority() const;
        std::string getDescription() const;
        void setName(std::string name);
        void setGroup(std::string group);
        void setPriority(std::string priority);
        void setDescription(std::string description);
        bool isRunning(int executionNumber = 0);
        int getExecNum();
        std::string getLogLevel();
        void setLogLevel(std::string loglevel);

        Action* newAction(std::string type);
        Action* pasteAction();
        Action* getAction() const;
        std::vector<Action*>* getActions() const;
        void setAction(Action* action);
        void unsetAction();
        bool moveUp(Statement* statement);
        bool moveDown(Statement* statement);

        std::string getSetting(std::string name) const;
        void setSetting(std::string name, std::string value);

        bb::cascades::GroupDataModel* getVariablesDataModel();

        QVariant toQVariant() const;

        ProcedureModel* getModel();

    Q_SIGNALS:
        void notifyChangeExecNum();
        void notifyChangeIsRunning();
        void signalAddVariable(QObject* msg);

    public slots:
        void stop();
        void displayExecutionVariable(QString key, QString value);
    private:
        void init(ProcedureModel* model);

        ProcedureModel* model;
        Action* action;
        static Logger* LOG;

        Action* appendAction(StatementModel* newModel);

        int currentExecutionNumber;
        ExecutionState* pendingExecutionState;
        ExecutionState* currentExecutionState;
        pthread_mutex_t deadbolt;
        pthread_mutexattr_t mattr;

        std::string getSettingDefaultValue(std::string name) const;

        ConnectExecutionModel connectExecutionModel;

        bb::cascades::GroupDataModel variablesDataModel;
        std::map<std::string, KeyValueCascades*>* variables;
    };

} /* namespace actiontrigger */
#endif /* PROCEDURE_HPP_ */
