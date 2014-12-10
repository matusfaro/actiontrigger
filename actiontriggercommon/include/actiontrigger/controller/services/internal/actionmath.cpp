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

#include "actiontrigger/controller/services/internal/actionmath.hpp"

namespace actiontrigger
{

    std::string ActionMath::TYPE = "ACTION_ActionMath";

    std::vector<ParameterDefinition*>* ActionMath::parameterDefinitions = NULL;
    Logger* ActionMath::StatementLOG = LoggerFactory::getInstance("ActionMath");
    const StatementInfo ActionMath::info(false, "Math operations", ActionMath::TYPE,
            "Compute a math operation on the given inputs. Where applicable, Radians are always used.");

    const StatementInfo ActionMath::getInfo()
    {
        return ActionMath::info;
    }

    ActionMath::ActionMath(StatementModel* model) :
            Action(model)
    {
    }

    ActionMath::~ActionMath()
    {
    }

    std::vector<ParameterDefinition*>* ActionMath::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            // Operation
            ParameterDefinition* state = ParameterDefinition::getDropdown("OPERATION", "Operation",
                    "mathematical operation to perform.", "Action");
            state->dropdownAddOption("x+y Addition", "ADD");
            state->dropdownAddOption("x-y Subtraction", "SUBTRACT");
            state->dropdownAddOption("x*y Multiplication", "MULTIPLY");
            state->dropdownAddOption("x^y Power of", "POWER");
            state->dropdownAddOption("-x Negative", "NEGATIVE");
            state->dropdownAddOption("sqrt(x) Square root", "SQRT");
            state->dropdownAddOption("cbrt(x) Cubic root", "SQRT");
            state->dropdownAddOption("hypot(x) Hypotenuse", "SQRT");
            state->dropdownAddOption("log(x) Natural log", "LOG");
            state->dropdownAddOption("ln(x) Common log", "LN");
            state->dropdownAddOption("sin(x) Sine", "SIN");
            state->dropdownAddOption("cos(x) Cosine", "COS");
            state->dropdownAddOption("tan(x) Tangent", "TAN");
            state->dropdownAddOption("acos(x) Arc cosine", "ACOS");
            state->dropdownAddOption("asin(x) Arc sine", "ASIN");
            state->dropdownAddOption("atan(x) Arc Tangent", "ATAN");
            state->dropdownAddOption("cosh(x) Hyperbolic cosine", "COSH");
            state->dropdownAddOption("sinh(x) Hyperbolic sine", "SING");
            state->dropdownAddOption("tanh(x) Hyperbolic tangent", "TANH");
            state->dropdownAddOption("acosh(x) Arc hyperbolic cosine", "ACOSH");
            state->dropdownAddOption("asinh(x) Arc hyperbolic sine", "ASINH");
            state->dropdownAddOption("atanh(x) Arc hyperbolic tangent", "ATANH");
            state->dropdownAddOption("x mod y Modulus", "MOD");
            state->dropdownAddOption("|x| Absolute value", "ABS");
            parameterDefinitions->push_back(state);
            // Inputs
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("X", "X",
                            "The X value of the mathematical operation."));
            parameterDefinitions->push_back(
                    ParameterDefinition::getNumber("Y", "Y",
                            "The Y value of the mathematical operation. May not be used depending on the operation."));
        }
        return parameterDefinitions;
    }

    std::string ActionMath::getDefaultParameter(std::string key)
    {
        if (key == "OPERATION") {
            return "ADD";
        } else if (key == "X") {
            return "1";
        } else if (key == "Y") {
            return "2";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionMath::getType() const
    {
        return ActionMath::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionMath::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("MATH_OUTPUT",
                        "Result of mathematical operation " + getParameter("OPERATION")
                                + " or 0 if error occured.", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MATH_VALID",
                        "TRUE if last operation was successful, FALSE otherwise. ", "BOOL"));
        return vars;
    }

    void ActionMath::execute(ExecutionState* state)
    {
        std::string op = getParameter("OPERATION");

        double x = 0, y = 0, z = 0;
        try {
            x = Common::StringToNumber<double>(getParameter("X", state));
        } catch (...) {
        }
        try {
            y = Common::StringToNumber<double>(getParameter("Y", state));
        } catch (...) {
        }
        errno = EOK;
        if (op == "ADD") {
            z = x + y;
        } else if (op == "SUBTRACT") {
            z = x - y;
        } else if (op == "MULTIPLY") {
            z = x * y;
        } else if (op == "POWER") {
            z = std::pow(x, y);
        } else if (op == "NEGATIVE") {
            z = -x;
        } else if (op == "SQRT") {
            z = std::sqrt(x);
        } else if (op == "CBRT") {
            z = std::cbrt(x);
        } else if (op == "HYP") {
            z = std::hypot(x, y);
        } else if (op == "LOG") {
            z = std::log10(x);
        } else if (op == "LN") {
            z = std::log(x);
        } else if (op == "SIN") {
            z = std::sin(x);
        } else if (op == "COS") {
            z = std::cos(x);
        } else if (op == "TAN") {
            z = std::tan(x);
        } else if (op == "ASIN") {
            z = std::asin(x);
        } else if (op == "ACOS") {
            z = std::acos(x);
        } else if (op == "ATAN") {
            z = std::atan(x);
        } else if (op == "SINH") {
            z = std::sinh(x);
        } else if (op == "COSH") {
            z = std::cosh(x);
        } else if (op == "TANH") {
            z = std::tanh(x);
        } else if (op == "ASINH") {
            z = std::asinh(x);
        } else if (op == "ACOSH") {
            z = std::acosh(x);
        } else if (op == "ATANH") {
            z = std::atanh(x);
        } else if (op == "MOD") {
            z = fmod(x, y);
        } else if (op == "ABS") {
            z = std::abs(x);
        } else {
            state->getLogger()->error(SSTR("Unknown operation " << op));
            state->setUserProperty("MATH_VALID", "FALSE");
            state->setUserProperty("MATH_OUTPUT", "0");
            return;
        }

        if (errno == EOK) {
            state->setUserProperty("MATH_VALID", "TRUE");
            state->setUserProperty("MATH_OUTPUT", SSTR(z));
        } else {
            state->getLogger()->error(SSTR("Operation " << op << " failed: " << strerror(errno)));
            state->setUserProperty("MATH_VALID", "FALSE");
            state->setUserProperty("MATH_OUTPUT", "0");
        }
    }

} /* namespace actiontrigger */
