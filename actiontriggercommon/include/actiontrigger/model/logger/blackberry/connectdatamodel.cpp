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

#include "actiontrigger/model/logger/blackberry/connectdatamodel.hpp"

namespace actiontrigger
{

    ConnectDataModel::ConnectDataModel(bb::cascades::ArrayDataModel* dataModel)
    {
        if (dataModel) {
            this->dataModel = dataModel;
        } else {
            this->dataModel = new bb::cascades::ArrayDataModel(0);
        }
    this->dataModel->connect(this, SIGNAL(signalSend(QVariant)), this->dataModel, SLOT(append(const QVariant &)));
}

ConnectDataModel::~ConnectDataModel()
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

void ConnectDataModel::send(std::string msg)
{
    emit signalSend(QVariant(msg.c_str()));
}

bb::cascades::ArrayDataModel* ConnectDataModel::getDataModel()
{
    return this->dataModel;
}

} /* namespace actiontrigger */
