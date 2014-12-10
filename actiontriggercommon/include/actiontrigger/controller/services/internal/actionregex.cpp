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

#include "actiontrigger/controller/services/internal/actionregex.hpp"

namespace actiontrigger
{

    std::string ActionRegex::TYPE = "ACTION_ActionRegex";

    std::vector<ParameterDefinition*>* ActionRegex::parameterDefinitions = NULL;
    Logger* ActionRegex::StatementLOG = LoggerFactory::getInstance("ActionRegex");
    const StatementInfo ActionRegex::info(false, "Regular Expression", ActionRegex::TYPE,
            "Executes regular expression on an input string and sets various variables with the result of the match.");

    const StatementInfo ActionRegex::getInfo()
    {
        return ActionRegex::info;
    }

    ActionRegex::ActionRegex(StatementModel* model) :
            Action(model)
    {
    }

    ActionRegex::~ActionRegex()
    {
    }

    std::vector<ParameterDefinition*>* ActionRegex::getParameterDefinitions()
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

    std::string ActionRegex::getDefaultParameter(std::string key)
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

    std::string ActionRegex::getType() const
    {
        return ActionRegex::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionRegex::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("MATCH_0", "Regex match",
                        "Regex full match"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_1", "Regex sub-match 1",
                        "Regex capturing group 1"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_2", "Regex sub-match 2",
                        "Regex capturing group 2"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_3", "Regex sub-match 3",
                        "Regex capturing group 3"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_4", "Regex sub-match 4",
                        "Regex capturing group 4"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_5", "Regex sub-match 5",
                        "Regex capturing group 5"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_6", "Regex sub-match 6",
                        "Regex capturing group 6"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_7", "Regex sub-match 7",
                        "Regex capturing group 7"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_8", "Regex sub-match 8",
                        "Regex capturing group 8"));
        vars->push_back(
                new statement_output_variable_definition("MATCH_9", "Regex sub-match 9",
                        "Regex capturing group 9"));
        return vars;
    }

    void ActionRegex::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        std::string inputStr = getParameter("INPUT", state);
        std::string regexStr = getParameter("REGEX", state);

        int flags = REG_EXTENDED;
        if (!isValueTrue(getParameter("CASE_SENSITIVE"))) {
            flags |= REG_ICASE;
        }
        if (!isValueTrue(getParameter("MATCH_NEWLINE"))) {
            flags |= REG_NEWLINE;
        }

        regex_t exp;
        if (0 != regcomp(&exp, regexStr.c_str(), flags)) {
            RUNLOG->error("Error compiling regular expression.");
            return;
        }
        regmatch_t inputMatches[10];
        if (0 != regexec(&exp, inputStr.c_str(), 10, inputMatches, 0)) {
            RUNLOG->info("String is not a match");
            return;
        }
        for (int i = 0; i < 10; i++) {
            if (inputMatches[i].rm_so == -1 || inputMatches[i].rm_eo == -1)
                break;
            state->setUserProperty(SSTR("MATCH_" << i),
                    inputStr.substr(inputMatches[i].rm_so,
                            inputMatches[i].rm_eo - inputMatches[i].rm_so));
        }
    }

} /* namespace actiontrigger */
