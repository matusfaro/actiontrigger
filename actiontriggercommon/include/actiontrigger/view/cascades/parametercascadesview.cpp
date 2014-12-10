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

#include "actiontrigger/view/cascades/parametercascadesview.hpp"

namespace actiontrigger
{

    Logger* ParameterCascadesView::LOG = LoggerFactory::getInstance("ParameterCascadesView");

    ParameterCascadesView::ParameterCascadesView(QObject* parent) :
            QObject(parent)
    {
    }

    ParameterCascadesView::ParameterCascadesView(Statement* statement,
            ParameterDefinition* parameter) :
            statement(statement), parameter(parameter)
    {
        LOG->trace("ParameterCascadesView");
    }

    ParameterCascadesView::~ParameterCascadesView()
    {
        LOG->trace("~ParameterCascadesView");
    }

    QString ParameterCascadesView::getName() const
    {
        LOG->trace("getName");
        return QString::fromStdString(this->parameter->name);
    }

    bool ParameterCascadesView::showParams() const
    {
        LOG->trace("showParams");
        return this->parameter->showParams;
    }

    QString ParameterCascadesView::getType() const
    {
        LOG->trace("getType");
        return QString::fromStdString(this->parameter->type);
    }

    QString ParameterCascadesView::getDescription() const
    {
        LOG->trace("getDescription");
        return QString::fromStdString(this->parameter->description);
    }

    QString ParameterCascadesView::getValue() const
    {
        LOG->trace("getValue");
        return QString::fromStdString(this->statement->getParameter(this->parameter->key));
    }

    QString ParameterCascadesView::getSubValue(QString subkey)
    {
        LOG->trace("getSubValue");
        return QString::fromStdString(
                this->statement->getSubParameter(this->parameter->key, subkey.toStdString()));
    }

    QStringList ParameterCascadesView::getDetails()
    {
        LOG->trace("getTypeDetails");
        return QStringList::fromVector(*CascadesCommon::convert(&this->parameter->details));
    }

    qint32 ParameterCascadesView::getOptionCount()
    {
        LOG->trace("getOptionLength");
        return (qint32) this->parameter->options.size();
    }

    QStringList ParameterCascadesView::getOption(qint32 index)
    {
        LOG->trace("getOption");
        return QStringList::fromVector(
                *CascadesCommon::convert(&this->parameter->options.at((int) index)));
    }

    void ParameterCascadesView::setValue(QString value)
    {
        LOG->trace("setValue");
        this->statement->setParameter(this->parameter->key, value.toStdString());
        emit notifyChangeValue(QString::fromStdString(this->parameter->key), value);
    }

    void ParameterCascadesView::setSubValue(QString subkey, QString value)
    {
        LOG->trace("setSubValue");
        this->statement->setSubParameter(this->parameter->key, subkey.toStdString(),
                value.toStdString());
        emit notifyChangeValue(
                QString::fromStdString(this->parameter->key + "_" + subkey.toStdString()), value);
    }

    QBool ParameterCascadesView::validate()
    {
        LOG->trace("validate");
        return QBool(
                this->statement->checkRegexMatch(
                        this->statement->getParameter(this->parameter->key),
                        this->parameter->validation));
    }

} /* namespace actiontrigger */
