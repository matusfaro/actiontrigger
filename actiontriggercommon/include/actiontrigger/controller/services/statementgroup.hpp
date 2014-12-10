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

#ifndef StatementGroup_HPP_
#define StatementGroup_HPP_

#include <string>
#include <map>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/statementfactory.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/services/branch.hpp"

namespace actiontrigger
{

    class StatementGroup
    {
    public:
        StatementGroup(std::string name, std::string description);
        virtual ~StatementGroup();

        void addStatement(StatementInfo info);

        virtual std::string getName() const;
        virtual std::string getDescription() const;
        int getStatementCount() const;
        bool containsTrigger() const;
        std::vector<StatementInfo>* getStatementInfos();

    private:
        bool hasTrigger;
        std::string name;
        std::string description;
        std::vector<StatementInfo>* statements;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* StatementGroup_HPP_ */
