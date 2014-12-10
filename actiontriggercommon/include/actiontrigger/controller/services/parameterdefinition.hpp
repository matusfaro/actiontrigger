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

#ifndef PARAMETERDEFINITION_HPP_
#define PARAMETERDEFINITION_HPP_

#include <string>
#include <vector>
#include "actiontrigger/model/loggerfactory.hpp"
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FileType>

class ParameterDefinition
{
public:
    virtual ~ParameterDefinition();
    // Parameter definition
    std::string type;
    std::string key;
    std::string name;
    std::string description;
    std::string validation;
    std::vector<std::string> details;
    std::vector<std::vector<std::string> > options;
    bool showParams;

    // Parameters:

    // Label
    static ParameterDefinition* getLabel(std::string name, std::string description);

    // Checkbox
    static ParameterDefinition* getCheckbox(std::string key, std::string name,
            std::string description, bool showParams = true);

    // String
    static ParameterDefinition* getString(std::string key, std::string name,
            std::string description, std::string validation = "", bool showParams = true);

    // String
    static ParameterDefinition* getText(std::string key, std::string name, std::string description,
            std::string validation = "", bool showParams = true);

    // Number
    static ParameterDefinition* getNumber(std::string key, std::string name,
            std::string description, std::string validation = "^-?[0-9]+(.[0-9]+)?$",
            bool showParams = true);

    // Dropdown
    static ParameterDefinition* getDropdown(std::string key, std::string name,
            std::string description, std::string dropdownText = "", bool showParams = true);
    void dropdownAddOption(std::string name, std::string value);

    // List
    static ParameterDefinition* getList(std::string key, std::string name, std::string description,
            std::string validation);
    void listAddOption(std::string name, std::string value);

    // Slider
    static ParameterDefinition* getSlider(std::string key, std::string name,
            std::string description, std::string fromValue, std::string toValue,
            std::string stepSize = "1", bool showParams = true);

    // Filesystem dialog
    static ParameterDefinition* getFileDialog(std::string key, std::string name,
            std::string description, bb::cascades::pickers::FilePickerMode::Type filePickerMode,
            bb::cascades::pickers::FileType::Type fileType, std::string filter = "",
            std::string pathAppend = "", bool showParams = true);
    static ParameterDefinition* getDirectoryDialog(std::string key, std::string name,
            std::string description, std::string validation = "", bool showParams = true);

    // Date and time
    static ParameterDefinition* getTime(std::string key, std::string name, std::string description,
            std::string validation = "", bool showParams = true);

    // TriggerValue - Creates parameters for value requirements based on type
    static ParameterDefinition* getTriggerValue(std::string key, std::string name,
            std::string description, std::string type);

    // DropdownFilter - Display a different set of parameters based on dropdown selection
    // Create main dropdown, key will hold the selected filter value
    static ParameterDefinition* getDropdownFilter(std::string key, std::string name,
            std::string description, std::string validation = "");
    // Adds new item to dropdown to symbolize a filter of parameters. Optional description is shown below the dropdown
    void createFilter(std::string name, std::string value, std::string description = "");
    // Add a parameter to the lastly created filter. Parameter is added to the definition vector
    void addToFilter(ParameterDefinition* parameter, std::vector<ParameterDefinition*>* parameters);
    // Add a parameter to the lastly created filter. Index of the inserted parameter is expected. Useful when using a parameter in multiple filters
    void addToFilter(int definitionIndex);
private:
    ParameterDefinition(std::string type, std::string key, std::string name,
            std::string description, std::string validation = "", bool showParams = true);
};

#endif /* PARAMETERDEFINITION_HPP_ */
