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

#ifndef StatementGroupCascadesView_HPP_
#define StatementGroupCascadesView_HPP_

#include <QObject>
#include <QString>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/service.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/logger/logger.hpp"
#include "actiontrigger/controller/services/statementgroup.hpp"
#include "actiontrigger/view/cascades/regexdatamodel.hpp"
#include "actiontrigger/view/cascades/toolscascades.hpp"
#include "actiontrigger/view/userinterfacefactory.hpp"

namespace actiontrigger
{

    class StatementGroupCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (bool containsTrigger READ containsTrigger FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    public:
        StatementGroupCascadesView(QObject* parent = 0);
        StatementGroupCascadesView(StatementGroup* group);
        virtual ~StatementGroupCascadesView();
        QString getName() const;
        QString getDescription() const;
        bool containsTrigger() const;

        Q_INVOKABLE
        bb::cascades::DataModel* getInfoList();

    private:
        RegexDataModel* dataModelInfoList;
        StatementGroup* group;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* StatementGroupCascadesView_HPP_ */
