import bb.cascades 1.2

/**
 * OBSOLETE
 */

Sheet {
    function init() {
        checkboxPriority.checked = Qt._actiontrigger.getSetting("HIGH_PRIO") == "TRUE"
        var loglevel = Qt._actiontrigger.getSetting("LOG_LEVEL");
        for (var i = 0; i < dropdownLogLevel.count(); i ++) {
            if (dropdownLogLevel.at(i).value == loglevel) {
                dropdownLogLevel.setSelectedIndex(i);
                break
            }
        }
    }
    content: Page {
        titleBar: TitleBar {
            title: qsTr("Settings") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    close()
                    destroy()
                }
            }
        }
        id: _Settings
        DefaultPageContainer {

            Label {
                text: "Settings"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }

            Space {
            }

            ScrollView {
                verticalAlignment: VerticalAlignment.Fill

                Container {

                    // Permissions
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        multiline: true
                        text: "Permissions"
                        preferredWidth: 9000
                    }
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        multiline: true
                        text: "For certain tasks, this application requires your premission to run. If you encounter a permissions denied error message, you need to allow the appropriate permission by going to the application permissions in the settings. You may need to restart the application for the permission changes to take effect."
                    }
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Open Settings"
                        onClicked: {
                            invocationAppPermissions.trigger("bb.action.OPEN")
                        }
                        attachedObjects: [
                            Invocation {
                                id: invocationAppPermissions
                                query {
                                    mimeType: "settings/view"
                                    invokeTargetId: "sys.settings.target"
                                    uri: "settings://permissions"
                                }
                            }
                        ]
                    }

                    Space {
                    }
                    Divider {
                    }
                    Space {
                    }

                    // Priority
                    DefaultContainer {
                        Label {
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                            multiline: true
                            text: "Run with high priority"
                            preferredWidth: 9000
                        }
                        CheckBox {
                            id: checkboxPriority
                            checked: false
                            onCheckedChanged: {
                                Qt._actiontrigger.setSetting("HIGH_PRIO", checked ? "TRUE" : "FALSE")
                            }
                        }
                    }
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        multiline: true
                        text: "All procedures run with lowest possible priority to ensure other applications are not affected. If checked, all procedures will run with the highest possible user priority. Warning: if a procedure misbehaves and attempts to use all available resources, you may experience system slow down if running with high priority."
                    }

                    Space {
                    }
                    Divider {
                    }
                    Space {
                    }

                    // LogLevel
                    DefaultContainer {
                        DropDown {
                            id: dropdownLogLevel
                            title: "LogLevel"
                            Option {
                                value: "NONE"
                                text: "None"
                            }
                            Option {
                                value: "ERROR"
                                text: "Error"
                            }
                            Option {
                                value: "WARNING"
                                text: "Warning"
                            }
                            Option {
                                value: "DEBUG"
                                text: "Debug"
                            }
                            Option {
                                value: "TRACE"
                                text: "Trace"
                            }
                            onSelectedValueChanged: {
                                onCheckedChanged:
                                {
                                    Qt._actiontrigger.setSetting("LOG_LEVEL", selectedValue)
                                }
                            }
                        }
                    }
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        multiline: true
                        text: "When running procedures, the amount of output you see is determined by the log level. For testing, you should use DEBUG or TRACE. For normal operation, use ERROR or WARNING."
                    }

                    // DEBUG menu
                    DefaultContainer {
                        visible: false

                        // Pause
                        DefaultContainer {
                            Label {
                                textStyle.base: SystemDefaults.TextStyles.TitleText
                                multiline: true
                                text: "Second pause between executions"
                                preferredWidth: 9000
                            }
                            CheckBox {
                                id: checkboxPause
                                checked: false
                                onCheckedChanged: {
                                    Qt._actiontrigger.setSetting("EXEC_PAUSE", checked ? "TRUE" : "FALSE")
                                }
                            }
                        }
                        Label {
                            textStyle.base: SystemDefaults.TextStyles.BodyText
                            multiline: true
                            text: "Procedures that are looping indefinitely, a second pause will be added between runs if checked. This mitigates resource consumption in the case of undesired infinite loops."
                        }

                        Divider {

                        }

                        Button {
                            id: crashButton
                            text: "Crash"
                            onClicked: {
                                Qt._actiontrigger.crash()
                            }
                        }
                    }
                }
            }
        }
        //        actions: [
        //            ActionItem {
        //                ActionBar.placement: ActionBarPlacement.OnBar
        //                title: "Defaults"
        //                imageSource: "asset:///images/bb10/ic_advanced_restore.png"
        //            },
        //            ActionItem {
        //                ActionBar.placement: ActionBarPlacement.OnBar
        //                title: "Help"
        //                imageSource: "asset:///images/bb10/ic_help.png"
        //            }
        //        ]

    }
}