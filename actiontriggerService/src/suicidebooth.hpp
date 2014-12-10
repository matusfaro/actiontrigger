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

#ifndef SUICIDEBOOTH_HPP_
#define SUICIDEBOOTH_HPP_

#include <QObject>
#include "actiontrigger/controller/procedure.hpp"
#include <QTimer>

namespace actiontrigger
{

    class SuicideBooth: public QObject
    {
    Q_OBJECT

    public:
        SuicideBooth(Procedure* procedure);
        virtual ~SuicideBooth();

    public Q_SLOTS:
        void isRunningChanged();
        void stopProcedure();
        void stopProcedure(int id);

    Q_SIGNALS:
        void procedureStopped(int id);

    private:
        static Logger* LOG;
        Procedure* procedure;
    };

} /* namespace actiontrigger */

#endif /* SUICIDEBOOTH_HPP_ */
