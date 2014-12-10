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

#ifndef PROCEDUREWORK_HPP_
#define PROCEDUREWORK_HPP_

#include <QReadWriteLock>
#include <QThread>
#include "actiontrigger/controller/procedure.hpp"
#include "actiontrigger/model/databasefactory.hpp"

namespace actiontrigger
{

    class ProcedureWork: public QObject
    {
    Q_OBJECT
    public:
        ProcedureWork(Procedure* procedure);
        virtual ~ProcedureWork();
        void start();

    Q_SIGNALS:
        void finished();
        void error(QString err);

    public slots:
        void process();
    private:
        QThread* runner;
        Procedure* procedure;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PROCEDUREWORK_HPP_ */
