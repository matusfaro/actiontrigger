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

#ifndef ServiceSimple_HPP_
#define ServiceSimple_HPP_

#include "actiontrigger/controller/statementfactory.hpp"
#include "actiontrigger/controller/statementfactorytemplate.hpp"
#include "actiontrigger/controller/services/service.hpp"

namespace actiontrigger
{

    /**
     * OBSOLETE
     */
    class ServiceSimple: public Service
    {
    public:
        ServiceSimple(std::string type, int id, std::string name, std::string description);
        virtual ~ServiceSimple();
        static std::string TYPE;
        int getId() const;
        std::string getName() const;
        std::string getType() const;
        std::string getDescription() const;
        std::map<std::string, StatementFactory<Trigger>*>* getTriggers();
        std::map<std::string, StatementFactory<Action>*>* getActions();
        std::map<std::string, StatementFactory<Branch>*>* getBranches();
        template<class TriggerType>
        void associateTrigger();
        template<class ActionType>
        void associateAction();
        template<class BranchType>
        void associateBranch();
    private:
        std::string type;
        int id;
        std::string name;
        std::string description;
        std::map<std::string, StatementFactory<Trigger>*>* triggers;
        std::map<std::string, StatementFactory<Action>*>* actions;
        std::map<std::string, StatementFactory<Branch>*>* branches;
    };

    template<class TriggerType>
    void ServiceSimple::associateTrigger()
    {
        this->triggers->insert(
                std::pair<std::string, StatementFactory<Trigger>*>(TriggerType::TYPE,
                        (StatementFactory<Trigger>*) new StatementFactoryTemplate<Trigger,
                                TriggerType>()));
    }
    template<class ActionType>
    void ServiceSimple::associateAction()
    {
        this->actions->insert(
                std::pair<std::string, StatementFactory<Action>*>(ActionType::TYPE,
                        (StatementFactory<Action>*) new StatementFactoryTemplate<Action, ActionType>()));
    }
    template<class BranchType>
    void ServiceSimple::associateBranch()
    {
        this->branches->insert(
                std::pair<std::string, StatementFactory<Branch>*>(BranchType::TYPE,
                        (StatementFactory<Branch>*) new StatementFactoryTemplate<Branch, BranchType>()));
    }

} /* namespace actiontrigger */
#endif /* ServiceSimple_HPP_ */
