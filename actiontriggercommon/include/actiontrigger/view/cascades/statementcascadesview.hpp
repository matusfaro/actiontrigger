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

#ifndef STATEMENTCASCADESVIEW_HPP_
#define STATEMENTCASCADESVIEW_HPP_

#include <QObject>
#include <QVector>
#include <qglobal.h>
#include <vector>
#include <map>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <QString>

#include "actiontrigger/controller/services/statement.hpp"
#include "actiontrigger/model/loggerfactory.hpp"

#include "actiontrigger/view/userinterfacefactory.hpp"
#include "actiontrigger/view/userinterface.hpp"

#include "actiontrigger/view/cascades/statementrun.hpp"
#include "actiontrigger/tools/work.hpp"

#include "actiontrigger/view/cascades/keyvaluecascades.hpp"

#include "actiontrigger/controller/servicemanagement.hpp"

namespace actiontrigger
{

    class StatementCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (qint32 id READ getId FINAL)
    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (QString type READ getType FINAL)
    Q_PROPERTY (qint32 position READ getPosition NOTIFY notifyChangePosition FINAL)
    Q_PROPERTY (bool isFirst READ getIsFirst NOTIFY notifyChangeIsFirst FINAL)
    Q_PROPERTY (bool isLast READ getIsLast NOTIFY notifyChangeIsLast FINAL)
    Q_PROPERTY (QObjectList branches READ getBranches NOTIFY notifyBranchesChanged FINAL)
    public:
        StatementCascadesView(QObject* parent = 0);
        StatementCascadesView(Procedure* parentProcedure, Statement* statement,
                std::string branchName = NULL, Statement* parentStatement = NULL);
        virtual ~StatementCascadesView();
        qint32 getId() const;
        QString getName() const;
        QString getDescription() const;
        QString getType() const;
        qint32 getPosition();
        bool getIsFirst() const;
        bool getIsLast() const;
        QObjectList getBranches();

        bool getIsRunning() const;

        Q_INVOKABLE
        void refreshPosition();

        Q_INVOKABLE
        void remove();

        Q_INVOKABLE
        QObjectList getParameters();

        Q_INVOKABLE
        bool validate();

        Q_INVOKABLE
        bool moveUp();

        Q_INVOKABLE
        bool moveDown();

        Q_INVOKABLE
        QObjectList getBranch(QString key, QString type);

        Q_INVOKABLE
        QObject* getNext();

        // Creates Action or Trigger at the end of the branch
        Q_INVOKABLE
        QObject* newBranchChild(QString branchName, QString type);Q_INVOKABLE
        QObject* pasteBranchChild(QString branchName);

        Q_INVOKABLE
        void copy();

        Q_INVOKABLE
        bool start(bb::cascades::ArrayDataModel* executionLog,
                bb::cascades::GroupDataModel* variableList = NULL);

        Q_INVOKABLE
        void stop();

    Q_SIGNALS:
        void notifyChangePosition();
        void notifyChangeIsFirst();
        void notifyChangeIsLast();
        void notifyBranchesChanged();
        void signalAddVariable(QObject* msg);

    public slots:
        void propagateChangePosition();
        void displayExecutionVariable(QString key, QString value);
    protected:
        Procedure* parentProcedure;
        Statement* statementController;
        std::string branchName;
        Statement* parentStatement;
        static Logger* LOG;
    private:
        bb::cascades::GroupDataModel* variableList;
        std::map<std::string, KeyValueCascades*>* variables;
    };

} /* namespace actiontrigger */
#endif /* STATEMENTCASCADESVIEW_HPP_ */
