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

#ifndef SERVICECASCADESVIEW_HPP_
#define SERVICECASCADESVIEW_HPP_

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <qglobal.h>
#include "actiontrigger/controller/services/service.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/logger/logger.hpp"
#include <bb/cascades/GroupDataModel>

namespace actiontrigger
{

    class ServiceCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (qint32 triggerCount READ getTriggerCount FINAL)
    Q_PROPERTY (qint32 actionCount READ getActionCount FINAL)
    Q_PROPERTY (QBool expanded READ isExpanded WRITE setExpanded NOTIFY notifyChangeExpanded FINAL)
    public:
        ServiceCascadesView(QObject* parent = 0);
        ServiceCascadesView(Service* service);
        virtual ~ServiceCascadesView();
        QString getName() const;
        QString getDescription() const;
        qint32 getTriggerCount() const;
        qint32 getActionCount() const;
        QBool isExpanded() const;
        void setExpanded(QBool expanded);

        Q_INVOKABLE
        void doExpandCollapse(QString type, bb::cascades::GroupDataModel* dataModel);

    Q_SIGNALS:
        void notifyChangeExpanded();
    private:
        bool expanded;
        Service* serviceController;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* SERVICECASCADESVIEW_HPP_ */
