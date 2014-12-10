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

#include "actiontrigger/view/cascades/cascades.hpp"

#include "actiontrigger/view/cascades/servicecascadesview.hpp"
#include "actiontrigger/view/cascades/procedurecascadesview.hpp"
#include "actiontrigger/view/cascades/parametercascadesview.hpp"
#include "actiontrigger/view/cascades/statementinfocascadesview.hpp"
#include "actiontrigger/view/cascades/permissioncascadesview.hpp"
#include "actiontrigger/view/cascades/branchinfocascadesview.hpp"
#include "actiontrigger/view/cascades/statementgroupcascadesview.hpp"

namespace actiontrigger
{

    Logger* Cascades::LOG = LoggerFactory::getInstance("Cascades");

    Cascades* Cascades::instance = NULL;

    Cascades* Cascades::getInstance()
    {
        if (!instance)
            instance = new Cascades();
        return instance;
    }

    Cascades::Cascades()
    {
#ifdef CASCADES_USE_CACHE
        serviceMap = new std::map<Service*, QObject*>();
        procedureMap = new std::map<Procedure*, QObject*>();
        statementMap = new std::map<Statement*, StatementCascadesView*>();
        permissionMap = new std::map<Permission*, QObject*>();
        statementInfoMap = new std::map<std::string, QObject*>();
        parameterMap = new std::map<std::pair<Statement*, ParameterDefinition*>, QObject*>();
#endif
    }

    Cascades::~Cascades()
    {
#ifdef CASCADES_USE_CACHE
        for (std::map<Service*, QObject*>::iterator it = serviceMap->begin(); it != serviceMap->end(); ++it) {
            delete it->second;
        }
        for (std::map<Procedure*, QObject*>::iterator it = procedureMap->begin(); it != procedureMap->end(); ++it) {
            delete it->second;
        }
        for (std::map<Statement*, StatementCascadesView*>::iterator it = statementMap->begin(); it != statementMap->end(); ++it) {
            delete it->second;
        }
        for (std::map<std::string, QObject*>::iterator it = statementInfoMap->begin(); it != statementInfoMap->end(); ++it) {
            delete it->second;
        }
        for (std::map<std::pair<Statement*, ParameterDefinition*>, QObject*>::iterator it = parameterMap->begin(); it != parameterMap->end(); ++it) {
            delete it->second;
        }
#endif
    }

    QObject* Cascades::displayService(Service* service)
    {
#ifdef CASCADES_USE_CACHE
        if (serviceMap->find(service) == serviceMap->end()) {
            QObject* object = (QObject*) new ServiceCascadesView(service);
            QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
            serviceMap->insert(std::pair<Service*, QObject*>(service, object));
            object->setParent(this);
        }
        return serviceMap->find(service)->second;
#else
        QObject* object = (QObject*) new ServiceCascadesView(service);
        QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
        return object;
#endif
    }

    QObject* Cascades::displayProcedure(Procedure* procedure)
    {
#ifdef CASCADES_USE_CACHE
        if (procedureMap->find(procedure) == procedureMap->end()) {
            QObject* object = (QObject*) new ProcedureCascadesView(procedure);
            QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
            procedureMap->insert(std::pair<Procedure*, QObject*>(procedure, object));
            object->setParent(this);
        }
        return procedureMap->find(procedure)->second;
#else
        QObject* object = (QObject*) new ProcedureCascadesView(procedure);
        QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
        return object;
#endif
    }

    StatementCascadesView* Cascades::displayStatement(Procedure* parentProcedure,
            Statement* statement, std::string branchName, Statement* parentStatement)
    {
#ifdef CASCADES_USE_CACHE
        if (statementMap->find(statement) == statementMap->end()) {
            StatementCascadesView* view = new StatementCascadesView(parentProcedure, statement, branchName, parentStatement);
            QDeclarativeEngine::setObjectOwnership((QObject*) view, QDeclarativeEngine::CppOwnership);
            statementMap->insert(std::pair<Statement*, StatementCascadesView*>(statement, view));
            view->setParent(this);
        }
        return statementMap->find(statement)->second;
#else
        StatementCascadesView* object = new StatementCascadesView(parentProcedure, statement,
                branchName, parentStatement);
        QDeclarativeEngine::setObjectOwnership((QObject*) object, QDeclarativeEngine::CppOwnership);
        return object;
#endif
    }

    QObject* Cascades::displayParameter(Statement* statement, ParameterDefinition* parameter,
            bool useCache)
    {
#ifdef CASCADES_USE_CACHE
        if (useCache) {
            if (parameterMap->find(std::pair<Statement *, ParameterDefinition*>(statement, parameter)) == parameterMap->end()) {
                QObject* object = (QObject*) new ParameterCascadesView(statement, parameter);
                QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
                parameterMap->insert(
                        std::pair<std::pair<Statement *, ParameterDefinition*>, QObject*>(
                                std::pair<Statement *, ParameterDefinition*>(statement, parameter), object));
                object->setParent(this);
            }
            return parameterMap->find(std::pair<Statement *, ParameterDefinition*>(statement, parameter))->second;
        } else {
            return (QObject*) new ParameterCascadesView(statement, parameter);
        }
#else
        (void) useCache;
        QObject* object = (QObject*) new ParameterCascadesView(statement, parameter);
        QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
        return object;
#endif
    }

