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

#ifndef BRANCHINFOCASCADESVIEW_HPP_
#define BRANCHINFOCASCADESVIEW_HPP_

#include <QObject>
#include <QString>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/service.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/logger/logger.hpp"
#include "actiontrigger/controller/common.hpp"

namespace actiontrigger
{

    class BranchInfoCascadesView: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString key READ getKey FINAL)
    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (QString type READ getType FINAL)
    public:
        BranchInfoCascadesView(QObject* parent = 0);
        BranchInfoCascadesView(branch_info info);
        virtual ~BranchInfoCascadesView();

        QString getKey() const;
        QString getName() const;
        QString getDescription() const;
        QString getType() const;
    private:
        branch_info info;
        static Logger* LOG;
    public:
    };

} /* namespace actiontrigger */
#endif /* BRANCHINFOCASCADESVIEW_HPP_ */
