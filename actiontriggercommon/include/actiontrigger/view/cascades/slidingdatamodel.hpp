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

#ifndef SlidingDataModel_HPP
#define SlidingDataModel_HPP

#include <QObject>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QVariantList>
#include <QRegExp>
#include <bb/cascades/ArrayDataModel>
#include "actiontrigger/model/loggerfactory.hpp"

namespace actiontrigger
{

    class SlidingDataModel: public bb::cascades::ArrayDataModel
    {
    Q_OBJECT

    public:
        SlidingDataModel(int maxSize = 100, QObject *parent = 0);

        Q_SLOT
        void handleInsert();

    private:
        int maxSize;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* SlidingDataModel_HPP */
