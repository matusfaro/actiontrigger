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

#ifndef PERMISSIONCASCADESVIEW_HPP_
#define PERMISSIONCASCADESVIEW_HPP_

#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/permissions/permission.hpp"

#include <QObject>
#include <qglobal.h>

namespace actiontrigger
{

    class PermissionCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (QString type READ getType FINAL)
    Q_PROPERTY (QString imagePath READ getImagePath FINAL)
    Q_PROPERTY (QBool hasAccessGranted READ hasAccessGranted NOTIFY notifyChangeAccessGranted FINAL)
    Q_PROPERTY (QBool hasAccessGrantedSystem READ hasAccessGrantedSystem NOTIFY notifyChangeAccessGrantedSystem FINAL)
    Q_PROPERTY (QBool hasAccessGrantedApplication READ hasAccessGrantedApplication WRITE setAccessGrantedApplication NOTIFY notifyChangeAccessGrantedApplication FINAL)
    public:
        PermissionCascadesView(QObject* parent = 0);
        PermissionCascadesView(Permission* permissionController);
        virtual ~PermissionCascadesView();
        QString getName() const;
        QString getDescription() const;
        QString getType() const;
        QString getImagePath() const;

        QBool hasAccessGranted() const;
        QBool hasAccessGrantedSystem() const;
        QBool hasAccessGrantedApplication() const;

        void setAccessGrantedApplication(QBool);

    Q_SIGNALS:
        void notifyChangeAccessGranted();
        void notifyChangeAccessGrantedSystem();
        void notifyChangeAccessGrantedApplication();
    private:
        Permission* permissionController;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PERMISSIONCASCADESVIEW_HPP_ */
