import bb.cascades 1.2
import bb.system 1.2

/*
 * Before instantiating this page, set the following global property:
 * Qt._statementPickerPage_showTriggerOnly
 */
Page {
    signal selected(string type)
    property int stepRollback: 0

    id: _Services
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    property bool active: false
    function init() {
        Qt._statementPickerPage = _Services
        Qt._statementPickerPage_activity = activity
        Qt._statementPickerPage_toastAdded = toastAdded

        // Categories
        infoList.dataModel = Qt._actiontrigger.getStatementGroupList();
        infoList.dataModel.resetFilter()

        // Search
        statementListSearch.dataModel = Qt._actiontrigger.getStatementSearchList();
        statementListSearch.dataModel.resetFilter()

        servicesPageTitle.text = "Select " + (Qt._statementPickerPage_showTriggerOnly ? "Trigger" : "Action")
        feedbackNote.visible = true
        activity.stop()
        active = true
    }
    function cleanup() {
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
            Label {
                id: servicesPageTitle
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        //        Label {
        //            visible: false
        //            multiline: true
        //            textStyle.base: SystemDefaults.TextStyles.SubtitleText
        //            text: "Pick something"
        //        }

        SegmentedControl {
            id: segmentEditTry
            Option {
                text: "Categories"
                onSelectedChanged: {
                    if (active) {
                        if (selected) {
                            search.visible = false
                            categories.visible = true
                        }
                    }
                }
            }
            Option {
                text: "Search"
                onSelectedChanged: {
                    if (active) {
                        if (selected) {
                            categories.visible = false
                            search.visible = true
                            textFieldFilter.requestFocus()
                        }
                    }
                }
            }
        }

        ScrollView {
            id: scrollview
            scrollRole: ScrollRole.Main

            Container {
                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "back" || Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionAtEnd" || Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionToBranch"
                    text: Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].text
                }

                // Search
                Container {
                    id: search
                    visible: false

                    // Filter
                    TextField {
                        id: textFieldFilter
                        onTextChanging: {
                            activity.start()
                            statementListSearch.dataModel.setFilter(text)
                            activity.stop()
                        }
                    }

                    // Filtered statements
                    Indent {
                        ListView {
                            id: statementListSearch
                            preferredHeight: 100
                            listItemComponents: [
                                ListItemComponent {
                                    type: "header"
                                    Container {
                                        // Empty header
                                    }
                                },
                                ListItemComponent {
                                    type: "item"
                                    NonStandardListItem {
                                        visible: ! Qt._statementPickerPage_showTriggerOnly || ListItemData.isTrigger
                                        id: listItem
                                        title: ListItemData.name
                                        flag: ListItemData.isTrigger ? "Trigger" : "Action"
                                        description: ListItemData.description
                                        selectImage: "asset:///images/bb10/ic_add.png"
                                        select.onTapped: {

                                            // Tutorial next step
                                            if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "all") {
                                                // Tutorial not running, continue
                                            } else if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "back") {
                                                Qt._statementPickerPage_toastAdded.cancel()
                                                Qt._statementPickerPage_toastAdded.body = "That's not right. Click the Back button."
                                                Qt._statementPickerPage_toastAdded.show()
                                                return
                                            } else if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionAtEnd" || Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionToBranch") {
                                                if (ListItemData.type.match(Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].constraint) == ListItemData.type) {
                                                    Qt._ProcedurePage.step ++
                                                } else {
                                                    Qt._statementPickerPage_toastAdded.cancel()
                                                    Qt._statementPickerPage_toastAdded.body = "Oops, that's not the one"
                                                    Qt._statementPickerPage_toastAdded.show()
                                                    return
                                                }
                                            }

                                            Qt._statementPickerPage_activity.start()

                                            // Send signal
                                            Qt._statementPickerPage.selected(ListItemData.type)

                                            // Handle received signals
                                            Qt._actiontrigger.processEventLoop

                                            Qt._statementPickerPage_activity.stop()
                                            Qt._statementPickerPage_toastAdded.cancel()
                                            Qt._statementPickerPage_toastAdded.body = ListItemData.name + " added"
                                            Qt._statementPickerPage_toastAdded.show()
                                        }

                                        attachedObjects: [
                                            // Auto height parent ListView
                                            LayoutUpdateHandler {
                                                property int propagatedHeight: 0
                                                onLayoutFrameChanged: {
                                                    if (layoutFrame.height == Number.POSITIVE_INFINITY)
                                                        return
                                                    if (Qt._statementPickerPage_showTriggerOnly && ! ListItemData.isTrigger)
                                                        return
                                                    var heightToPropagate = layoutFrame.height - propagatedHeight
                                                    parent.parent.preferredHeight = parent.parent.preferredHeight + heightToPropagate
                                                    propagatedHeight = layoutFrame.height
                                                }
                                            }
                                        ]
                                    }
                                }
                            ]
                        }
                    }
                }

                // Categories
                Container {
                    id: categories
                    preferredHeight: 100

                    ListView {
                        id: infoList
                        objectName: "listServices"
                        layout: FlowListLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                        listItemComponents: [
                            ListItemComponent {
                                type: "header"
                                Container {
                                    // Empty header
                                }
                            },
                            ListItemComponent {
                                type: "item"
                                NonStandardListItem {
                                    visible: ! Qt._statementPickerPage_showTriggerOnly || ListItemData.containsTrigger
                                    id: root
                                    title: ListItemData.name
                                    //                            description: ListItemData.description
                                    showSelect: false
                                    tap.onTapped: {
                                        if (statementListContainer.visible == true) {
                                            statementListContainer.visible = false
                                        } else {
                                            //                                        statementList.dataModel = ListItemData.getInfoList()
                                            statementListContainer.visible = true
                                        }
                                    }
                                    Indent {
                                        preferredHeight: 1
                                        id: statementListContainer
                                        visible: false
                                        ListView {
                                            dataModel: ListItemData.getInfoList()
                                            //                                property LayoutUpdateHandler statementListLayout: statementListLayout
                                            //                                preferredHeight: 300
                                            //                                maxHeight: dataModel.childCount(0) * 500
                                            id: statementList
                                            listItemComponents: [
                                                ListItemComponent {
                                                    type: "header"
                                                    Container {
                                                        // Empty header
                                                    }
                                                },
                                                ListItemComponent {
                                                    type: "item"
                                                    NonStandardListItem {
                                                        visible: ! Qt._statementPickerPage_showTriggerOnly || ListItemData.isTrigger
                                                        id: listItem
                                                        title: ListItemData.name
                                                        flag: ListItemData.isTrigger ? "Trigger" : "Action"
                                                        description: ListItemData.description
                                                        selectImage: "asset:///images/bb10/ic_add.png"
                                                        select.onTapped: {

                                                            // Tutorial next step
                                                            if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "all") {
                                                                // Tutorial not running, continue
                                                            } else if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "back") {
                                                                Qt._statementPickerPage_toastAdded.cancel()
                                                                Qt._statementPickerPage_toastAdded.body = "That's not right. Click the Back button."
                                                                Qt._statementPickerPage_toastAdded.show()
                                                                return
                                                            } else if (Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionAtEnd" || Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].action == "addActionToBranch") {
                                                                if (ListItemData.type.match(Qt._ProcedurePage.tutorialSteps[Qt._ProcedurePage.step].constraint) == ListItemData.type) {
                                                                    Qt._ProcedurePage.step ++
                                                                } else {
                                                                    Qt._statementPickerPage_toastAdded.cancel()
                                                                    Qt._statementPickerPage_toastAdded.body = "Oops, that's not the one"
                                                                    Qt._statementPickerPage_toastAdded.show()
                                                                    return
                                                                }
                                                            }

                                                            Qt._statementPickerPage_activity.start()

                                                            // Send signal
                                                            Qt._statementPickerPage.selected(ListItemData.type)

                                                            // Handle received signals
                                                            Qt._actiontrigger.processEventLoop

                                                            Qt._statementPickerPage_activity.stop()
                                                            Qt._statementPickerPage_toastAdded.cancel()
                                                            Qt._statementPickerPage_toastAdded.body = ListItemData.name + " added"
                                                            Qt._statementPickerPage_toastAdded.show()
                                                        }

                                                        attachedObjects: [
                                                            // Auto height parent ListView
                                                            LayoutUpdateHandler {
                                                                property int propagatedHeight: 0
                                                                onLayoutFrameChanged: {
                                                                    if (layoutFrame.height == Number.POSITIVE_INFINITY)
                                                                        return
                                                                    if (Qt._statementPickerPage_showTriggerOnly && ! ListItemData.isTrigger)
                                                                        return
                                                                    var heightToPropagate = layoutFrame.height - propagatedHeight
                                                                    parent.parent.parent.preferredHeight = parent.parent.parent.preferredHeight + heightToPropagate
                                                                    propagatedHeight = layoutFrame.height
                                                                }
                                                            }
                                                        ]
                                                    }
                                                }
                                            ]
                                            //                                attachedObjects: [
                                            //                                    LayoutUpdateHandler {
                                            //                                        id: statementListLayout
                                            //                                        onLayoutFrameChanged: {
                                            //
                                            //                                        }
                                            //                                    }
                                            //                                ]
                                        }
                                    }
                                    attachedObjects: [
                                        // Auto height parent ListView
                                        LayoutUpdateHandler {
                                            property int propagatedHeight: 0
                                            onLayoutFrameChanged: {
                                                if (layoutFrame.height == Number.POSITIVE_INFINITY)
                                                    return
                                                if (Qt._statementPickerPage_showTriggerOnly && ! ListItemData.containsTrigger)
                                                    return
                                                var heightToPropagate = layoutFrame.height - propagatedHeight
                                                parent.parent.parent.preferredHeight = parent.parent.parent.preferredHeight + heightToPropagate
                                                propagatedHeight = layoutFrame.height
                                            }
                                        }
                                    ]
                                }
                            }
                        ]
                        attachedObjects: [
                            SystemToast {
                                id: toastAdded
                            }
                        ]
                    }
                }

                // Feedback
                Label {
                    id: feedbackNote
                    visible: false
                    textFormat: TextFormat.Html
                    multiline: true
                    text: "<html>Looking for something else? <font style='text-decoration:underline'>Let us know</font></html>!"
                    onTouch: {
                        if (event.isDown())
                            Qt._supportSheet.createObject().open();
                    }
                }

                Space {

                }
            }
        }

    }

    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Help"
            imageSource: "asset:///images/bb10/ic_help.png"
            onTriggered: {
                var page = helpSheet.createObject()
                page.topic = "selectAction"
                page.open()
            }
        }
    ]
}