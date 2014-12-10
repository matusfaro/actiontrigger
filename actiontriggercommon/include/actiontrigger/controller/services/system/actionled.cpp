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

#include "actiontrigger/controller/services/system/actionled.hpp"

namespace actiontrigger
{

    std::string ActionLed::TYPE = "ACTION_LED";
    std::vector<ParameterDefinition*>* ActionLed::parameterDefinitions = NULL;
    Logger* ActionLed::StatementLOG = LoggerFactory::getInstance("ActionLed");
    const StatementInfo ActionLed::info(false, "Led", ActionLed::TYPE,
            RichText::html(
                    "Blinks various colored LEDs. When invoked, LEDs are queued and will slowly blink away until the desired amount."
                            + RichText::newline()
                            + RichText::color("red",
                                    "Warning: Due to Blackberry restrictions, led may not blink immediately but on a predetermined interval.")));

    const StatementInfo ActionLed::getInfo()
    {
        return ActionLed::info;
    }

    ActionLed::ActionLed(StatementModel* model) :
            Action(model)
    {
    }

    ActionLed::~ActionLed()
    {
    }

    std::vector<ParameterDefinition*>* ActionLed::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            ParameterDefinition* param = ParameterDefinition::getDropdown("COLOR", "Color",
                    "LED Color to blink. ", "Colors");
            param->dropdownAddOption("Stop blinking!", "STOP");
            param->dropdownAddOption("Red", "RED");
            param->dropdownAddOption("Green", "GREEN");
            param->dropdownAddOption("Blue", "BLUE");
            param->dropdownAddOption("Yellow", "YELLOW");
            param->dropdownAddOption("Cyan", "CYAN");
            param->dropdownAddOption("Magenta", "MAGENTA");
            param->dropdownAddOption("White", "WHITE");
            parameterDefinitions->push_back(param);
            parameterDefinitions->push_back(
                    ParameterDefinition::getSlider("COUNT", "Blink count",
                            "Amount of times to blink. If color is set to Stop, this has no effect.",
                            "1", "20", "1")); // TODO Allow setting this to 0 to blink forever. Handle being able to cancel.
        }
        return parameterDefinitions;
    }

    std::string ActionLed::getDefaultParameter(std::string key)
    {
        if (key == "COLOR") {
            return "GREEN";
        } else if (key == "COUNT") {
            return "1";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionLed::getType() const
    {
        return ActionLed::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionLed::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionLed::execute(ExecutionState* state)
    {
        DataModelLogger* USERLOG = state->getLogger();
        errno = EOK;
        std::string blinkCountString = getParameter("COUNT");
        std::string color = getParameter("COLOR");
        int blinkCount = blinkCountString != "" ? Common::StringToNumber<int>(blinkCountString) : 1;
        state->getRuntimeResources()->bpsInitialize();
        if (color == "STOP") {
            if (BPS_SUCCESS != led_cancel("1")) {
                StatementLOG->warning(SSTR("Error starting BPS: " << strerror(errno)));
                USERLOG->error(SSTR("Error stopping LED: " << strerror(errno)));
            }
        } else {
            if (BPS_SUCCESS
                    != led_request_color("1", getColor(getParameter("COLOR")), blinkCount)) {
                StatementLOG->warning(SSTR("Error blinking LED: " << strerror(errno)));
                USERLOG->error(SSTR("Error blinking LED: " << strerror(errno)));
            }
        }
    }

    led_color_t ActionLed::getColor(std::string color)
    {
        if (color == "RED") {
            return LED_COLOR_RED;
        } else if (color == "GREEN") {
            return LED_COLOR_GREEN;
        } else if (color == "BLUE") {
            return LED_COLOR_BLUE;
        } else if (color == "YELLOW") {
            return LED_COLOR_YELLOW;
        } else if (color == "CYAN") {
            return LED_COLOR_CYAN;
        } else if (color == "MAGENTA") {
            return LED_COLOR_MAGENTA;
        } else if (color == "WHITE") {
            return LED_COLOR_WHITE;
        } else {
            return (led_color_t) 0x00000000;
        }
    }

} /* namespace actiontrigger */
