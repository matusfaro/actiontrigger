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

#include "actiontrigger/controller/services/internal/actionsetpersistentvar.hpp"

namespace actiontrigger
{

    std::string ActionSetPersistentVar::TYPE = "ACTION_ActionSetPersistentVar";

    std::vector<ParameterDefinition*>* ActionSetPersistentVar::parameterDefinitions = NULL;
    Logger* ActionSetPersistentVar::StatementLOG = LoggerFactory::getInstance(
            "ActionSetPersistentVar");
    const StatementInfo ActionSetPersistentVar::info(false, "Set persistent variable",
            ActionSetPersistentVar::TYPE,
            "Sets a local variable to persistent storage. Persistent storage is local to the procedure. It is not exported or imported and will be lost if application data is removed.");

    const StatementInfo ActionSetPersistentVar::getInfo()
    {
        return ActionSetPersistentVar::info;
    }

    ActionSetPersistentVar::ActionSetPersistentVar(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetPersistentVar::~ActionSetPersistentVar()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetPersistentVar::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("NAME", "Name",
                            "Name of the global variable to set. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$", false));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("CONTENT", "Content", "Content of the variable",
                            ""));
        }
        return parameterDefinitions;
    }

    std::string ActionSetPersistentVar::getType() const
    {
        return ActionSetPersistentVar::TYPE;
    }

    void ActionSetPersistentVar::execute(ExecutionState* state)
    {
        PersistentStorage::get()->setUserValue(
                state->getProcedureUuid() + "_" + getParameter("NAME", state),
                getParameter("CONTENT", state));
    }

} /* namespace actiontrigger */
