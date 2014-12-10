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

#ifndef STATEMENTFACTORYTEMPLATE_HPP_
#define STATEMENTFACTORYTEMPLATE_HPP_

#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/trigger.hpp"
#include "actiontrigger/controller/services/action.hpp"
#include "actiontrigger/controller/statementfactory.hpp"

namespace actiontrigger
{

    template<class StatementBaseType, class StatementType>
    class StatementFactoryTemplate: public StatementFactory<StatementBaseType>
    {
    public:
        StatementFactoryTemplate();
        virtual ~StatementFactoryTemplate();
        const StatementInfo getInfo();
        virtual StatementBaseType* getStatement(StatementModel* model);
    private:
        static Logger* LOG;
    };

    template<class StatementBaseType, class StatementType>
    Logger* StatementFactoryTemplate<StatementBaseType, StatementType>::LOG =
            LoggerFactory::getInstance("StatementFactory");

    template<class StatementBaseType, class StatementType>
    StatementFactoryTemplate<StatementBaseType, StatementType>::StatementFactoryTemplate()
    {
        LOG->trace("StatementFactory");
    }

    template<class StatementBaseType, class StatementType>
    StatementFactoryTemplate<StatementBaseType, StatementType>::~StatementFactoryTemplate()
    {
        LOG->trace("~StatementFactory");
    }

    template<class StatementBaseType, class StatementType>
    const StatementInfo StatementFactoryTemplate<StatementBaseType, StatementType>::getInfo()
    {
        LOG->trace("getInfo");
        return StatementType::info;
    }

    template<class StatementBaseType, class StatementType>
    StatementBaseType* StatementFactoryTemplate<StatementBaseType, StatementType>::getStatement(
            StatementModel* model)
    {
        LOG->trace("getStatement");
        return (StatementBaseType*) new StatementType(model);
    }

} /* namespace actiontrigger */
#endif /* STATEMENTFACTORYTEMPLATE_HPP_ */
