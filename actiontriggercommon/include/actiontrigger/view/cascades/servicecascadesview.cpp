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

#include "actiontrigger/view/cascades/servicecascadesview.hpp"

namespace actiontrigger
{

    Logger* ServiceCascadesView::LOG = LoggerFactory::getInstance("ServiceCascadesView");

    ServiceCascadesView::ServiceCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    ServiceCascadesView::ServiceCascadesView(Service* service) :
            expanded(false), serviceController(service)
    {
    }

    ServiceCascadesView::~ServiceCascadesView()
    {
    }

    QString ServiceCascadesView::getName() const
    {
        LOG->trace("getName");
        return QString::fromStdString(this->serviceController->getName());
    }

    QString ServiceCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        return QString::fromStdString(this->serviceController->getDescription());
    }

    qint32 ServiceCascadesView::getTriggerCount() const
    {
        LOG->trace("getTriggerCount");
        return (qint32) this->serviceController->getTriggerCount();
    }

    qint32 ServiceCascadesView::getActionCount() const
    {
        LOG->trace("getActionCount");
        return (qint32) this->serviceController->getActionCount();
    }

    QBool ServiceCascadesView::isExpanded() const
    {
        LOG->trace("getActionCount");
        return (QBool) this->expanded;
    }

    void ServiceCascadesView::setExpanded(QBool expanded)
    {
        this->expanded = expanded;
        emit notifyChangeExpanded();
    }

    void ServiceCascadesView::doExpandCollapse(QString type,
            bb::cascades::GroupDataModel* dataModel)
    {
        if (this->isExpanded()) {
            // Collapse
            setExpanded((QBool) false);
            dataModel->clear();
        } else {
            //Expand
            setExpanded((QBool) true);
            std::map<std::string, StatementInfo>* statements;
            if ("triggers" == type) {
                statements = this->serviceController->getTriggerInfos();
            } else {
                statements = this->serviceController->getActionInfos();
            }
            for (std::map<std::string, StatementInfo>::iterator it = statements->begin();
                    it != statements->end(); ++it) {
                QVariantMap* item = new QVariantMap();
                item->insert("type", QVariant::fromValue(QString::fromStdString(it->first)));
                item->insert("name", QVariant::fromValue(QString::fromStdString(it->second.name)));
                item->insert("description",
                        QVariant::fromValue(QString::fromStdString(it->second.description)));
                dataModel->insert(*item);
            }
        }
    }

} /* namespace actiontrigger */
