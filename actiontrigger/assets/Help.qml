import bb.cascades 1.2

Sheet {
    property string topic

    function gotoTop() {
        gotoPoint(0, 0)
    }
    function gotoItem(layoutUpdateHandler) {
        gotoPoint(layoutUpdateHandler.layoutFrame.x, layoutUpdateHandler.layoutFrame.y)
    }
    function gotoPoint(x, y) {
        console.log("Scrolling to ", x, y)
        scrollView.scrollToPoint(x, y, ScrollAnimation.Smooth)
    }
    content: Page {
        titleBar: TitleBar {
            title: qsTr("Help") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    close()
                    destroy()
                }
            }
        }
        ScrollView {
            id: scrollView
            Container {

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Introduction"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorIntro
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "ActionTrigger is an application that allows you to take control of your mobile device. You can create custom procedures that execute specific actions in response to triggers."
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Table of content"
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Breakdown</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorBreakdown)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Runtime variables</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorVariables)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Backups via Import and Export</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorBackup)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Dashboard</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorOverview)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Library</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorLibrary)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Procedure page</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorProcedure)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Selecting an Action or Trigger</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorSelectAction)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Action/Trigger page</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorStatementPage)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Selecting a variable</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorSelectVariable)
                    }
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Breakdown"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorBreakdown
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>A set of ordered Actions that can be executed in one unit is called a procedure. Actions have two basic subclasses:</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Trigger</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>A Trigger is an Action that is able to determine a state of a resource. Execution of a Trigger will halt execution until all the defined state requirements of the Trigger are met. An example of a Trigger is checking the state of Bluetooth power or waiting to receive the next BBM message.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Branch</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>A Branch contains one or more chain of Actions or Triggers. As part of the Branch execution, any of the chains can be triggered based on the functionality of the Branch. An Action chain can contain Actions, Triggers and other Branches while a Trigger chain can only contain Triggers. A Trigger chain can be setup to trigger once all Triggers are active or at least one is active. A simple example is an If-Then-Else Branch that contains one 'If' Trigger chain and two Action chains 'Then' and 'Else'. The Action chains are executed based on the outcome of the Trigger chain.</html>"
                }
                
                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Runtime variables"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorVariables
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>Most actions set variables with various data. Every Action parameter can be given a variable that will be replaced by the value of that variable during runtime. A variable is enclosed by curly braces: {MY_VARIABLE} Variables are only accessible by the executing procedure and they are lost when the procedure stops. For permanent storage or cross-procedure communication, see the Global and Persistent variables.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Backups via Import and Export"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorBackup
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>Procedures can be exported to a plain text JSON file on your device for backup or for sharing with your friend. Do not import procedures if you don't trust the origininal author of the file. The import or the execution of the imported procedures can allow malicious activity on your device.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Dashboard page"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorOverview
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "The dashboard allows you to quickly start/stop procedures and view currently running procedures. You can toggle between your favourite procedures and all procedures sorted in groups by toggling the bottom right button."
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "Each square corresponds to a procedure. A green square signifies that the procedure is running. By pressing the procedure square, you can start or stop a procedure. Once a procedure finishes, the green background will change back to its original color."
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Library page"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorLibrary
                            onLayoutFrameChanged: {
                                if (topic == "library") gotoItem(anchorLibrary)
                            }
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "The library displays all procedures. You can filter your procedure based on name using regular expressions."
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>To backup your procedures to a file, either open the right hand side menu and click <strong>Export All</strong> or select the procedures you want to export and press <strong>Export Selected</strong>. See backups for more information:</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Backups via Import and Export</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorBackup)
                    }
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>Creating a new procedure can be accomplished within the right hand side menu by clicking on <strong>New</strong> that will direct you to the procedure page. See the next section for more information.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Procedure page"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorProcedure
                            onLayoutFrameChanged: {
                                if (topic == "procedure") gotoItem(anchorProcedure)
                            }
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "Procedure contains a list of actions that will be executed in sequence. The procedure page allows you to edit and test your procedure."
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Edit</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>When editing your procedure, a list of Actions belonging to the procedure is displayed. Each Action can have branches that contain other either Action or Trigger lists. You can add Actions or Triggers by clicking on the plus sign that will take you to an Action or Trigger selection page. Actions can be re-ordered or deleted by holding the item until the right hand side menu opens up. </html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Settings</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>You can change the <strong>Name</strong> of your procedure that will be visible in the dashboard and library page. The <strong>Group</strong> name allows procedures with the same group to be grouped together when displaying.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html><strong>Favourite</strong> procedures are your most frequently used procedures that can be filtered in the dashboard.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>Procedures marked to <strong>Start On App Startup</strong> will be automatically started when ActionTrigger application is started.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>When the last action is executed in a procedure, the <strong>Loop Actions Forever</strong> setting causes the execution to continue from beginning indefinitely until stopped by user or another procedure.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Run it</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>When you are ready to test out your procedure, you can click on <strong>Run it</strong> to start the procedure in debug mode. You can observe the <strong>Runtime Variables</strong> that show you the variables that have been set as part of the procedure and the <strong>Run Log</strong> for any further information about what is currently happening.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Selecting an Action or Trigger"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorSelectAction
                            onLayoutFrameChanged: {
                                if (topic == "selectAction") gotoItem(anchorSelectAction)
                            }
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>Selecting an Action or a Trigger is as easy as clicking on the plus sign next to your desired item. Your item will be placed at the end of the chain. Multiple clicks on the plus sign will continuously add more items at the end of the chain. You can filter items based on a regular expression filter. When you are done adding items, navigate back by clicking the back button.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Action/Trigger page"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorStatementPage
                            onLayoutFrameChanged: {
                                if (topic == "statement") gotoItem(anchorStatementPage)
                            }
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Edit</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>When editing your Action or Trigger, a list of properties are shown that allow you to customize the runtime of your item. The properties vary based on the type of the Action or Trigger.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    multiline: true
                    text: "<html>Run it</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>When you are ready to test out your Action or Trigger, you can click on <strong>Run it</strong> to start the item in debug mode. You can observe the <strong>Runtime Variables</strong> that show you the variables that have been set as part of the item and the <strong>Run Log</strong> for any further information about what is currently happening.</html>"
                }

                Space {
                }
                Space {
                }

                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Selecting a variable"
                    multiline: true
                    attachedObjects: [
                        LayoutUpdateHandler {
                            id: anchorSelectVariable
                            onLayoutFrameChanged: {
                                if (topic == "selectVariable") gotoItem(anchorSelectVariable)
                            }
                        }
                    ]
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>A property of an Action or Trigger can be set with a value or a variable. On this page, you can select from a list of variables that were detected as part of this procedure. Click on the plus sign to set the property to the variable. Some properties allow you to have multiple variables and you can add them by clicking the plus sign several times. Navigate back to the property by clicking on the back button.</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    multiline: true
                    text: "<html>To learn how variables work, please see this section:</html>"
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "<html style='text-decoration:underline'>Runtime variables</html>"
                    textFormat: TextFormat.Html
                    onTouch: {
                        if (event.isDown())
                            gotoItem(anchorVariables)
                    }
                }

                Space {
                }
                Space {
                }

                Space {
                }
                Space {
                }

            }
        }
    }
}
