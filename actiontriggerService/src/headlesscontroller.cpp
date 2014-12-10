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

#include <src/HeadlessController.hpp>

namespace actiontrigger
{

    Logger* HeadlessController::LOG = LoggerFactory::getInstance("HeadlessController");

    HeadlessController::HeadlessController() :
            QObject()
    {
    }

    HeadlessController::~HeadlessController()
    {
    }

    int HeadlessController::numRunningProcedures()
    {
        return procedures.size();
    }

    void HeadlessController::startProcedure(int id)
    {
        // Stop previous execution
        if (procedures.find(id) != procedures.end()) {
            Procedure* procedure = procedures.find(id)->second;
            if(procedure->isRunning()){
                LOG->debug(SSTR("procedureStart: procedure already started, not doing anything: " << id));
                return;
            }
            LOG->debug(SSTR("procedureStart: procedure present but not running, restarting: " << id));
            stopProcedure(id);
        }

        // Read model from database
        std::vector<ProcedureModel*>* procedureModels =
                DatabaseFactory::getInstance()->loadProcedures(id);
        if (procedureModels->size() != 1) {
            LOG->error(SSTR("Cannot find procedure with id in db: " << id));
            emit procedureStopping(id);
            disconnect();
            return;
        }
        ProcedureModel* procedureModel = procedureModels->at(0);

        // Construct and prepare for execution
        Procedure* procedure = new Procedure(procedureModel);
        if (!procedure->prepareStart(false, true)) {
            LOG->error(SSTR("Cannot prepare procedure start with id: " << id));
            emit procedureStopping(id);
            delete procedure;
            delete procedureModel;
            return;
        }

        // Prepare cleanup
        LOG->debug(SSTR("Starting procedure: " << procedure->getName()));
        SuicideBooth* booth = new SuicideBooth(procedure);
        QObject::connect(booth, SIGNAL(procedureStopped(int)), this, SLOT(procedureEnded(int)));
//        QObject::connect(this, SIGNAL(procedureStopping(int)), booth, SLOT(stopProcedure(int)));

        // Start execution
        procedures.insert(std::pair<int, Procedure*>(id, procedure));
        (new ProcedureWork(procedure))->start();
    }

    void HeadlessController::stopProcedure(int id)
    {
        if (procedures.find(id) == procedures.end()) {
            LOG->debug(SSTR("procedureEnded: procedure not running: " << id));
            return;
        }
        Procedure* procedure = procedures.find(id)->second;

        procedure->stop();
    }

    void HeadlessController::procedureEnded(int id)
    {
        if (procedures.find(id) == procedures.end()) {
            LOG->error("procedureEnded: Cannot find running procedure");
            return;
        }
        procedures.erase(id);
        emit procedureStopping(id);
    }

    std::map<int, Procedure*>* HeadlessController::getProcedures()
    {
        return &this->procedures;
    }

} /* namespace actiontrigger */
