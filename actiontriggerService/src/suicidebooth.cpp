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

#include <src/suicidebooth.hpp>

namespace actiontrigger
{
    Logger* SuicideBooth::LOG = LoggerFactory::getInstance("SuicideBooth");

    SuicideBooth::SuicideBooth(Procedure* procedure) :
            procedure(procedure)
    {
        QObject::connect(procedure, SIGNAL(notifyChangeIsRunning()), this,
                SLOT(isRunningChanged()));
    }

    SuicideBooth::~SuicideBooth()
    {
    }

    void SuicideBooth::isRunningChanged()
    {
        if (!procedure){
            LOG->debug("isRunningChanged: procedure null");
            return;
    }
        if (procedure->isRunning()) {
            LOG->debug(SSTR("isRunningChanged: still running: " << procedure->getId()));
            return;
        }
        LOG->debug(SSTR("isRunningChanged: " << procedure->getId()));
        emit procedureStopped(procedure->getId());

        QTimer::singleShot(5000, this, SLOT(stopProcedure()));
    }

    void SuicideBooth::stopProcedure(int id)
    {
        if (!procedure) {
            return;
        }
        if (procedure->getId() != id) {
            return;
        }
        LOG->debug(SSTR("stopProcedure(id): " << procedure->getId()));
        stopProcedure();
    }

    void SuicideBooth::stopProcedure()
    {
        if (!procedure) {
            return;
        }
        LOG->debug(SSTR("stopProcedure(): " << procedure->getId()));
        QObject::disconnect(procedure, SIGNAL(notifyChangeIsRunning()), this,
                SLOT(isRunningChanged()));
        ProcedureModel* model = procedure->getModel();
        delete procedure;
        delete model;
        procedure = NULL;
        deleteLater();
    }

} /* namespace actiontrigger */
