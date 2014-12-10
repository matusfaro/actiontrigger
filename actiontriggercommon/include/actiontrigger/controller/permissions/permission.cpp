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

#include "actiontrigger/controller/permissions/permission.hpp"

namespace actiontrigger
{

    Permission::Permission(std::string type) :
            type(type)
    {
        this->model = (PermissionModel*) new PermissionDBModel(type);
    }

    Permission::~Permission()
    {
        delete this->model;
    }

    std::string Permission::getType()
    {
        return this->type;
    }

    bool Permission::hasAccessGranted()
    {
        return this->hasAccessGrantedApplication() && this->hasAccessGrantedSystem();
    }

    bool Permission::hasAccessGrantedApplication()
    {
        return this->model->hasAccessGranted();
    }

    void Permission::setAccessGrantedApplication()
    {
        this->model->setAccessGranted();
    }

    void Permission::setAccessDeniedApplication()
    {
        this->model->setAccessDenied();
    }

} /* namespace actiontrigger */
