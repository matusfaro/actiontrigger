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

#include "actiontrigger/view/cascades/statementinfocascadesview.hpp"

namespace actiontrigger
{

    Logger* StatementInfoCascadesView::LOG = LoggerFactory::getInstance(
            "StatementInfoCascadesView");

    StatementInfoCascadesView::StatementInfoCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    StatementInfoCascadesView::StatementInfoCascadesView(StatementInfo info) :
            info(info)
    {
        LOG->trace("StatementInfoCascadesView");
    }

    StatementInfoCascadesView::~StatementInfoCascadesView()
    {
        LOG->trace("~StatementInfoCascadesView");
    }

    QString StatementInfoCascadesView::getName() const
    {
        LOG->trace("getName");
        return QString::fromStdString(this->info.name);
    }

    QString StatementInfoCascadesView::getType() const
    {
        LOG->trace("getType");
        return QString::fromStdString(this->info.type);
    }

    bool StatementInfoCascadesView::isTrigger() const
    {
        LOG->trace("isTrigger");
        return this->info.isTrigger;
    }

    QString StatementInfoCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        return QString::fromStdString(this->info.description);
    }

} /* namespace actiontrigger */
