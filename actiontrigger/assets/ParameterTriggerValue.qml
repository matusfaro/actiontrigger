import bb.cascades 1.2

Indent {
    property int index
    // Private properties
    property TextField currentActiveField
    property Option customOption
    property bool active: false
    function init() {
        var model = Qt.StatementPageParametersList[index]
        base.name = model.name
        console.log("name: " + model.name)
        base.description = model.description
        console.log("description: " + model.description)
        var type = model.getDetails()[0]
        console.log("Type: " + model.type)
        // Dynamic generation of requirement elements based on model
        if (type == "NUMBER") {
            textEqual.hintText = "Enter number"
            textEqual.inputMode = TextFieldInputMode.NumbersAndPunctuation
        }
        // Setup requirements
        var requirement = model.getSubValue("REQUIREMENT")
        initRequirementSelectionDropdown(type, requirement)
        showRequirement(requirement)
        active = true
    }
    function variableSelected(value) {
        currentActiveField.text += value
    }
    function initRequirementSelectionDropdown(type, requirement) {
        // Remove requirements not compatible with type
        switch (type) {
            case "BOOL":
                requirementSelection.remove(optionRegex)
                requirementSelection.remove(optionContains)
                requirementSelection.remove(optionEqual)
                requirementSelection.remove(optionDifference)
                requirementSelection.remove(optionNumberHigher)
                requirementSelection.remove(optionNumberHigherAbs)
                break
            case "STRING":
            default:
                requirementSelection.remove(optionDifference)
                requirementSelection.remove(optionNumberHigher)
                requirementSelection.remove(optionNumberHigherAbs)
            case "NUMBER":
                requirementSelection.remove(optionTrue)
                requirementSelection.remove(optionFalse)
                break
            // Requirements for custom input variable
            case "VARIABLE":
                requirementSelection.remove(optionChanged)
                requirementSelection.remove(optionDifference)
        }
        // Show appropriate option selected
        switch (requirement) {
            case "TRUE":
                optionTrue.selected = true
                break
            case "FALSE":
                optionFalse.selected = true
                break
            case "REGEX":
                optionRegex.selected = true
                break
            case "CONTAINS":
                optionContains.selected = true
                break
            case "EQUAL":
                optionEqual.selected = true
                break
            case "DIFFERENCE":
                optionDifference.selected = true
                break
            case "CHANGED":
                optionChanged.selected = true
                break
            case "NUMBER_HIGHER":
                optionNumberHigher.selected = true
                break
            case "NUMBER_HIGHER_ABS":
                optionNumberHigherAbs.selected = true
                break
            default:
                optionNone.selected = true
                break
        }
    }
    function showRequirement(requirement) {
        // Set appropriate requirement visible
        requirementRegex.visible = false
        requirementContains.visible = false
        requirementEqual.visible = false
        requirementDifference.visible = false
        requirementChanged.visible = false
        requirementNumberHigher.visible = false
        requirementNumberHigherAbs.visible = false
        containerNot.visible = true
        active = false
        switch (requirement) {
            case "TRUE":
            case "FALSE":
                containerNot.visible = false
                break
            case "REGEX":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // REGEX
                checkboxCaseInsensitive.checked = Qt.StatementPageParametersList[index].getSubValue("CASE_INSENSITIVE") == "TRUE"
                textRegex.text = Qt.StatementPageParametersList[index].getSubValue("MATCH")
                requirementRegex.visible = true
                break
            case "CONTAINS":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // CONTAINS
                textContains.text = Qt.StatementPageParametersList[index].getSubValue("MATCH")
                requirementContains.visible = true
                break
            case "EQUAL":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // EQUAL
                textEqual.text = Qt.StatementPageParametersList[index].getSubValue("MATCH")
                requirementEqual.visible = true
                break
            case "DIFFERENCE":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // DIFFERENCE
                requirementDifference.visible = true
                textDifferenceTolerance.text = Qt.StatementPageParametersList[index].getSubValue("TOLERANCE")
                break
            case "CHANGED":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // CHANGED
                requirementChanged.visible = true
                break
            case "NUMBER_HIGHER":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // NUMBER_HIGHER
                requirementNumberHigher.visible = true
                textNumberHigher.text = Qt.StatementPageParametersList[index].getSubValue("LOWER_BOUND")
                break
            case "NUMBER_HIGHER_ABS":
                // Checkbox
                checkboxNot.checked = Qt.StatementPageParametersList[index].getSubValue("NOT") == "TRUE"
                // NUMBER_HIGHER_ABS
                requirementNumberHigherAbs.visible = true
                textNumberHigherAbs.text = Qt.StatementPageParametersList[index].getSubValue("LOWER_BOUND")
                break
            default:
                containerNot.visible = false
                break
        }
        active = true
    }
    function setSubValue(subkey, value) {
        if (active) {
            Qt.StatementPageParametersList[index].setSubValue(subkey, value)
        }
    }

    Container { // Container for Indent
        // PARAMETER NAME DESCRIPTION
        ParameterBase {
            id: base
        }
        // Requirement selection
        Label {
            text: "Select the requirement type for this item."
            multiline: true
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
        }
        DropDown {
            id: requirementSelection
            title: "Requirement"
            options: [
                Option {
                    id: optionNone
                    text: "None"
                    value: ""
                    selected: true
                },
                Option {
                    id: optionTrue
                    text: "True"
                    value: "TRUE"
                },
                Option {
                    id: optionFalse
                    text: "False"
                    value: "FALSE"
                },
                Option {
                    id: optionRegex
                    text: "Regex"
                    value: "REGEX"
                },
                Option {
                    id: optionContains
                    text: "Contains"
                    value: "CONTAINS"
                },
                Option {
                    id: optionEqual
                    text: "Equality"
                    value: "EQUAL"
                },
                Option {
                    id: optionDifference
                    text: "Detect difference"
                    value: "DIFFERENCE"
                },
                Option {
                    id: optionChanged
                    text: "Detect Change"
                    value: "CHANGED"
                },
                Option {
                    id: optionNumberHigher
                    text: "HigherThan"
                    value: "NUMBER_HIGHER"
                },
                Option {
                    id: optionNumberHigherAbs
                    text: "HigherThanAbsoluteValue"
                    value: "NUMBER_HIGHER_ABS"
                }
            ]
            onSelectedValueChanged: {
                setSubValue("REQUIREMENT", selectedValue)
                showRequirement(selectedValue)
            }
        }
        // Requirements
        Indent {
            Container { // Container for Indent
            /* REGEX */
            Container {
                id: requirementRegex
                ParameterBase {
                    name: "Regex"
                    description: "Triggered when regular expression matches the value"
                }
                DefaultContainer {

                    // PARAMETER VALUE
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        text: "Case insensitive match"
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    // PARAMETER VALUE
                    CheckBox {
                        id: checkboxCaseInsensitive
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Center
                        bottomMargin: 50
                        onCheckedChanged: {
                            setSubValue("CASE_INSENSITIVE", checked ? "TRUE" : "FALSE");
                        }
                    }
                }
                DefaultContainer {
                    TextField {
                        id: textRegex
                        hintText: "^.*$"
                        onTextChanging: {
                            setSubValue("MATCH", text);
                        }
                    }
                    // VARIABLE
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textRegex

                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // REGEX
            /* CONTAINS */
            Container {
                id: requirementContains
                ParameterBase {
                    name: "Contains"
                    description: "Triggered when value contains given value"
                }
                DefaultContainer {
                    TextField {
                        id: textContains
                        onTextChanging: {
                            setSubValue("MATCH", text)
                        }
                    }
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textContains

                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // CONTAINS
            /* EQUAL */
            Container {
                id: requirementEqual
                ParameterBase {
                    name: "Equality"
                    description: "Triggered when value is equivalent to given value"
                }
                DefaultContainer {
                    TextField {
                        id: textEqual
                        onTextChanging: {
                            setSubValue("MATCH", text)
                        }
                    }
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textEqual

                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // EQUAL
            /* DIFFERENCE */
            Container {
                id: requirementDifference
                ParameterBase {
                    name: "Detect difference"
                    description: "Triggered when difference between measured value and previously measured value is higher than tolerance."
                }
                DefaultContainer {
                    TextField {
                        id: textDifferenceTolerance
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        hintText: "Enter tolerance"
                        onTextChanging: {
                            setSubValue("TOLERANCE", text)
                        }
                    }
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textDifferenceTolerance
                            
                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // DIFFERENCE
            /* CHANGED */
            Container {
                id: requirementChanged
                ParameterBase {
                    name: "Detect change"
                    description: "Triggered when measured value differs from the previously measured value. The initial case when there is no previous measured value will not trigger regardless of the state of 'Not'."
                }
            } // CHANGED
            /* NUMBER_HIGHER */
            Container {
                id: requirementNumberHigher
                ParameterBase {
                    name: "Higher than ..."
                    description: "Triggered when measured value is higher than this given value"
                }
                DefaultContainer {
                    TextField {
                        id: textNumberHigher
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        hintText: "Enter number"
                        onTextChanging: {
                            setSubValue("LOWER_BOUND", text)
                        }
                    }
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textNumberHigher

                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // NUMBER_HIGHER
            /* NUMBER_HIGHER_ABS */
            Container {
                id: requirementNumberHigherAbs
                ParameterBase {
                    name: "Higher than absolute value of ..."
                    description: "Triggered when the absolute value of the measured value is higher than this given value"
                }
                DefaultContainer {
                    TextField {
                        id: textNumberHigherAbs
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        hintText: "Enter number"
                        onTextChanging: {
                            setSubValue("LOWER_BOUND", text)
                        }
                    }
                    ParameterVariable {
                        onClicked: {
                            currentActiveField = textNumberHigherAbs

                            var page = selectVariablePage.createObject();
                            page.index = index
                            page.variableSelected.connect(variableSelected)
                            Qt._pane.push(page);
                        }
                    }
                }
            } // NUMBER_HIGHER_ABS
            /* NOT */
            Container {
                id: containerNot
                ParameterBase {
                    name: "Not"
                    description: "This property inverses the outcome of the requirement. If checked, the requirement will only be satisfied if the requirement is not satisfied."
                }
                DefaultContainer {
                    // PARAMETER VALUE
                    Label {
                        text: "Inverse requirement"
                        verticalAlignment: VerticalAlignment.Center
                    }
                    CheckBox {
                        id: checkboxNot
                        bottomMargin: 50
                        verticalAlignment: VerticalAlignment.Center
                        onCheckedChanged: {
                            setSubValue("NOT", checked ? "TRUE" : "FALSE");
                        }
                    }
                }
            }
            Space {
            }
        }}
        Space {
        }
        attachedObjects: [
            ComponentDefinition {
                id: factoryOption
                Option {
                }
            }
        ]
    }
}
