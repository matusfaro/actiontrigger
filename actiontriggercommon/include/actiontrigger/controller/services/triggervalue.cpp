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

#include "actiontrigger/controller/services/triggervalue.hpp"

namespace actiontrigger
{

    Logger* TriggerValue::StatementLOG = LoggerFactory::getInstance("TriggerValue");

    TriggerValue::TriggerValue(StatementModel* model) :
            Trigger(model), parameterDefinitions(NULL)
    {
        requirementToFunctionMap.insert(std::make_pair("TRUE", &TriggerValue::requirementTrue));
        requirementToFunctionMap.insert(std::make_pair("FALSE", &TriggerValue::requirementFalse));
        requirementToFunctionMap.insert(std::make_pair("REGEX", &TriggerValue::requirementRegex));
        requirementToFunctionMap.insert(
                std::make_pair("CONTAINS", &TriggerValue::requirementContains));
        requirementToFunctionMap.insert(std::make_pair("EQUAL", &TriggerValue::requirementEqual));
        requirementToFunctionMap.insert(
                std::make_pair("DIFFERENCE", &TriggerValue::requirementDetectDifference));
        requirementToFunctionMap.insert(
                std::make_pair("CHANGED", &TriggerValue::requirementDetectChange));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_IN_RANGE", &TriggerValue::requirementNumberInRange));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_IN_RANGE_OR_ON",
                        &TriggerValue::requirementNumberInRangeOrOn));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_HIGHER", &TriggerValue::requirementNumberHigher));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_HIGHER_OR_EQUAL",
                        &TriggerValue::requirementNumberHigherOrEqual));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_HIGHER_ABS",
                        &TriggerValue::requirementNumberHigherAbsoluteValue));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_LOWER", &TriggerValue::requirementNumberLower));
        requirementToFunctionMap.insert(
                std::make_pair("NUMBER_LOWER_OR_EQUAL",
                        &TriggerValue::requirementNumberLowerOrEqual));
    }

    TriggerValue::~TriggerValue()
    {
    }

    std::vector<ParameterDefinition*>* TriggerValue::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            std::vector<trigger_output_variable_definition*>* valueDefinitions =
                    getValueDefinitions();
            // Granularity parameter - Let's not give the user this much control
