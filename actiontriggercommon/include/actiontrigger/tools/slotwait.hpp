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

#ifndef SLOTWAIT_HPP_
#define SLOTWAIT_HPP_

#include <QObject>
#include <QEventLoop>
#include <QTimer>

namespace actiontrigger
{

    /**
     * Used to synchronously wait for a signal to be delivered to the quit slot of this object.
     */
    class SlotWait: public QEventLoop
    {
    public:
        SlotWait(QObject *parent = 0);
        /**
         * Waits indefinitely (timeout_ms=0) or timeout_ms milliseconds
         * to receive a signal in the quit slot. returns true if
         * received, returns false if timeout expired.
         */
        bool wait(long timeout_ms = 0, ProcessEventsFlags flags = AllEvents);
        virtual ~SlotWait();

    private:
        QTimer timer;
    };

} /* namespace actiontrigger */
#endif /* SLOTWAIT_HPP_ */
