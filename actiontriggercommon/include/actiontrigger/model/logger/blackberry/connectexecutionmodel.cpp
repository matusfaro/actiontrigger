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

#include "actiontrigger/model/logger/blackberry/connectexecutionmodel.hpp"

namespace actiontrigger
{

    Logger* ConnectExecutionModel::LOG = LoggerFactory::getInstance("ConnectExecutionModel");

    ConnectExecutionModel::ConnectExecutionModel(bb::cascades::ArrayDataModel* logOutDataModel)
    {
        if (logOutDataModel) {
            this->logOutDataModel = logOutDataModel;
        } else {
            this->logOutDataModel =
                    static_cast<bb::cascades::ArrayDataModel*>(new SlidingDataModel());
        }
        this->logOutDataModel->connect(this, SIGNAL(signalAddLogLine(QVariant)),
                this->logOutDataModel, SLOT(append(const QVariant &)));
    }

    ConnectExecutionModel::~ConnectExecutionModel()
    {
        /**
         * Intentional leak here. If parent is set, a segfault occurs
         * Steps to reproduce:
         *  - Set the parent below to "(QObject*) this" instead of 0
         *  - Run app
         *  - Delete all procedures
         *  - create procedure
         *  - repeat last two steps until segfault :)
         *  Problem is something is attempting to QObject::disconnect from the below ArrayDataModel when it is already freed.
         */
//    dataModel.deleteLater();
    }

    void ConnectExecutionModel::clear()
    {
//    emit this->variables->clear();
        this->logOutDataModel->clear();
//    this->variablesDataModel->clear();
    }

    void ConnectExecutionModel::log(std::string msg)
    {
        LOG->debug("RUNLOG >>> " + msg);
        emit signalAddLogLine(QVariant(msg.c_str()));
    }

    void ConnectExecutionModel::variable(std::string key, std::string value)
    {
        LOG->trace("variable");
        emit signalAddVariable(QString::fromStdString(key), QString::fromStdString(value));
    }

    bb::cascades::ArrayDataModel* ConnectExecutionModel::getLogOutDataModel()
    {
        return this->logOutDataModel;
    }

} /* namespace actiontrigger */
