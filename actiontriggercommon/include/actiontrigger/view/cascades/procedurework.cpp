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

#include "actiontrigger/view/cascades/procedurework.hpp"

namespace actiontrigger
{

    Logger* ProcedureWork::LOG = LoggerFactory::getInstance("ProcedureWork");

    ProcedureWork::ProcedureWork(Procedure* procedure) :
            runner(NULL), procedure(procedure)
    {
        LOG->trace("ProcedureWork");
    }

    ProcedureWork::~ProcedureWork()
    {
        LOG->trace("~ProcedureWork");
    }

    void ProcedureWork::start()
    {
        LOG->trace("start");
        // Attempt to start thread
        if (runner == NULL) {
            // Create a thread
            LOG->trace("Instantiate QThread");
            runner = new QThread;

            // Give QThread ownership of Worker Object
            LOG->trace("Move cascades view object to thread");
            this->moveToThread(runner);

            // Connects the thread's started() signal to the process() slot in the worker,
            // causing it to start
            LOG->trace("Connect started signal to process");
            connect(runner, SIGNAL(started()), this, SLOT(process()));

            QThread::Priority priority;
            if (procedure->getPriority() == "LOWEST") {
                priority = QThread::LowestPriority;
            } else if (procedure->getPriority() == "HIGHEST") {
                priority = QThread::TimeCriticalPriority;
            } else {
                priority = QThread::NormalPriority;
            }
            LOG->trace(SSTR("Call runner start method priority " << priority));
            runner->start(priority);
            LOG->trace("Done");
        } else {
            // TODO handle error
            LOG->error(
                    "Attempted to start thread but the previous thread was not properly cleaned up.");
        }
    }

    void ProcedureWork::process()
    {
        LOG->trace("process");
        LOG->trace("Executing procedure");
        procedure->start();
        // Commit suicide
        delete this;
    }

} /* namespace actiontrigger */
