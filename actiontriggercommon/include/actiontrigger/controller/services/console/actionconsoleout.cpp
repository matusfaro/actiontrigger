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

#include "actiontrigger/controller/services/console/actionconsoleout.hpp"

namespace actiontrigger
{

    std::string ActionConsoleOut::TYPE = "ACTION_PRINT_OUT";

    enum
    {
        PARAMETER_TEXT
    };

    std::vector<ParameterDefinition*>* ActionConsoleOut::parameterDefinitions = NULL;
    Logger* ActionConsoleOut::StatementLOG = LoggerFactory::getInstance("ConsoleOut");
    const StatementInfo ActionConsoleOut::info(false, "Console Out", ActionConsoleOut::TYPE,
            "Print text to the run log");

    const StatementInfo ActionConsoleOut::getInfo()
    {
        return ActionConsoleOut::info;
    }

    ActionConsoleOut::ActionConsoleOut(StatementModel* model) :
            Action(model)
    {
    }

    ActionConsoleOut::~ActionConsoleOut()
    {
        StatementLOG->trace("~ConsoleOut");
    }

    std::vector<ParameterDefinition*>* ActionConsoleOut::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("TEXT", "Text",
                            "Text to be printed to console"));
        }
        return parameterDefinitions;
    }

    std::string ActionConsoleOut::getDefaultParameter(std::string key)
    {
        if (key == "TEXT") {
            return "Hello!";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionConsoleOut::getType() const
    {
        return ActionConsoleOut::TYPE;
    }

    void ActionConsoleOut::execute(ExecutionState* state)
    {
        StatementLOG->trace("execute");
        state->getLogger()->info(getParameter("TEXT", state));
        std::cout << getParameter("TEXT", state) << std::endl;
    }

} /* namespace actiontrigger */
