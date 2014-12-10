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

#include "actiontrigger/tools/orphanhome.hpp"

namespace actiontrigger
{

    OrphanHome* OrphanHome::instance = NULL;
    QThread* OrphanHome::orphanThread = NULL;

    void OrphanHome::build(QObject* parent)
    {
        if (instance) {
            instance->setParent(parent);
            orphanThread = QThread::currentThread();
        } else {
            instance = new OrphanHome(parent);
        }
    }

    void OrphanHome::add(QObject * orphan)
    {
        if (!instance) {
            build();
        }
        orphan->moveToThread(orphanThread);
        orphan->setParent(instance);
    }

    OrphanHome::OrphanHome(QObject* parent) :
            QObject(parent)
    {
    }

    OrphanHome::~OrphanHome()
    {
        // No-op
    }

} /* namespace actiontrigger */
