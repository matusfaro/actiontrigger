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

#include "actiontrigger/controller/services/statementgroup.hpp"

namespace actiontrigger
{

    Logger* StatementGroup::LOG = LoggerFactory::getInstance("StatementGroup");

    StatementGroup::StatementGroup(std::string name, std::string description) :
            hasTrigger(false), name(name), description(description)
    {
        this->statements = new std::vector<StatementInfo>();
    }

    StatementGroup::~StatementGroup()
    {
        // No-op
    }

    void StatementGroup::addStatement(StatementInfo info)
    {
        statements->push_back(info);
        if (info.isTrigger)
            hasTrigger = true;
    }

    std::string StatementGroup::getName() const
    {
        return this->name;
    }

    std::string StatementGroup::getDescription() const
    {
        return this->description;
    }

    int StatementGroup::getStatementCount() const
    {
        return this->statements->size();
    }

    bool StatementGroup::containsTrigger() const
    {
        return hasTrigger;
    }

    std::vector<StatementInfo>* StatementGroup::getStatementInfos()
    {
        return statements;
    }

} /* namespace actiontrigger */