    QObject* Cascades::displayPermission(Permission* permission)
    {
#ifdef CASCADES_USE_CACHE
        if (permissionMap->find(permission) == permissionMap->end()) {
            QObject* object = (QObject*) new PermissionCascadesView(permission);
            QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
            permissionMap->insert(std::pair<Permission*, QObject*>(permission, object));
            object->setParent(this);
        }
        return permissionMap->find(permission)->second;
#else
        QObject* object = (QObject*) new PermissionCascadesView(permission);
        QDeclarativeEngine::setObjectOwnership(object, QDeclarativeEngine::CppOwnership);
        return object;
#endif
    }

    std::vector<QObject*>* Cascades::displayServices(std::vector<Service*>* services)
    {
        std::vector<QObject*>* serviceViews = new std::vector<QObject*>();
        for (std::vector<Service*>::iterator it = services->begin(); it != services->end(); ++it) {
            serviceViews->push_back((QObject *) displayService(*it));
        }
        return serviceViews;
    }

    std::vector<QObject*>* Cascades::displayProcedures(std::vector<Procedure*>* procedures)
    {
        std::vector<QObject*>* procedureViews = new std::vector<QObject*>();
        for (std::vector<Procedure*>::iterator it = procedures->begin(); it != procedures->end();
                ++it) {
            procedureViews->push_back(displayProcedure(*it));
        }
        return procedureViews;
    }

    std::vector<QObject*>* Cascades::displayTriggers(Procedure* parentProcedure,
            std::vector<Trigger*>* triggers)
    {
        std::vector<QObject*>* triggerViews = new std::vector<QObject*>();
        for (std::vector<Trigger*>::iterator it = triggers->begin(); it != triggers->end(); ++it) {
            triggerViews->push_back(displayStatement(parentProcedure, (Statement*) *it));
        }
        return triggerViews;
    }

    std::vector<QObject*>* Cascades::displayActions(Procedure* parentProcedure,
            std::vector<Action*>* actions)
    {
        std::vector<QObject*>* actionViews = new std::vector<QObject*>();
        for (std::vector<Action*>::iterator it = actions->begin(); it != actions->end(); ++it) {
            actionViews->push_back(displayStatement(parentProcedure, (Statement*) *it));
        }
        return actionViews;
    }

    QObjectList Cascades::displayBranches(std::vector<branch_info> branches)
    {
        QObjectList* branchViews = new QObjectList();
        for (std::vector<branch_info>::iterator it = branches.begin(); it != branches.end(); ++it) {
            branchViews->append((QObject*) new BranchInfoCascadesView(*it));
        }
        return *branchViews;
    }

    QObjectList Cascades::displayParameters(Statement* statement, bool useCache)
    {
        LOG->trace("displayParameters");
        QObjectList* parameterViews = new QObjectList();
        std::vector<ParameterDefinition*>* parameters = statement->getParameterDefinitions();
        for (std::vector<ParameterDefinition*>::iterator it = parameters->begin();
                it != parameters->end(); ++it) {
            parameterViews->append(displayParameter(statement, *it, useCache));
        }
        return *parameterViews;
    }

    std::vector<QObject*>* Cascades::displayStatementGroups(std::vector<StatementGroup*>* groups)
    {
        std::vector<QObject*>* groupViews = new std::vector<QObject*>();
        // Iterate over Services
        for (std::vector<StatementGroup*>::iterator it = groups->begin(); it != groups->end();
                ++it) {
            groupViews->push_back((QObject*) new StatementGroupCascadesView(*it));
        }
        return groupViews;
    }

    std::vector<QObject*>* Cascades::displayStatementInfos(std::vector<StatementInfo>* infos)
    {
        std::vector<QObject*>* infoViews = new std::vector<QObject*>();
        // Iterate over Services
        for (std::vector<StatementInfo>::iterator it = infos->begin(); it != infos->end(); ++it) {
            infoViews->push_back((QObject*) new StatementInfoCascadesView(*it));
        }
        return infoViews;
    }

    std::vector<QObject*>* Cascades::displayPermissions(std::vector<Permission*>* permissions)
    {
        std::vector<QObject*>* permissionViews = new std::vector<QObject*>();
        for (std::vector<Permission*>::iterator it = permissions->begin(); it != permissions->end();
                ++it) {
            permissionViews->push_back(displayPermission(*it));
        }
        return permissionViews;
    }

} /* namespace actiontrigger */

