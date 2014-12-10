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

#ifndef PARAMETERCASCADESVIEW_HPP_
#define PARAMETERCASCADESVIEW_HPP_

#include "actiontrigger/view/cascades/cascadescommon.hpp"
#include <bb/cascades/GroupDataModel>
#include "actiontrigger/controller/services/parameter.hpp"
#include "actiontrigger/controller/services/statement.hpp"
#include "actiontrigger/controller/common.hpp"
#include <QBool>
#include "actiontrigger/model/loggerfactory.hpp"

namespace actiontrigger
{

    class ParameterCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (bool showParams READ showParams FINAL)
    Q_PROPERTY (QString type READ getType FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (QString value READ getValue WRITE setValue NOTIFY notifyChangeValue FINAL)
    public:
        ParameterCascadesView(QObject* parent = 0);
        ParameterCascadesView(Statement* statement, ParameterDefinition* parameter);
        virtual ~ParameterCascadesView();
        QString getName() const;
        QString getType() const;
        QString getDescription() const;
        bool showParams() const;
        QString getValue() const;
        void setValue(QString value);

        Q_INVOKABLE
        QString getSubValue(QString subkey);

        Q_INVOKABLE
        void setSubValue(QString subkey, QString value);

        Q_INVOKABLE
        QStringList getDetails();

        Q_INVOKABLE
        qint32 getOptionCount();

        Q_INVOKABLE
        QStringList getOption(qint32);

        Q_INVOKABLE
        QBool validate();

    Q_SIGNALS:
        void notifyChangeValue(QString key, QString value);
    private:
        Statement* statement;
        ParameterDefinition* parameter;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PARAMETERCASCADESVIEW_HPP_ */
