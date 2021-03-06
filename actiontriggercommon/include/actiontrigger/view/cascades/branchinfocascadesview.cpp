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

#include "actiontrigger/view/cascades/branchinfocascadesview.hpp"

namespace actiontrigger
{

    BranchInfoCascadesView::BranchInfoCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    BranchInfoCascadesView::BranchInfoCascadesView(branch_info info) :
            info(info)
    {
    }

    BranchInfoCascadesView::~BranchInfoCascadesView()
    {
    }

    QString BranchInfoCascadesView::getKey() const
    {
        return QString::fromStdString(this->info.key);
    }

    QString BranchInfoCascadesView::getName() const
    {
        return QString::fromStdString(this->info.name);
    }

    QString BranchInfoCascadesView::getDescription() const
    {
        return QString::fromStdString(this->info.description);
    }

    QString BranchInfoCascadesView::getType() const
    {
        return QString::fromStdString(this->info.type);
    }

} /* namespace actiontrigger */
