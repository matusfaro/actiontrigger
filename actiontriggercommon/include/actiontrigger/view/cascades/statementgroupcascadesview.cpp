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

#include "actiontrigger/view/cascades/statementgroupcascadesview.hpp"

namespace actiontrigger
{

    Logger* StatementGroupCascadesView::LOG = LoggerFactory::getInstance(
            "StatementGroupCascadesView");

    StatementGroupCascadesView::StatementGroupCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    StatementGroupCascadesView::StatementGroupCascadesView(StatementGroup* group) :
            group(group)
    {
        LOG->trace("StatementGroupCascadesView");
        this->dataModelInfoList = new RegexDataModel(this);
        this->dataModelInfoList->setSortingKeys(QList<QString>() << QString("name"));
        this->dataModelInfoList->setGrouping(bb::cascades::ItemGrouping::ByFullValue);
        std::vector<QObject*>* displayStatementInfos =
                UserInterfaceFactory::getInstance()->displayStatementInfos(
                        this->group->getStatementInfos());
        ToolsCascades::populateDataModel(this->dataModelInfoList, displayStatementInfos);
        delete displayStatementInfos;
    }

    StatementGroupCascadesView::~StatementGroupCascadesView()
    {
        LOG->trace("~StatementGroupCascadesView");
    }

    QString StatementGroupCascadesView::getName() const
    {
        LOG->trace("getName");
        return QString::fromStdString(this->group->getName());
    }

    QString StatementGroupCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        return QString::fromStdString(this->group->getDescription());
    }

    bool StatementGroupCascadesView::containsTrigger() const
    {
        LOG->trace("containsTrigger");
        return this->group->containsTrigger();
    }

    bb::cascades::DataModel* StatementGroupCascadesView::getInfoList()
    {
        LOG->trace("getInfoList");
        return this->dataModelInfoList;
    }

} /* namespace actiontrigger */
