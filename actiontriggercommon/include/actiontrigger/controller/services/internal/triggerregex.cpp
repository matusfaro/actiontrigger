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

#include "actiontrigger/controller/services/internal/triggerregex.hpp"

namespace actiontrigger
{

    std::string TriggerRegex::TYPE = "TRIGGER_TriggerRegex";
    std::vector<ParameterDefinition*>* TriggerRegex::parameterDefinitions = NULL;
    Logger* TriggerRegex::StatementLOG = LoggerFactory::getInstance("TriggerRegex");
    const StatementInfo TriggerRegex::info(true, "Regular Expression", TriggerRegex::TYPE,
            "Triggers if the regular expression matches the input string.");

    const StatementInfo TriggerRegex::getInfo()
    {
        return TriggerRegex::info;
    }

    TriggerRegex::TriggerRegex(StatementModel* model) :
            Trigger(model)
    {
    }

    std::string TriggerRegex::getType() const
    {
        return TriggerRegex::TYPE;
    }

    TriggerRegex::~TriggerRegex()
    {
    }

    std::vector<ParameterDefinition*>* TriggerRegex::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getLabel("",
                            "<html>Regular Expressions cheat sheet:\n"
                                    "\n<b>^</b> beginning of string"
                                    "\n<b>$</b> end of string"
                                    "\n<b>.</b> match any character"
                                    "\n<b>[...]</b> match single character from group ie [a-z] or [aeiouy] or [a-b3-6]"
                                    "\n<b>...|...</b> Match either or ie cat|dog"
                                    "\n<b>\\n</b> new line character"
                                    "\n<b>\\</b> escape character ie \\. matches literal dot"
                                    "\n<b>*</b> 0 or more ie a* matches any number of letter 'a's"
                                    "\n<b>+</b> 1 or more ie a+ matches at least one letter 'a'"
                                    "\n<b>?</b> 0 or 1 ie a? matches either nothing or single letter 'a'"
                                    "\n<b>{2,4}</b> between 2 and 4 ie a{2,4} matches either 'aa', 'aaa', or 'aaaa'"
                                    "</html>"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("INPUT", "Input", "Input string to match"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getText("REGEX", "Regex", "Regular Expression to run"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("CASE_SENSITIVE", "Case sensitive",
                            "Match case in match"));
            parameterDefinitions->push_back(
                    ParameterDefinition::getCheckbox("MATCH_NEWLINE", "Match newline",
                            "Treat newline as ordinary character"));
        }
        return parameterDefinitions;
    }

    std::string TriggerRegex::getDefaultParameter(std::string key)
    {
        if (key == "INPUT") {
            return "";
        } else if (key == "REGEX") {
            return "";
        } else if (key == "CASE_SENSITIVE") {
            return "FALSE";
        } else if (key == "MATCH_NEWLINE") {
            return "FALSE";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    bool TriggerRegex::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        std::string inputStr = getParameter("INPUT", state);
        std::string regexStr = getParameter("REGEX", state);

        int flags = REG_EXTENDED | REG_NOSUB;
        if (!isValueTrue(getParameter("CASE_SENSITIVE"))) {
            flags |= REG_ICASE;
        }
        if (!isValueTrue(getParameter("MATCH_NEWLINE"))) {
            flags |= REG_NEWLINE;
        }

        regex_t exp;
        if (0 != regcomp(&exp, regexStr.c_str(), flags)) {
            RUNLOG->error("Error compiling regular expression.");
            return false;
        }
        if (0 != regexec(&exp, inputStr.c_str(), 0, NULL, 0)) {
            RUNLOG->debug("String is not a match");
            return false;
        }
        RUNLOG->debug("String is a match");
        return true;
    }

    bool TriggerRegex::isEventOnlyTrigger()
    {
        return false;
    }

} /* namespace actiontrigger */
