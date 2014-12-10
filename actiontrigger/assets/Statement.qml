import bb.cascades 1.2
import ActionTrigger 1.0

Page {
    property StatementView statement
    property int stepRollback: 0

    property bool active: false
    id: root
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    function init() {
        Qt._statementPage = root
        statementNameLabel.text = statement.name
        statementDescriptionLabel.text = statement.description
        var parameters = statement.getParameters();
        // Display parameters
        Qt.StatementPageParametersList = parameters
        for (var i = 0; i < parameters.length; ++ i) {
            var parameter = parameters[i]
            console.log("Parameter: ", parameter)
            console.log("name: ", parameter.name)
            console.log("type: '" + parameter.type + "'")
            parameter.notifyChangeValue.connect(notifyParameterValueChanged)
            var parameterContainer
            switch (parameter.type) {
                case "DROPDOWNBRANCH":
                    parameterContainer = parameterTypeDropdownFilter.createObject()
                    break;
                case "TRIGGERVALUE":
                    parameterContainer = parameterTriggerValue.createObject()
                    break;
                case "SLIDER":
                    parameterContainer = parameterTypeSlider.createObject()
                    break;
                case "DROPDOWN":
                    parameterContainer = parameterTypeDropdown.createObject()
                    break;
                case "LABEL":
                    parameterContainer = parameterTypeLabel.createObject()
                    break;
                case "LIST":
                    parameterContainer = parameterTypeList.createObject()
                    break;
                case "FILE_DIALOG":
                    parameterContainer = parameterTypeFileDialog.createObject()
                    break;
                case "DIR_DIALOG":
                    parameterContainer = parameterTypeDirDialog.createObject()
                    break;
                case "TIME":
                    parameterContainer = parameterTypeTime.createObject()
                    break;
                case "CHECKBOX":
                    parameterContainer = parameterTypeCheckbox.createObject()
                    break;
                case "TEXTAREA":
                    parameterContainer = parameterTypeText.createObject()
                    break;
                default:
                    console.log("Warning: could not determine parameter type: ", parameter.type)
                case "NUMBER":
                case "STRING":
                    parameterContainer = parameterTypeString.createObject()
                    break;
            }
            parameterContainer.index = i
            parametersContainer.add(parameterContainer);
        }
        // Once all parameters are present, initialize all of them
        for (var i = 0; i < parameters.length; ++ i) {
            console.log("Initializing: ", i)
            parametersContainer.at(i).init()
        }
        activity.stop()
        active = true
    }
    function notifyParameterValueChanged(key, value) {
        // Tutorial next step
        if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "setRequirement") {
            if (key.match(Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].key) == key && value.match(Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].value) == value) {
                Qt._ProcedurePage.step ++
            } else {
                console.log("Requirement not according to tutorial:", key, value)
            }
        }
    }
    function cleanup() {
        console.log("About to clean up run")
        // Make sure we shutdown any running statements when popping page
        if (segmentEditTry.selectedOption.value == "RUN") {
            statement.stop();
            console.log("Successfully cleaned up statement run")
        }
        
        // Tutorial next step
        if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "all") {
            // Tutorial done or not active, continue
        } else if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "back") {
            // Back is expected, increment step
            Qt._ProcedurePage.step ++
        } else if (stepRollback == -1) {
            // Rollback disabled
        } else {
            // Rollback step
            Qt._ProcedurePage.step = stepRollback
        }
    }
    DefaultPageContainer {

        DefaultContainer {
            DefaultActivity {
                id: activity
            }
            // NAME
            Label {
                id: statementNameLabel
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }

        SegmentedControl {
            id: segmentEditTry
            Option {
                id: segmentEdit
                value: "EDIT"
                text: "Edit"
                onSelectedChanged: {
                    if (active) {
                        if (selected) {
                            viewable.visible = false
                            statement.stop();
                            editable.visible = true
                        }
                    }
                }
            }
            Option {
                id: segmentRun
                value: "RUN"
                text: "Try it"
                onSelectedChanged: {
                    if (active) {
                        if (selected) {
                            editable.visible = false
                            listViewVarListDataModel.clear()
                            listViewRunLogDataModel.clear()
                            varListEmptyLabel.visible = true
                            runLogEmptyLabel.visible = true
                            statement.start(listViewRunLogDataModel, listViewVarListDataModel);
                            viewable.visible = true
                        }
                    }
                }
            }
        }

        Container {
            visible: false
            id: viewable
            verticalAlignment: VerticalAlignment.Fill

            Container {
                SegmentedControl {
                    id: segmentLogVar
                    Option {
                        id: segmentLog
                        text: "Log"
                        onSelectedChanged: {
                            if (active) {
                                if (selected) {
                                    runvar.visible = false
                                    runlog.visible = true
                                }
                            }
                        }
                    }
                    Option {
                        id: segmentVar
                        text: "Variables"
                        onSelectedChanged: {
                            if (active) {
                                if (selected) {
                                    runlog.visible = false
                                    runvar.visible = true
                                }
                            }
                        }
                    }
                }

                Container {
                    id: runlog
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Run Log"
                    }
                    Indent {
                        Container { // Container for Indent
                            Label {
                                id: runLogEmptyLabel
                                textStyle.fontStyle: FontStyle.Italic
                                text: "EMPTY"
                            }
                            ListView {
                                id: listViewRunLog
                                scrollRole: ScrollRole.Main
                                dataModel: SlidingDataModel {
                                    id: listViewRunLogDataModel
                                    onItemAdded: {
                                        runLogEmptyLabel.visible = false
                                    }
                                    onItemsChanged: {
                                        runLogEmptyLabel.visible = false
                                    }
                                }
                                stickToEdgePolicy: ListViewStickToEdgePolicy.End
                                listItemComponents: [
                                    ListItemComponent {
                                        Label {
                                            textStyle.fontSize: FontSize.XSmall
                                            multiline: true
                                            text: ListItemData
                                        }
                                    }
                                ]
                            }
                        }

                    }
                }

                Container {
                    id: runvar
                    visible: false
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Runtime Variables"
                    }
                    Indent {
                        Container { // Container for Indent
                            id: varListContainer
                            Label {
                                id: varListEmptyLabel
                                textStyle.fontStyle: FontStyle.Italic
                                text: "EMPTY"
                            }
                            ListView {
                                id: listViewVarList
                                scrollRole: ScrollRole.Main
                                dataModel: GroupDataModel {
                                    id: listViewVarListDataModel
                                    grouping: ItemGrouping.None
                                    onItemAdded: {
                                        varListEmptyLabel.visible = false
                                    }
                                    onItemsChanged: {
                                        varListEmptyLabel.visible = false
                                    }
                                }
                                stickToEdgePolicy: ListViewStickToEdgePolicy.End
                                listItemComponents: [
                                    ListItemComponent {
                                        type: "header"
                                        Container {
                                            // Empty header
                                        }
                                    },
                                    ListItemComponent {
                                        type: "item"
                                        DefaultContainer {
                                            Label {
                                                textStyle.fontSize: FontSize.XSmall
                                                multiline: true
                                                preferredWidth: 9000
                                                text: ListItemData.key
                                                layoutProperties: StackLayoutProperties {
                                                    spaceQuota: 1
                                                }
                                            }
                                            Label {
                                                textStyle.fontSize: FontSize.XSmall
                                                textStyle.fontStyle: FontStyle.Italic
                                                multiline: true
                                                text: ListItemData.value
                                                layoutProperties: StackLayoutProperties {
                                                    spaceQuota: 1
                                                }
                                            }
                                        }
                                    }
                                ]
                            }
                        }

                    }
                }
            }
        }

        ScrollView {
            visible: true
            id: editable
            scrollRole: ScrollRole.Main
            verticalAlignment: VerticalAlignment.Fill
            Container {
                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "back" || Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "setRequirement"
                    text: Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].text
                }
                
                // DESCRIPTION
                Label {
                    id: statementDescriptionLabel
                    multiline: true
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }

                Space {
                }

                // Holds all parameters. It's important to keep the container clean as indexes matter.
                Container {
                    id: parametersContainer
                }
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: parameterTypeDropdownFilter
            ParameterDropdownFilter {
            }
        },
        ComponentDefinition {
            id: parameterTriggerValue
            ParameterTriggerValue {
            }
        },
        ComponentDefinition {
            id: parameterTypeSlider
            ParameterSlider {
            }
        },
        ComponentDefinition {
            id: parameterTypeText
            ParameterText {
            }
        },
        ComponentDefinition {
            id: parameterTypeString
            ParameterString {
            }
        },
        ComponentDefinition {
            id: parameterTypeDropdown
            ParameterDropdown {
            }
        },
        ComponentDefinition {
            id: parameterTypeList
            ParameterDropdown {
            }
        },
        ComponentDefinition {
            id: parameterTypeLabel
            ParameterLabel {
            }
        },
        ComponentDefinition {
            id: parameterTypeFileDialog
            ParameterFileDialog {
            }
        },
        ComponentDefinition {
            id: parameterTypeDirDialog
            ParameterDirDialog {
            }
        },
        ComponentDefinition {
            id: parameterTypeTime
            ParameterTime {
            }
        },
        ComponentDefinition {
            id: parameterTypeCheckbox
            ParameterCheckbox {
            }
        }
    ]

    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Help"
            imageSource: "asset:///images/bb10/ic_help.png"
            onTriggered: {
                var page = helpSheet.createObject()
                page.topic = "statement"
                page.open()
            }
        }
    ]
}