//        parameterDefinitions->push_back(
//                ParameterDefinition::getSlider("GRANULARITY", "Granularity", "Amount of time (milliseconds) to wait between checking again", "0",
//                        "10000", "100"));
            if (valueDefinitions->size() > 1) {
                // Display AND or OR operator to apply to multiple requirements only if multiple requirements are present
                parameterDefinitions->push_back(
                        ParameterDefinition::getCheckbox("TRIGGERVALUE_AND", "Satisfy all",
                                "Trigger will be successful only if all below requirements are met. Uncheck this value to have this trigger be successfull with at least one requirement met."));
            }
            // Rest of parameters and requirements
            for (std::vector<trigger_output_variable_definition*>::iterator it =
                    valueDefinitions->begin(); it != valueDefinitions->end(); ++it) {
                parameterDefinitions->push_back(
                        ParameterDefinition::getTriggerValue((*it)->key, (*it)->name,
                                (*it)->description, (*it)->type));
            }
        }
        return parameterDefinitions;
    }

    std::vector<statement_output_variable_definition*>* TriggerValue::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        std::vector<trigger_output_variable_definition*>* valueDefinitions = getValueDefinitions();
        for (std::vector<trigger_output_variable_definition*>::iterator it =
                valueDefinitions->begin(); it != valueDefinitions->end(); ++it) {
            // Note that the key in this case is the name
            vars->push_back(
                    new statement_output_variable_definition((*it)->key, (*it)->description,
                            (*it)->type));
        }
        return vars;
    }

    bool TriggerValue::checkRequirements(ExecutionState* state, std::vector<std::string>* values)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        bool doAnd = this->isValueTrue(this->getParameter("TRIGGERVALUE_AND"));
        std::vector<trigger_output_variable_definition*>* valueDefinitions = getValueDefinitions();
        std::vector<trigger_output_variable_definition*>::iterator itDef;
        std::vector<std::string>::iterator itVal;
        bool allValuesSkipped = true;
        if (values == NULL) {
            RUNLOG->error("Cannot retrieve values, trigger failed");
            return false;
        }
        for (itDef = valueDefinitions->begin(), itVal = values->begin();
                itDef != valueDefinitions->end() && itVal != values->end(); ++itDef, ++itVal) {
            trigger_output_variable_definition* definition = (*itDef);
            std::string value = (*itVal);

            // This doesn't belong here, but who's gonna stop me
            state->setUserProperty(definition->key, value);

            bool isReqMet = true;
            // Determine if value satisfies requirements
            std::string requirementType = this->getSubParameter(definition->key, "REQUIREMENT");
            if (!requirementType.empty()) {
                allValuesSkipped = false;
                try {
                    if (this->requirementToFunctionMap.find(requirementType)
                            != this->requirementToFunctionMap.end()) {
                        // Check appropriate requirement
                        isReqMet = (this->*requirementToFunctionMap[requirementType])(state,
                                definition->type, definition->key, value);
                        // Reverse outcome if NOT is set
                        if (this->isValueTrue(this->getSubParameter(definition->key, "NOT"))) {
                            isReqMet = !isReqMet;
                        }
                    } else {
                        RUNLOG->critical(
                                "Internal error: Unknown requirement type: " + definition->type);
                        return false;
                    }
                } catch (std::exception& e) {
                    RUNLOG->error(e.what());
                    isReqMet = false;
                }
                RUNLOG->debug(
                        "Requirement " + requirementType + " was "
                                + (isReqMet ? "met" : "not met"));
                if (!doAnd && isReqMet)
                    return true; /* If OR logic selected and one value is true, return true */
                if (doAnd && !isReqMet)
                    return false; /* If AND logic selected and one value is false, return false */
            } else {
                // No requirement set
                LOG->trace("Requirement not set");
            }
        }
        // If all values were skipped, return true
        // If OR logic selected, means no value was true, return false
        // If AND logic selected, means all values are true, return true
        if (allValuesSkipped) {
            LOG->trace("No requirements present");
            RUNLOG->debug("No requirements defined for this trigger");
        }
        return allValuesSkipped || doAnd;
    }

    bool TriggerValue::requirementRegex(ExecutionState* state, std::string type, std::string key,
            std::string value)
    {
        (void) type;
        return this->checkRegexMatch(value, this->getSubParameter(key, "MATCH", state),
                isValueTrue(this->getSubParameter(key, "CASE_INSENSITIVE", state)));
    }

    bool TriggerValue::requirementTrue(ExecutionState* state, std::string type, std::string key,
            std::string value)
    {
        (void) state;
        (void) type;
        (void) key;
        return this->isValueTrue(value);
    }

    bool TriggerValue::requirementFalse(ExecutionState* state, std::string type, std::string key,
            std::string value)
    {
        (void) state;
        (void) type;
        (void) key;
        return !this->isValueTrue(value);
    }

    bool TriggerValue::requirementContains(ExecutionState* state, std::string type, std::string key,
            std::string value)
    {
        (void) state;
        (void) type;
        std::string match = getSubParameter(key, "MATCH", state);
        return value.find(match) != std::string::npos;
    }

    bool TriggerValue::requirementEqual(ExecutionState* state, std::string type, std::string key,
            std::string value)
    {
        if (type == "NUMBER") {
            // Treat NUMBER as double
            double doubleMatch = Common::StringToNumber<double>(
                    getSubParameter(key, "MATCH", state));
            double doubleValue = Common::StringToNumber<double>(value);
            return Common::compare(doubleMatch, doubleValue);
        } else {
            // In case of Strings and unknown types
            std::string match = getSubParameter(key, "MATCH", state);
            return match == value;
        }
    }

    bool TriggerValue::requirementDetectDifference(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        bool changeDetected;
        do {
            if (type == "NUMBER") {
                // If this is the initial case, value should be false as we don't have the previous value to compare
                if (!isValueTrue(
                        state->getExecutionProperty(
                                "REQUIREMENT_DETECT_DIFFERENCE_ISSET_" + key))) {
                    state->setExecutionProperty("REQUIREMENT_DETECT_DIFFERENCE_ISSET_" + key,
                            "TRUE");
                    // If NOT is set, we need to return true since the value will be flipped outside of this method.
                    // This ensures the initial case is always false.
                    if (isValueTrue(getSubParameter(key, "NOT"))) {
                        changeDetected = true;
                        break;
                    }
                    changeDetected = false;
                    break;
                }
                // Compare value
                double currentValue = Common::StringToNumber<double>(value);
                double prevValue = Common::StringToNumber<double>(
                        state->getExecutionProperty("REQUIREMENT_DETECT_DIFFERENCE_VALUE_" + key));
                double tolerance = Common::StringToNumber<double>(
                        getSubParameter(key, "TOLERANCE", state));
                changeDetected = std::abs(prevValue - currentValue) > tolerance;
                break;
            } else {
                changeDetected = requirementDetectChange(state, type, key, value);
                break;
            }
        } while (false);

        state->setExecutionProperty("REQUIREMENT_DETECT_DIFFERENCE_VALUE_" + key, value);
        return changeDetected;
    }

    bool TriggerValue::requirementDetectChange(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        bool changeDetected;
        do {
            // If this is the initial case, value should be false as we don't have the previous value to compare
            if (!isValueTrue(
                    state->getExecutionProperty("REQUIREMENT_DETECT_CHANGE_ISSET_" + key))) {
                state->setExecutionProperty("REQUIREMENT_DETECT_CHANGE_ISSET_" + key, "TRUE");
                // If NOT is set, we need to return true since the value will be flipped outside of this method.
                // This ensures the initial case is always false.
                if (isValueTrue(getSubParameter(key, "NOT"))) {
                    changeDetected = true;
                    break;
                }
                changeDetected = false;
                break;
            }
            // Compare value
            if (state->getExecutionProperty("REQUIREMENT_DETECT_CHANGE_VALUE_" + key) == value) {
                changeDetected = false;
                break;
            }
            changeDetected = true;
        } while (false);

        state->setExecutionProperty("REQUIREMENT_DETECT_CHANGE_VALUE_" + key, value);
        return changeDetected;
    }

    bool TriggerValue::requirementNumberInRange(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleLowerBound = Common::StringToNumber<double>(
                getSubParameter(key, "LOWER_BOUND", state));
        double doubleUpperBound = Common::StringToNumber<double>(
                getSubParameter(key, "UPPER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleLowerBound < (doubleValue < doubleUpperBound);
    }

    bool TriggerValue::requirementNumberInRangeOrOn(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleLowerBound = Common::StringToNumber<double>(
                getSubParameter(key, "LOWER_BOUND", state));
        double doubleUpperBound = Common::StringToNumber<double>(
                getSubParameter(key, "UPPER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleLowerBound <= (doubleValue <= doubleUpperBound);
    }

    bool TriggerValue::requirementNumberHigher(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleLowerBound = Common::StringToNumber<double>(
                getSubParameter(key, "LOWER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleLowerBound < doubleValue;
    }

    bool TriggerValue::requirementNumberHigherOrEqual(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleLowerBound = Common::StringToNumber<double>(
                getSubParameter(key, "LOWER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleLowerBound <= doubleValue;
    }

    bool TriggerValue::requirementNumberHigherAbsoluteValue(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleLowerBound = Common::StringToNumber<double>(
                getSubParameter(key, "LOWER_BOUND", state));
        double doubleValue = abs(Common::StringToNumber<double>(value));
        return doubleLowerBound < doubleValue;
    }

    bool TriggerValue::requirementNumberLower(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleUpperBound = Common::StringToNumber<double>(
                getSubParameter(key, "UPPER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleValue <= doubleUpperBound;
    }

    bool TriggerValue::requirementNumberLowerOrEqual(ExecutionState* state, std::string type,
            std::string key, std::string value)
    {
        (void) type;
        double doubleUpperBound = Common::StringToNumber<double>(
                getSubParameter(key, "UPPER_BOUND", state));
        double doubleValue = Common::StringToNumber<double>(value);
        return doubleValue < doubleUpperBound;
    }

} /* namespace actiontrigger */
