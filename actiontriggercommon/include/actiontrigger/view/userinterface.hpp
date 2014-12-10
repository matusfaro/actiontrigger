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

#ifndef USERINTERFACE_HPP_
#define USERINTERFACE_HPP_

#include <vector>
#include <QObject>
#include <string>

#include "actiontrigger/controller/services/service.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/parameter.hpp"
#include "actiontrigger/controller/services/statementgroup.hpp"
#include "actiontrigger/controller/permissions/permission.hpp"

namespace actiontrigger
{

    class ServiceCascadesView;
    class ProcedureCascadesView;
    class ParameterCascadesView;
    class PermissionCascadesView;
    class StatementCascadesView;
    class StatementGroupCascadesView;

    class UserInterface
    {
    public:
        virtual QObject* displayService(Service* service) = 0;
        virtual QObject* displayProcedure(Procedure* procedure) = 0;
        virtual StatementCascadesView* displayStatement(Procedure* parentProcedure,
                Statement* statement, std::string branchName = "",
                Statement* parentStatement = NULL) = 0;
        virtual QObject* displayParameter(Statement* statement, ParameterDefinition* parameter,
                bool useCache) = 0;
        virtual QObject* displayPermission(Permission* permission) = 0;
        virtual std::vector<QObject*>* displayServices(std::vector<Service*>* services) = 0;
        virtual std::vector<QObject*>* displayProcedures(std::vector<Procedure*>* procedures) = 0;
        virtual std::vector<QObject*>* displayTriggers(Procedure* parentProcedure,
                std::vector<Trigger*>* triggers) = 0;
        virtual std::vector<QObject*>* displayActions(Procedure* parentProcedure,
                std::vector<Action*>* actions) = 0;
        virtual QObjectList displayBranches(std::vector<branch_info> branches) = 0;
        virtual QObjectList displayParameters(Statement* statement, bool useCache) = 0;
        virtual std::vector<QObject*>* displayStatementGroups(
                std::vector<StatementGroup*>* groups) = 0;
        virtual std::vector<QObject*>* displayStatementInfos(std::vector<StatementInfo>* infos) = 0;
        virtual std::vector<QObject*>* displayPermissions(
                std::vector<Permission*>* permissions) = 0;
        virtual ~UserInterface()
        {
        }
        ;
    };

} /* namespace actiontrigger */
#endif /* USERINTERFACE_HPP_ */
