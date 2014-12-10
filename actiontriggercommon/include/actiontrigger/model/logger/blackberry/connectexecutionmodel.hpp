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

#ifndef ConnectExecutionModel_HPP_
#define ConnectExecutionModel_HPP_

#include <QObject>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include "actiontrigger/view/cascades/slidingdatamodel.hpp"
#include <string>
#include <map>
#include "actiontrigger/view/cascades/keyvaluecascades.hpp"
#include <QMutex>
#include "actiontrigger/tools/orphanhome.hpp"

namespace actiontrigger
{

    class ConnectExecutionModel: public QObject
    {
    Q_OBJECT

    public:
        ConnectExecutionModel(bb::cascades::ArrayDataModel* logOutDataModel = NULL);
        virtual ~ConnectExecutionModel();
        void log(std::string msg);
        void variable(std::string key, std::string value);
        bb::cascades::ArrayDataModel* getLogOutDataModel();
        void clear();

    Q_SIGNALS:
        void signalAddLogLine(QVariant msg);
        void signalAddVariable(QString key, QString value);
    private:
        static Logger* LOG;
        bb::cascades::ArrayDataModel* logOutDataModel;
        QMutex mutexVariables;
//    std::map<std::string, KeyValueCascades*>* variables;
    };

} /* namespace actiontrigger */
#endif /* ConnectExecutionModel_HPP_ */
