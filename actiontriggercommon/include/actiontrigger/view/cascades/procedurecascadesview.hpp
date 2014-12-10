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

#ifndef PROCEDURECASCADESVIEW_HPP_
#define PROCEDURECASCADESVIEW_HPP_

#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/view/cascades/statementcascadesview.hpp"
#include "actiontrigger/view/cascades/procedurework.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/view/cascades/variabledefinitioncascades.hpp"
#include "actiontrigger/controller/services/statement.hpp"

#include <QBool>
#include <QObject>
#include <QObjectList>
#include <QList>
#include <QVector>
#include <vector>
#include <bb/cascades/GroupDataModel>

#include <bb/cascades/ListView>
#include "qt4/QtDeclarative/qdeclarative.h"

#include "actiontrigger/view/userinterfacefactory.hpp"
#include "actiontrigger/view/userinterface.hpp"

#include "actiontrigger/view/cascades/toolscascades.hpp"

namespace actiontrigger
{

    class ProcedureCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString uuid READ getUuid FINAL)
    Q_PROPERTY (QString flag READ getFlag WRITE setFlag NOTIFY notifyChangeFlag FINAL)
    Q_PROPERTY (QString name READ getName WRITE setName NOTIFY notifyChangeName FINAL)
    Q_PROPERTY (QString group READ getGroup WRITE setGroup NOTIFY notifyChangeGroup FINAL)
    Q_PROPERTY (QString priority READ getPriority WRITE setPriority NOTIFY notifyChangePriority FINAL)
    Q_PROPERTY (QString loglevel READ getLogLevel WRITE setLogLevel NOTIFY notifyChangeLogLevel FINAL)
    Q_PROPERTY (QString description READ getDescription WRITE setDescription NOTIFY notifyChangeDescription FINAL)
    Q_PROPERTY (bool execNum READ getExecNum NOTIFY notifyChangeExecNum FINAL)
    Q_PROPERTY (bool isRunning READ getIsRunning NOTIFY notifyChangeIsRunning FINAL)
    Q_PROPERTY (QString isRunningAsString READ getIsRunningAsString NOTIFY notifyChangeIsRunning FINAL)
    Q_PROPERTY (bool backgrounded READ getBackgrounded WRITE setBackgrounded NOTIFY notifyChangeBackgrounded FINAL)
    Q_PROPERTY (QString backgroundedAsString READ getBackgroundedAsString NOTIFY notifyChangeBackgrounded FINAL)
    Q_PROPERTY (bool favourite READ getFav WRITE setFav NOTIFY notifyChangeFav FINAL)
    Q_PROPERTY (bool autoStart READ getAutoStart WRITE setAutoStart NOTIFY notifyChangeAutoStart FINAL)
    Q_PROPERTY (bool runOnce READ getRunOnce WRITE setRunOnce NOTIFY notifyChangeRunOnce FINAL)
    Q_PROPERTY (bb::cascades::ArrayDataModel* executionLog READ getExecutionLog NOTIFY notifyChangeExecutionLog FINAL)
    Q_PROPERTY (bb::cascades::GroupDataModel* variableList READ getVariableList NOTIFY notifyChangeVariableList FINAL)
    public:
        ProcedureCascadesView(QObject* parent = 0);
        ProcedureCascadesView(Procedure* procedure);
        virtual ~ProcedureCascadesView();
        Procedure* getController() const;
        QString getUuid() const;
        QString getFlag() const;
        QString getName() const;
        QString getGroup() const;
        QString getPriority() const;
        QString getLogLevel() const;
        QString getDescription() const;
        void setFlag(QString flag);
        void setName(QString name);
        void setGroup(QString group);
        void setPriority(QString priority);
        void setLogLevel(QString level);
        void setDescription(QString Description);
        int getExecNum() const;
        bool getIsRunning(int executionNumber = 0) const;
        QString getIsRunningAsString() const;
        bool getBackgrounded() const;
        QString getBackgroundedAsString() const;
        void setBackgrounded(bool value);
        bool getFav() const;
        void setFav(bool value);
        bool getAutoStart() const;
        void setAutoStart(bool value);
        bool getRunOnce() const;
        void setRunOnce(bool value);
        bb::cascades::ArrayDataModel* getExecutionLog() const;
        bb::cascades::GroupDataModel* getVariableList() const;

        Q_INVOKABLE
        QObjectList getStatements();

        Q_INVOKABLE
        QObject* newAction(QString type);Q_INVOKABLE
        QObject* pasteAction();

        Q_INVOKABLE
        void refreshStatementPosition();

        Q_INVOKABLE
        QObjectList getExposedVariableDefinitions();

        Q_INVOKABLE
        bool filter(QString filter);

        // Returns execution number or 0 if another start is in progress
        Q_INVOKABLE
        bool start(bool debug = false);

        Q_INVOKABLE
        void stop();

        Q_INVOKABLE
        void remove();

    public slots:
        void propagateChangeExecNum();
        void propagateChangeIsRunning();

    Q_SIGNALS:
        void notifyChangeFlag();
        void notifyChangeName();
        void notifyChangeGroup();
        void notifyChangePriority();
        void notifyChangeLogLevel();
        void notifyChangeDescription();
        void notifyChangeExecNum();
        void notifyChangeIsRunning();
        void notifyChangeBackgrounded();
        void notifyChangeFav();
        void notifyChangeAutoStart();
        void notifyChangeRunOnce();
        void notifyChangeExecutionLog();
        void notifyChangeVariableList();
    private:
        QString flag;
        Procedure* procedureController;
        ProcedureWork* work;
        static Logger* LOG;
        // Appends variables from statement to qobjectlist and updates variablesNames to keep a unique count
        void appendExposedVariableDefinitions(QObjectList* qobjectList,
                std::list<std::string> variableNames, Statement* statement);
    };

} /* namespace actiontrigger */
#endif /* PROCEDURECASCADESVIEW_HPP_ */
