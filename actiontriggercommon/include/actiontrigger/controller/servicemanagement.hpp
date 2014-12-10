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

#ifndef SERVICEMANAGEMENT_HPP_
#define SERVICEMANAGEMENT_HPP_

#include <map>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/services/statementgroup.hpp"
#include "actiontrigger/controller/statementfactory.hpp"
#include "actiontrigger/controller/statementfactorytemplate.hpp"

namespace actiontrigger
{

    class ServiceManagement
    {
    public:
        static ServiceManagement* getInstance();
        std::vector<StatementGroup*>* getGroups();
        std::vector<StatementInfo>* getAllInfos();
        Trigger* loadTrigger(StatementModel* model);
        Action* loadAction(StatementModel* model);
    private:
        std::map<StatementModel*, Trigger*>* triggerMap;
        std::map<StatementModel*, Action*>* actionMap;
        ServiceManagement();
        virtual ~ServiceManagement();
        static ServiceManagement* instance;
        static Logger* LOG;

        std::vector<StatementGroup*>* groups;
        std::vector<StatementInfo>* all;
        std::map<std::string, StatementFactory<Trigger>*>* triggers;
        std::map<std::string, StatementFactory<Action>*>* actions;
        std::map<std::string, StatementFactory<Branch>*>* branches;

        StatementGroup* newGroup(std::string name, std::string description);

        template<class ActionType>
        void addActionDeprecated();
        template<class TriggerType>
        void addTriggerDeprecated();
        template<class BranchType>
        void addBranchDeprecated();
        template<class ActionType>
        void addActionToGroup(StatementGroup* group);
        template<class TriggerType>
        void addTriggerToGroup(StatementGroup* group);
        template<class BranchType>
        void addBranchToGroup(StatementGroup* group);
    };

    template<class ActionType>
    void ServiceManagement::addActionDeprecated()
    {
        StatementFactory<Action>* factory =
                (StatementFactory<Action>*) new StatementFactoryTemplate<Action, ActionType>();
        this->actions->insert(
                std::pair<std::string, StatementFactory<Action>*>(ActionType::TYPE, factory));
    }
    template<class TriggerType>
    void ServiceManagement::addTriggerDeprecated()
    {
        StatementFactory<Trigger>* factory =
                (StatementFactory<Trigger>*) new StatementFactoryTemplate<Trigger, TriggerType>();
        this->triggers->insert(
                std::pair<std::string, StatementFactory<Trigger>*>(TriggerType::TYPE, factory));
    }
    template<class BranchType>
    void ServiceManagement::addBranchDeprecated()
    {
        StatementFactory<Branch>* factory =
                (StatementFactory<Branch>*) new StatementFactoryTemplate<Branch, BranchType>();
        this->branches->insert(
                std::pair<std::string, StatementFactory<Branch>*>(BranchType::TYPE, factory));
    }

    template<class ActionType>
    void ServiceManagement::addActionToGroup(StatementGroup* group)
    {
        StatementFactory<Action>* factory =
                (StatementFactory<Action>*) new StatementFactoryTemplate<Action, ActionType>();
        StatementInfo info = factory->getInfo();
        group->addStatement(info);
        this->all->push_back(info);
        this->actions->insert(
                std::pair<std::string, StatementFactory<Action>*>(ActionType::TYPE, factory));
    }
    template<class TriggerType>
    void ServiceManagement::addTriggerToGroup(StatementGroup* group)
    {
        StatementFactory<Trigger>* factory =
                (StatementFactory<Trigger>*) new StatementFactoryTemplate<Trigger, TriggerType>();
        StatementInfo info = factory->getInfo();
        group->addStatement(info);
        this->all->push_back(info);
        this->triggers->insert(
                std::pair<std::string, StatementFactory<Trigger>*>(TriggerType::TYPE, factory));
    }
    template<class BranchType>
    void ServiceManagement::addBranchToGroup(StatementGroup* group)
    {
        StatementFactory<Branch>* factory =
                (StatementFactory<Branch>*) new StatementFactoryTemplate<Branch, BranchType>();
        StatementInfo info = factory->getInfo();
        group->addStatement(info);
        this->all->push_back(info);
        this->branches->insert(
                std::pair<std::string, StatementFactory<Branch>*>(BranchType::TYPE, factory));
    }

} /* namespace actiontrigger */
#endif /* SERVICEMANAGEMENT_HPP_ */
