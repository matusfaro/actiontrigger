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

#include "actiontrigger/controller/services/internal/actionprocedurestartstop.hpp"

namespace actiontrigger
{

    std::string ActionProcedureStartStop::TYPE = "ACTION_ActionProcedureStartStop";

    Logger* ActionProcedureStartStop::StatementLOG = LoggerFactory::getInstance(
            "ActionProcedureStartStop");
    const StatementInfo ActionProcedureStartStop::info(false, "Start/Stop Procedure",
            ActionProcedureStartStop::TYPE,
            "Start/stop this or another procedure. A call to start will start procedure only if it is not already running. A call to restart will send a stop signal to procedure if it is running and then start the procedure. A stop signal is delivered immediately, but the procedure may not stop immediately and will do as soon as possible.");

    const StatementInfo ActionProcedureStartStop::getInfo()
    {
        return ActionProcedureStartStop::info;
    }

    ActionProcedureStartStop::ActionProcedureStartStop(StatementModel* model) :
            Action(model)
    {
    }

    ActionProcedureStartStop::~ActionProcedureStartStop()
    {
    }

    std::vector<ParameterDefinition*>* ActionProcedureStartStop::getParameterDefinitions()
    {
        std::vector<ParameterDefinition*>* parameterDefinitions = new std::vector<
                ParameterDefinition*>();
        // State
        ParameterDefinition* state = ParameterDefinition::getDropdown("ACTION", "Action",
                "Action to execute against procedure.", "Action");
        state->dropdownAddOption("Start", "start");
        state->dropdownAddOption("Restart", "restart");
        state->dropdownAddOption("Stop", "stop");
        parameterDefinitions->push_back(state);
        // Target
        ParameterDefinition* target = ParameterDefinition::getDropdown("TARGET", "Target",
                "Procedure to target", "Target");
        std::vector<Procedure*>* procedures = GlobalResources::get()->getProcedures();
        for (std::vector<Procedure*>::iterator iter = procedures->begin();
                iter != procedures->end(); ++iter) {
            target->dropdownAddOption((*iter)->getName(), (*iter)->getUuid());
        }
        parameterDefinitions->push_back(target);
        return parameterDefinitions;
    }

    std::string ActionProcedureStartStop::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "start";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionProcedureStartStop::getType() const
    {
        return ActionProcedureStartStop::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionProcedureStartStop::getExposedVariableDefinitions()
    {
        return new std::vector<statement_output_variable_definition*>();
    }

    void ActionProcedureStartStop::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        // Retrieve desired state
        std::string paramState = getParameter("ACTION", state);
        int action;
        if (paramState.compare("start") == 0) {
            action = START;
        } else if (paramState.compare("restart") == 0) {
            action = RESTART;
        } else if (paramState.compare("stop") == 0) {
            action = STOP;
        } else {
            RUNLOG->error("Pick an action to execute");
            return;
        }
        // Retrieve target UUID
        std::string targetUuid = getParameter("TARGET", state);
        if (targetUuid == "") {
            RUNLOG->error("Pick a target");
            return;
        }
        // Find the target procedure
        std::vector<Procedure*>* procedures = GlobalResources::get()->getProcedures();
        for (std::vector<Procedure*>::iterator iter = procedures->begin();
                iter != procedures->end(); ++iter) {
            if ((*iter)->getUuid() == targetUuid) {

                if (action == START) {
                    if (!(*iter)->isRunning()) {
                        if ((*iter)->prepareStart()) {
                            ProcedureWork* work = new ProcedureWork((*iter));
                            work->start();
                            RUNLOG->debug("Starting: " + (*iter)->getName());
                        } else {
                            RUNLOG->warning("Procedure already starting: " + (*iter)->getName());
                        }
                    } else {
                        RUNLOG->warning("Procedure already running: " + (*iter)->getName());
                    }

                } else if (action == RESTART) {
                    if ((*iter)->prepareStart()) {
                        ProcedureWork* work = new ProcedureWork((*iter));
                        work->start();
                        RUNLOG->debug("Starting: " + (*iter)->getName());
                    } else {
                        RUNLOG->warning("Procedure already starting: " + (*iter)->getName());
                    }

                } else if (action == STOP) {
                    (*iter)->stop();
                    RUNLOG->debug("Stopping: " + (*iter)->getName());
                }

                return; // Successful return
            }
        }
        RUNLOG->warning("Target does not exist");
    }

} /* namespace actiontrigger */
