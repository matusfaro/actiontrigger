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

#include "actiontrigger/controller/services/cover/actioncovercontent.hpp"

namespace actiontrigger
{

    std::string ActionCoverContent::TYPE = "ACTION_ActionCoverContent";

    std::vector<ParameterDefinition*>* ActionCoverContent::parameterDefinitions = NULL;
    Logger* ActionCoverContent::StatementLOG = LoggerFactory::getInstance("ActionCoverContent");
    const StatementInfo ActionCoverContent::info(false, "Update active frame content",
            ActionCoverContent::TYPE,
            "Updates the content of the cover screen shown when the application is minimized. Also known as active frame.");

    const StatementInfo ActionCoverContent::getInfo()
    {
        return ActionCoverContent::info;
    }

    ActionCoverContent::ActionCoverContent(StatementModel* model) :
            Action(model)
    {
    }

    ActionCoverContent::~ActionCoverContent()
    {
    }

    std::vector<ParameterDefinition*>* ActionCoverContent::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* state = ParameterDefinition::getText("TEXT", "Content",
                    "Text to display as content.");
            parameterDefinitions->push_back(state);
        }
        return parameterDefinitions;
    }

    std::string ActionCoverContent::getDefaultParameter(std::string key)
    {
        if (key == "TEXT") {
            return "Content";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionCoverContent::getType() const
    {
        return ActionCoverContent::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionCoverContent::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionCoverContent::execute(ExecutionState* state)
    {
        state->getGlobalResources()->setCoverContent(getParameter("TEXT", state));
    }

} /* namespace actiontrigger */
