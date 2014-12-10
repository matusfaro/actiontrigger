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

#include "actiontrigger/controller/services/service.hpp"

namespace actiontrigger
{

    Logger* Service::LOG = LoggerFactory::getInstance("Service");

    Service::~Service()
    {
        LOG->trace("~Service");
        // Delete triggers
        for (std::map<std::string, StatementFactory<Trigger>*>::iterator it = triggers->begin();
                it != triggers->end(); ++it) {
            delete it->second;
        }
        // Delete actions
        for (std::map<std::string, StatementFactory<Action>*>::iterator it = actions->begin();
                it != actions->end(); ++it) {
            delete it->second;
        }
        // Delete branches
        for (std::map<std::string, StatementFactory<Branch>*>::iterator it = branches->begin();
                it != branches->end(); ++it) {
            delete it->second;
        }
    }

    void Service::setup()
    {
        LOG->trace("setup");
        this->triggers = getTriggers();
        this->actions = getActions();
        this->branches = getBranches();
    }

    int Service::getTriggerCount() const
    {
        return this->triggers->size();
    }

    int Service::getActionCount() const
    {
        return this->triggers->size() + this->actions->size();
    }

    std::map<std::string, StatementInfo>* Service::getTriggerInfos()
    {
        LOG->trace("getTriggerInfos");
        std::map<std::string, StatementInfo>* triggerInfos =
                new std::map<std::string, StatementInfo>();
        for (std::map<std::string, StatementFactory<Trigger>*>::iterator it = triggers->begin();
                it != triggers->end(); ++it) {
            triggerInfos->insert(
                    std::pair<std::string, StatementInfo>(it->first, it->second->getInfo()));
        }
        return triggerInfos;
    }

    std::map<std::string, StatementInfo>* Service::getActionInfos()
    {
        LOG->trace("getActionInfos");
        std::map<std::string, StatementInfo>* actionInfos =
                new std::map<std::string, StatementInfo>();
        // Insert all actions
        for (std::map<std::string, StatementFactory<Action>*>::iterator it = actions->begin();
                it != actions->end(); ++it) {
            actionInfos->insert(
                    std::pair<std::string, StatementInfo>(it->first, it->second->getInfo()));
        }
        // Also add all branches
        for (std::map<std::string, StatementFactory<Branch>*>::iterator it = branches->begin();
                it != branches->end(); ++it) {
            actionInfos->insert(
                    std::pair<std::string, StatementInfo>(it->first, it->second->getInfo()));
        }
        // Also add all triggers since a trigger can be an action as well
        for (std::map<std::string, StatementFactory<Trigger>*>::iterator it = triggers->begin();
                it != triggers->end(); ++it) {
            actionInfos->insert(
                    std::pair<std::string, StatementInfo>(it->first, it->second->getInfo()));
        }
        return actionInfos;
    }

    Trigger* Service::getTriggerStatement(StatementModel* model)
    {
        LOG->trace("getTriggerStatement");
        if (this->triggers->find(model->getType()) != this->triggers->end()) {
            return this->triggers->find(model->getType())->second->getStatement(model);
        }
        LOG->warning("Unknown statement type: " + model->getType());
        throw new UnknownStatementException("Unknown statement type: " + model->getType());
    }

    Action* Service::getActionStatement(StatementModel* model)
    {
        LOG->trace("getActionStatement");
        if (this->actions->find(model->getType()) != this->actions->end()) {
            return this->actions->find(model->getType())->second->getStatement(model);
        }
        if (this->branches->find(model->getType()) != this->branches->end()) {
            return (Action*) this->branches->find(model->getType())->second->getStatement(model);
        }
        if (this->triggers->find(model->getType()) != this->triggers->end()) {
            return (Action*) this->triggers->find(model->getType())->second->getStatement(model);
        }
        LOG->warning("Unknown statement type: " + model->getType());
        throw new UnknownStatementException("Unknown statement type: " + model->getType());
    }

    std::map<std::string, StatementFactory<Branch>*>* Service::getBranches()
    {
        return new std::map<std::string, StatementFactory<Branch>*>();
    }

} /* namespace actiontrigger */
