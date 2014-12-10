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

#include "actiontrigger/view/cascades/permissioncascadesview.hpp"

namespace actiontrigger
{

    Logger* PermissionCascadesView::LOG = LoggerFactory::getInstance("PermissionCascadesView");

    PermissionCascadesView::PermissionCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    PermissionCascadesView::PermissionCascadesView(Permission* permissionController) :
            permissionController(permissionController)
    {
    }

    PermissionCascadesView::~PermissionCascadesView()
    {
    }

    QString PermissionCascadesView::getName() const
    {
        return QString::fromStdString(this->permissionController->getName());
    }

    QString PermissionCascadesView::getDescription() const
    {
        return QString::fromStdString(this->permissionController->getDescription());
    }

    QString PermissionCascadesView::getType() const
    {
        return QString::fromStdString(this->permissionController->getType());
    }

    QString PermissionCascadesView::getImagePath() const
    {
        return QString::fromStdString(this->permissionController->getImagePath());
    }

    QBool PermissionCascadesView::hasAccessGranted() const
    {
        return (QBool) this->permissionController->hasAccessGranted();
    }

    QBool PermissionCascadesView::hasAccessGrantedSystem() const
    {
        return (QBool) this->permissionController->hasAccessGrantedSystem();
    }

    QBool PermissionCascadesView::hasAccessGrantedApplication() const
    {
        return (QBool) this->permissionController->hasAccessGrantedApplication();
    }

    void PermissionCascadesView::setAccessGrantedApplication(QBool hasAccess)
    {
        if (hasAccess)
            this->permissionController->setAccessGrantedApplication();
        else
            this->permissionController->setAccessDeniedApplication();
        emit notifyChangeAccessGranted();
        emit notifyChangeAccessGrantedApplication();
    }

} /* namespace actiontrigger */
