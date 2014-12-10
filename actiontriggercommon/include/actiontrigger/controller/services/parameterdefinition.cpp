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

#include "actiontrigger/controller/services/parameterdefinition.hpp"

ParameterDefinition::ParameterDefinition(std::string type, std::string key, std::string name,
        std::string description, std::string validation, bool showParams) :
        type(type), key(key), name(name), description(description), validation(validation), showParams(
                showParams)
{
}

ParameterDefinition::~ParameterDefinition()
{
    for (std::vector<std::vector<std::string> >::iterator it = this->options.begin();
            it != this->options.end(); ++it) {
        delete &it;
    }
}

// Label
ParameterDefinition* ParameterDefinition::getLabel(std::string name, std::string description)
{
    return new ParameterDefinition("LABEL", "", name, description);
}

// Checkbox
ParameterDefinition* ParameterDefinition::getCheckbox(std::string key, std::string name,
        std::string description, bool showParams)
{
    return new ParameterDefinition("CHECKBOX", key, name, description, "", showParams);
}

// String
ParameterDefinition* ParameterDefinition::getString(std::string key, std::string name,
        std::string description, std::string validation, bool showParams)
{
    return new ParameterDefinition("STRING", key, name, description, validation, showParams);
}

// Text area
ParameterDefinition* ParameterDefinition::getText(std::string key, std::string name,
        std::string description, std::string validation, bool showParams)
{
    return new ParameterDefinition("TEXTAREA", key, name, description, validation, showParams);
}

// Number
ParameterDefinition* ParameterDefinition::getNumber(std::string key, std::string name,
        std::string description, std::string validation, bool showParams)
{
    return new ParameterDefinition("NUMBER", key, name, description, validation, showParams);
}

// Dropdown
ParameterDefinition* ParameterDefinition::getDropdown(std::string key, std::string name,
        std::string description, std::string dropdownText, bool showParams)
{
    ParameterDefinition* def = new ParameterDefinition("DROPDOWN", key, name, description, "",
            showParams);
    def->details.push_back(dropdownText);
    return def;
}

void ParameterDefinition::dropdownAddOption(std::string name, std::string value)
{
    std::vector<std::string>* option = new std::vector<std::string>();
    option->push_back(name);
    option->push_back(value);
    this->options.push_back(*option);
    if (validation != "")
        validation += "|";
    validation += "^" + value + "$";
}

// Slider
ParameterDefinition* ParameterDefinition::getSlider(std::string key, std::string name,
        std::string description, std::string fromValue, std::string toValue, std::string stepSize,
        bool showParams)
{
    ParameterDefinition* def = new ParameterDefinition("SLIDER", key, name, description, "",
            showParams);
    def->details.push_back(fromValue);
    def->details.push_back(toValue);
    def->details.push_back(stepSize);
    return def;
}

// File path
ParameterDefinition* ParameterDefinition::getFileDialog(std::string key, std::string name,
        std::string description, bb::cascades::pickers::FilePickerMode::Type filePickerMode,
        bb::cascades::pickers::FileType::Type fileType, std::string filter, std::string pathAppend,
        bool showParams)
{
    ParameterDefinition* def = new ParameterDefinition("FILE_DIALOG", key, name, description, "",
            showParams);
    def->details.push_back(SSTR(filePickerMode));
    def->details.push_back(SSTR(fileType));
    def->details.push_back(filter);
    def->details.push_back(pathAppend);
    return def;
}

ParameterDefinition* ParameterDefinition::getDirectoryDialog(std::string key, std::string name,
        std::string description, std::string validation, bool showParams)
{
    ParameterDefinition* def = new ParameterDefinition("DIR_DIALOG", key, name, description,
            validation, showParams);
    return def;
}

// Date and time
ParameterDefinition* ParameterDefinition::getTime(std::string key, std::string name,
        std::string description, std::string validation, bool showParams)
{
    ParameterDefinition* def = new ParameterDefinition("TIME", key, name, description, validation,
            showParams);
    return def;
}

// List
ParameterDefinition* ParameterDefinition::getList(std::string key, std::string name,
        std::string description, std::string validation)
{
    return new ParameterDefinition("LIST", key, name, description, validation);
}

void ParameterDefinition::listAddOption(std::string name, std::string value)
{
    std::vector<std::string>* option = new std::vector<std::string>();
    option->push_back(name);
    option->push_back(value);
    this->options.push_back(*option);
}

// TriggerValue
ParameterDefinition* ParameterDefinition::getTriggerValue(std::string key, std::string name,
        std::string description, std::string type)
{
    ParameterDefinition* def = new ParameterDefinition("TRIGGERVALUE", key, name, description);
    def->details.push_back(type);
    return def;
}

// DropdownFilter
ParameterDefinition* ParameterDefinition::getDropdownFilter(std::string key, std::string name,
        std::string description, std::string validation)
{
    return new ParameterDefinition("DROPDOWNBRANCH", key, name, description, validation);
}

void ParameterDefinition::createFilter(std::string name, std::string value, std::string description)
{
    std::vector<std::string>* filter = new std::vector<std::string>();
    filter->push_back(name);
    filter->push_back(description);
    filter->push_back(value);
    this->options.push_back(*filter);
}

void ParameterDefinition::addToFilter(ParameterDefinition* parameter,
        std::vector<ParameterDefinition*>* parameters)
{
    // Push the parameter into the definition at that index
    parameters->push_back(parameter);
    // Push the parameter's index number into the last Filter's options to associate parameter with Filter
    addToFilter(parameters->size() - 1);
}

void ParameterDefinition::addToFilter(int definitionIndex)
{
    this->options.back().push_back(SSTR(definitionIndex));
}
