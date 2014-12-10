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

#include "actiontrigger/controller/services/internal/actiongetpersistentvar.hpp"

namespace actiontrigger
{

    std::string ActionGetPersistentVar::TYPE = "ACTION_ActionGetPersistentVar";

    std::vector<ParameterDefinition*>* ActionGetPersistentVar::parameterDefinitions = NULL;
    Logger* ActionGetPersistentVar::StatementLOG = LoggerFactory::getInstance(
            "ActionGetPersistentVar");
    const StatementInfo ActionGetPersistentVar::info(false, "Get persistent variable",
            ActionGetPersistentVar::TYPE,
            "Gets a persistent variable from storage and stores value in a local variable. Persistent storage is local to the procedure. It is not exported or imported and will be lost if application data is removed.");

    const StatementInfo ActionGetPersistentVar::getInfo()
    {
        return ActionGetPersistentVar::info;
    }

    ActionGetPersistentVar::ActionGetPersistentVar(StatementModel* model) :
            Action(model)
    {
    }

    ActionGetPersistentVar::~ActionGetPersistentVar()
    {
    }

    std::vector<ParameterDefinition*>* ActionGetPersistentVar::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("PERSISTENT_NAME", "Persistent name",
                            "Name of the persistent variable to get. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$", false));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("LOCAL_NAME", "Local name",
                            "Name of the local variable to set. Variable name must contain only alphabetic characters and underscores.",
                            "^[A-Za-z_]*$"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("DEFAULT_VALUE", "Default value",
                            "If variable was not previously set, use this default value instead. A default value will not be used if the variable was previously set with an empty string."));
        }
        return parameterDefinitions;
    }

    std::string ActionGetPersistentVar::getType() const
    {
        return ActionGetPersistentVar::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionGetPersistentVar::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        std::string name = getParameter("LOCAL_NAME");
        if (checkRegexMatch(name, "^[A-Za-z_]*$")) {
            vars->push_back(
                    new statement_output_variable_definition(name, "User defined variable",
                            "UNKNOWN"));
        }
        return vars;
    }

    void ActionGetPersistentVar::execute(ExecutionState* state)
    {
        state->setUserProperty(getParameter("LOCAL_NAME", state),
                PersistentStorage::get()->getUserValue(
                        state->getProcedureUuid() + "_" + getParameter("PERSISTENT_NAME", state),
                        getParameter("DEFAULT_VALUE", state)));
    }

} /* namespace actiontrigger */
