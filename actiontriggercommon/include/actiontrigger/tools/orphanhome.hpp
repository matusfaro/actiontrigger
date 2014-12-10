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

#ifndef ORPHANHOME_HPP_
#define ORPHANHOME_HPP_

#include <QObject>
#include <QThread>

namespace actiontrigger
{

    /**
     * This singleton object is used to be a parent for orphaned QObjects
     *
     * Obsoleted before even used. Left for historical reasons.
     */
    class OrphanHome: public QObject
    {
        Q_OBJECT

    public:
        static void build(QObject* parent = 0);
        static void add(QObject * orphan);
    private:
        static OrphanHome* instance;
        static QThread* orphanThread;
        OrphanHome(QObject* parent);
        virtual ~OrphanHome();
    };

} /* namespace actiontrigger */
#endif /* ORPHANHOME_HPP_ */
