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

#ifndef HEADLESSCONTROLLER_HPP_
#define HEADLESSCONTROLLER_HPP_

#include <QObject>
#include <map>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/view/cascades/procedurework.hpp"
#include "suicidebooth.hpp"

namespace actiontrigger
{

    class HeadlessController: public QObject
    {
    Q_OBJECT

    public:
        HeadlessController();
        virtual ~HeadlessController();

        void startProcedure(int id);
        void stopProcedure(int id);

        int numRunningProcedures();
        std::map<int, Procedure*>* getProcedures();

    public Q_SLOTS:
        // Feedback from procedure to notify it ended
        void procedureEnded(int id);

    Q_SIGNALS:
        // Propagate feedback to notify a procedure stopped
        void procedureStopping(int);

    private:
        static Logger* LOG;
        std::map<int, Procedure*> procedures;
    };

} /* namespace actiontrigger */
#endif /* HEADLESSCONTROLLER_HPP_ */
