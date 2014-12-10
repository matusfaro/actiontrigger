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

#ifndef KeyValueCascades_HPP_
#define KeyValueCascades_HPP_

#include <QObject>
#include <QString>
#include <string>
#include "actiontrigger/model/loggerfactory.hpp"

namespace actiontrigger
{

    class KeyValueCascades: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString key READ getKey NOTIFY changedKey FINAL)
    Q_PROPERTY (QString value READ getValue NOTIFY changedValue FINAL)
    public:
        KeyValueCascades(QString key, QString value, QObject* parent = 0);
        virtual ~KeyValueCascades();
        QString getKey() const;
        QString getValue() const;
        void setValue(QString value);

    Q_SIGNALS:
        void changedKey();
        void changedValue();
    private:
        static Logger* LOG;
        QString key;
        QString value;
    };

} /* namespace actiontrigger */
#endif /* KeyValueCascades_HPP_ */
