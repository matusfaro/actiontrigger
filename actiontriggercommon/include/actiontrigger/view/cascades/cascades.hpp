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

#ifndef CASCADES_HPP_
#define CASCADES_HPP_

#include <vector>
#include <string>
#include <map>
#include <utility>
#include "actiontrigger/view/userinterface.hpp"
#include <QObject>
#include "actiontrigger/model/loggerfactory.hpp"

#include "actiontrigger/controller/services/service.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/parameter.hpp"
#include "actiontrigger/controller/common.hpp"

/**
 * Let's practice with pre-processor definitions
 */
//#define CASCADES_USE_CACHE
namespace actiontrigger
{

    class Cascades: public QObject, public UserInterface
    {
    Q_OBJECT

    public:
        static Cascades* getInstance();
        QObject* displayService(Service * service);
        QObject* displayProcedure(Procedure * procedure);
        StatementCascadesView* displayStatement(Procedure* parentProcedure, Statement* statement,
                std::string branchName = "", Statement* parentStatement = NULL);
        QObject* displayParameter(Statement* statement, ParameterDefinition* parameter,
                bool useCache = true);
        QObject* displayPermission(Permission* permission);
        std::vector<QObject*>* displayServices(std::vector<Service *>* services);
        std::vector<QObject*>* displayProcedures(std::vector<Procedure *>* procedures);
        std::vector<QObject*>* displayTriggers(Procedure* parentProcedure,
                std::vector<Trigger *>* triggers);
        std::vector<QObject*>* displayActions(Procedure* parentProcedure,
                std::vector<Action *>* actions);
        QObjectList displayBranches(std::vector<branch_info> branches);
        QObjectList displayParameters(Statement* statement, bool useCache = true);
        std::vector<QObject*>* displayStatementGroups(std::vector<StatementGroup*>* groups);
        std::vector<QObject*>* displayStatementInfos(std::vector<StatementInfo>* infos);
        std::vector<QObject*>* displayPermissions(std::vector<Permission*>* permissions);
    private:
        static Logger* LOG;
        static Cascades *instance;
        Cascades();
        virtual ~Cascades();
#ifdef CASCADES_USE_CACHE
        std::map<Service*, QObject*>* serviceMap;
        std::map<Procedure*, QObject*>* procedureMap;
        std::map<Statement*, StatementCascadesView*>* statementMap;
        std::map<Permission*, QObject*>* permissionMap;
        std::map<std::string, QObject*>* statementInfoMap;
        std::map<std::pair<Statement*, ParameterDefinition*>, QObject*>* parameterMap;
#endif
    };

} /* namespace actiontrigger */
#endif /* CASCADES_HPP_ */
