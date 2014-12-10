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

#include "actiontrigger/controller/services/cover/actioncovertitle.hpp"

namespace actiontrigger
{

    std::string ActionCoverTitle::TYPE = "ACTION_ActionCoverTitle";

    std::vector<ParameterDefinition*>* ActionCoverTitle::parameterDefinitions = NULL;
    Logger* ActionCoverTitle::StatementLOG = LoggerFactory::getInstance("ActionCoverTitle");
    const StatementInfo ActionCoverTitle::info(false, "Update active frame title",
            ActionCoverTitle::TYPE,
            "Updates the title of the cover screen shown when the application is minimized. Also known as active frame.");

    const StatementInfo ActionCoverTitle::getInfo()
    {
        return ActionCoverTitle::info;
    }

    ActionCoverTitle::ActionCoverTitle(StatementModel* model) :
            Action(model)
    {
    }

    ActionCoverTitle::~ActionCoverTitle()
    {
    }

    std::vector<ParameterDefinition*>* ActionCoverTitle::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getString("TEXT", "Title",
                    "Text to display as title");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionCoverTitle::getDefaultParameter(std::string key)
    {
        if (key == "TEXT") {
            return "ActionTrigger";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionCoverTitle::getType() const
    {
        return ActionCoverTitle::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionCoverTitle::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionCoverTitle::execute(ExecutionState* state)
    {
        state->getGlobalResources()->setCoverTitle(getParameter("TEXT", state));
    }

} /* namespace actiontrigger */
