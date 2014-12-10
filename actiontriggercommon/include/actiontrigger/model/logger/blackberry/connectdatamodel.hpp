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

#ifndef CONNECTDATAMODEL_HPP_
#define CONNECTDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/ArrayDataModel>
#include <QVariant>
#include <string>

namespace actiontrigger
{

    class ConnectDataModel: public QObject
    {
        Q_OBJECT

    public:
        ConnectDataModel(bb::cascades::ArrayDataModel* dataModel = NULL);
        virtual ~ConnectDataModel();
        void send(std::string msg);
        bb::cascades::ArrayDataModel* getDataModel();

        Q_SIGNALS:
        void signalSend(QVariant msg);
    private:
        bb::cascades::ArrayDataModel* dataModel;
    };

} /* namespace actiontrigger */
#endif /* CONNECTDATAMODEL_HPP_ */
