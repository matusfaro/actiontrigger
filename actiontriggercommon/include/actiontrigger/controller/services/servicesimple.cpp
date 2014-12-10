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

#include "actiontrigger/controller/services/servicesimple.hpp"

namespace actiontrigger
{

    ServiceSimple::ServiceSimple(std::string type, int id, std::string name,
            std::string description) :
            type(type), id(id), name(name), description(description)
    {
        this->triggers = new std::map<std::string, StatementFactory<Trigger>*>();
        this->actions = new std::map<std::string, StatementFactory<Action>*>();
        this->branches = new std::map<std::string, StatementFactory<Branch>*>();
    }

    ServiceSimple::~ServiceSimple()
    {
        delete this->triggers;
        delete this->actions;
        delete this->branches;
    }

    std::map<std::string, StatementFactory<Trigger>*>* ServiceSimple::getTriggers()
    {
        return this->triggers;
    }

    std::map<std::string, StatementFactory<Action>*>* ServiceSimple::getActions()
    {
        return this->actions;
    }

    std::map<std::string, StatementFactory<Branch>*>* ServiceSimple::getBranches()
    {
        return this->branches;
    }

    int ServiceSimple::getId() const
    {
        return 11;
    }

    std::string ServiceSimple::getName() const
    {
        return this->name;
    }

    std::string ServiceSimple::getType() const
    {
        return this->type;
    }

    std::string ServiceSimple::getDescription() const
    {
        return this->description;
    }

} /* namespace actiontrigger */
