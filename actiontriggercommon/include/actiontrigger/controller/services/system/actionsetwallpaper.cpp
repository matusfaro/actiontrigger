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

#include "actiontrigger/controller/services/system/actionsetwallpaper.hpp"

namespace actiontrigger
{

    std::string ActionSetWallpaper::TYPE = "ACTION_SET_WALLPAPER";
    std::vector<ParameterDefinition*>* ActionSetWallpaper::parameterDefinitions = NULL;
    Logger* ActionSetWallpaper::StatementLOG = LoggerFactory::getInstance("ActionSetWallpaper");
    const StatementInfo ActionSetWallpaper::info(false, "Set Wallpaper", ActionSetWallpaper::TYPE,
            "Sets device wallpaper from given image path.");

    const StatementInfo ActionSetWallpaper::getInfo()
    {
        return ActionSetWallpaper::info;
    }

    ActionSetWallpaper::ActionSetWallpaper(StatementModel* model) :
            Action(model)
    {
    }

    ActionSetWallpaper::~ActionSetWallpaper()
    {
    }

    std::vector<ParameterDefinition*>* ActionSetWallpaper::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();
            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("PATH", "Image path",
                            "File path pointing to desired wallpaper image.",
                            bb::cascades::pickers::FilePickerMode::Picker,
                            bb::cascades::pickers::FileType::Picture));
        }
        return parameterDefinitions;
    }

    std::string ActionSetWallpaper::getType() const
    {
        return ActionSetWallpaper::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionSetWallpaper::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        return vars;
    }

    void ActionSetWallpaper::execute(ExecutionState* state)
    {
        std::string wallpaperPath = getParameter("PATH", state);
        if (BPS_SUCCESS != navigator_set_wallpaper(wallpaperPath.c_str())) {
            LOG->trace(SSTR("Unable to set wallpaper: " << strerror(errno)));
            state->getLogger()->error(SSTR("Unable to set wallpaper: " << strerror(errno)));
        } else {
            state->getLogger()->debug(SSTR("Wallpaper set to: " << wallpaperPath));
        }
    }

} /* namespace actiontrigger */
