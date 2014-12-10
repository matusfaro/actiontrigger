import bb.cascades 1.2
import ActionTrigger 1.0
import bb.system 1.2

Container {
    id: root
    property Procedure procedure
    function init(model) {
        procedure = model
        console.log("editor:init")

        clipboardChanged()
        Qt._actiontrigger.getGlobalResources().clipboardChanged.connect(clipboardChanged)

        var actions = procedure.getStatements()
        for (var i = 0; i < actions.length; ++ i) {
            appendChild(actions[i])
        }
    }
    // Create child statement and append at the end
    function createChild(type) {
        appendChild(procedure.newAction(type))
    }
    // Paste child statement and append at the end
    function pasteChild() {
        appendChild(procedure.pasteAction())
    }
    // Append statement at the end
    function appendChild(statement) {
        rootContainer.visible = true
        var itemObject = item.createObject(0)
        itemObject.init(statement, rootContainer)
        rootContainer.add(itemObject)
    }
    function clipboardChanged() {
        procedurePasteButton.visible = pasteAvailable()
    }
    function pasteAvailable() {
        if (Qt._actiontrigger.getGlobalResources().clipboardHasStatement() == false) {
            return false
        }
        return true
    }

    // Tutorial hint
    Label {
        textStyle.color: Color.Green
        multiline: true
        visible: tutorialSteps[step].action == "addActionToBranch"
        text: tutorialSteps[step].pretext
    }

    Container {
        id: rootContainer
    }

    // Tutorial hint
    Label {
        textStyle.color: Color.Green
        multiline: true
        visible: tutorialSteps[step].action == "addActionAtEnd"
        text: tutorialSteps[step].pretext
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        ImageButton {
            defaultImageSource: "asset:///images/bb10/ic_add.png"
            onClicked: {
                if (! Qt._premium.askPurchase()) {
                    return
                }

                var currAction = tutorialSteps[step].action

                if (currAction != "all" && currAction != "addActionAtEnd") {
                    allPurposeToast.cancel()
                    allPurposeToast.body = "That's not correct!"
                    allPurposeToast.show()
                    return
                }

                // Add new action to procedure
                var page = statementPickerPage.createObject()

                if (currAction == "addActionAtEnd") {
                    // Don't rollback
                    page.stepRollback = -1
                }

                Qt._statementPickerPage_showTriggerOnly = false // Root actions under procedure are actions
                page.selected.connect(createChild);
                Qt._pane.push(page)
            }
        }
        ImageButton {
            id: procedurePasteButton
            visible: false
            defaultImageSource: "asset:///images/bb10/ic_paste.png"
            onClicked: {
                if (! Qt._premium.askPurchase()) {
                    return
                }
                if (! pasteAvailable()) {
                    console.log("Attempted to paste when clipboard is empty")
                    return
                }
                // Paste action to procedure
                Qt.ProcedurePageActivity.start()
                root.pasteChild()
                Qt.ProcedurePageActivity.stop()
            }
        }
    }

    attachedObjects: [
        // Item
        ComponentDefinition {
            id: item
            Container {
                id: itemContainer
                property StatementView statement
                property Container parentContainer
                function init(model, parent) {
                    statement = model
                    statement.notifyChangeIsFirst.connect(refreshMoveButtons);
                    statement.notifyChangeIsLast.connect(refreshMoveButtons);
                    refreshMoveButtons();
                    parentContainer = parent
                    infoView.title = statement.name
                    //                    infoView.description = statement.description
                    var branches = statement.branches
                    for (var i = 0; i < branches.length; ++ i) {
                        // Create and init branch
                        branchesContainer.visible = true
                        var branchObject = branch.createObject(0)
                        branchObject.init(statement, branches[i], statement.getBranch(branches[i].key, branches[i].type))
                        branchesRootContent.add(branchObject)
                    }
                }
                function refreshMoveButtons() {
                    actionItemMoveUp.enabled = ! statement.isFirst
                    actionItemMoveDown.enabled = ! statement.isLast
                }
                function removeItem(item) {
                    var index = findIndex(item)
                    if (index == -1) {
                        console.log("ERROR: Editor.qml: Could not find item to remove")
                        return
                    }
                    var obj = parentContainer.at(index)
                    parentContainer.remove(obj)
                    obj.destroy()
                }
                function moveUp(item) {
                    var index = findIndex(item)
                    if (index == -1 || parentContainer.count() <= index) {
                        console.log("ERROR: Editor.qml: Could not moveDown, container size ", parentContainer.count(), ", item index1 ", index, ", item index2 ", index - 1)
                        return
                    }
                    console.log("Swapping indexes: ", index - 1, index)
                    parentContainer.swap(index - 1, index)
                }
                function moveDown(item) {
                    var index = findIndex(item)
                    // Ensure item was found and is not outside of boundaries
                    if (index == -1 || parentContainer.count() <= index + 1) {
                        console.log("ERROR: Editor.qml: Could not moveDown, container size ", parentContainer.count(), ", item index1 ", index, ", item index2 ", index + 1)
                        return
                    }
                    console.log("Swapping indexes: ", index, index + 1)
                    parentContainer.swap(index, index + 1)
                }
                // Finds index of statement container from statement object
                function findIndex(item) {
                    console.log("Current parent container count: ", parentContainer.count())
                    for (var index = 0; index < parentContainer.count(); ++ index) {
                        if (parentContainer.at(index).statement.id == item.id) {
                            return index
                        }
                    }
                    console.log("ERROR: Editor.qml: Could not find item in container: ", item.id, item.name)
                    return -1
                }
                NonStandardListItem {
                    id: infoView
                    showDivider: true
                    padding: 0
                    expandDescOnTap: true
                    title: "Unknown"
                    description: ""
                    tap.onTapped: {
                        // Open statement
                        if (! Qt._premium.askPurchase()) {
                            return
                        }

                        var currAction = tutorialSteps[step].action

                        if (currAction != "all" && currAction != "openStatement") {
                            allPurposeToast.cancel()
                            allPurposeToast.body = "That's not correct!"
                            allPurposeToast.show()
                            return
                        } else if (currAction == "openStatement" && statement.type.match(tutorialSteps[step].constraint) != statement.type) {
                            allPurposeToast.cancel()
                            allPurposeToast.body = "That's the wrong one!"
                            allPurposeToast.show()
                            return
                        }

                        // Add new action to procedure
                        var page = statementPage.createObject()

                        if (currAction != "all") {
                            page.stepRollback = step
                            step ++
                        }

                        page.statement = itemContainer.statement
                        Qt._pane.push(page)
                    }
                    // Statement context actions
                    contextActions: [
                        ActionSet {
                            actions: [
                                ActionItem {
                                    id: actionItemMoveUp
                                    enabled: ! itemContainer.isFirst
                                    title: "Move up"
                                    imageSource: "asset:///images/custom/ic_move_up.png"
                                    onTriggered: {
                                        if (! Qt._premium.askPurchase()) {
                                            return
                                        }
                                        Qt.ProcedurePageActivity.start()
                                        if (itemContainer.statement.moveUp() == true) {
                                            // Move was successful, let's move it visually as well
                                            itemContainer.moveUp(itemContainer.statement)
                                        }
                                        Qt.ProcedurePageActivity.stop()
                                    }
                                },
                                ActionItem {
                                    id: actionItemMoveDown
                                    enabled: ! itemContainer.isLast
                                    title: "Move down"
                                    imageSource: "asset:///images/custom/ic_move_down.png"
                                    onTriggered: {
                                        if (! Qt._premium.askPurchase()) {
                                            return
                                        }
                                        Qt.ProcedurePageActivity.start()
                                        if (itemContainer.statement.moveDown()) {
                                            // Move was successful, let's move it visually as well
                                            itemContainer.moveDown(itemContainer.statement)
                                        }
                                        Qt.ProcedurePageActivity.stop()
                                    }
                                },
                                ActionItem {
                                    title: "Copy"
                                    imageSource: "asset:///images/bb10/ic_copy.png"
                                    onTriggered: {
                                        if (! Qt._premium.askPurchase()) {
                                            return
                                        }
                                        Qt.ProcedurePageActivity.start()
                                        itemContainer.statement.copy()
                                        Qt.ProcedurePageActivity.stop()
                                    }
                                },
                                ActionItem {
                                    title: "Cut"
                                    imageSource: "asset:///images/bb10/ic_cut.png"
                                    onTriggered: {
                                        if (! Qt._premium.askPurchase()) {
                                            return
                                        }
                                        Qt.ProcedurePageActivity.start()
                                        itemContainer.statement.copy()
                                        itemContainer.removeItem(itemContainer.statement)
                                        itemContainer.statement.remove()
                                        Qt.ProcedurePageActivity.stop()
                                    }
                                },
                                DeleteActionItem {
                                    attachedObjects: [
                                        SystemToast {
                                            id: deleteToastAction
                                            body: qsTr("Deleted") + Retranslate.onLanguageChanged
                                        },
                                        SystemDialog {
                                            id: deleteDialogAction
                                            // TODO fix bug - if multiple items deleted after each other, this variable may not be set to the correct selected item. (Is this still a problem?)
                                            property variant selectedItemAction
                                            title: qsTr("Delete Item") + Retranslate.onLanguageChanged
                                            body: qsTr("This action will delete this item.  This action cannot be un-done.") + Retranslate.onLanguageChanged
                                        }
                                    ]
                                    onTriggered: {
                                        if (! Qt._premium.askPurchase()) {
                                            return
                                        }
                                        Qt.ProcedurePageActivity.start()
                                        deleteDialogAction.exec()
                                        if (deleteDialogAction.result == SystemUiResult.ConfirmButtonSelection) {
                                            itemContainer.removeItem(itemContainer.statement)
                                            itemContainer.statement.remove()
                                            deleteToastAction.show()
                                        }
                                        Qt.ProcedurePageActivity.stop()
                                    }
                                }
                            ]
                        } // end of ActionSet
                    ]
                }

                // Branches
                Container {
                    id: branchesContainer
                    visible: false
                    leftMargin: 30
                    Container {
                        id: branchesRootContent
                    }
                    Divider {
                    }
                }
                //                HorizontalLine {
                //                    id: branchesContainer
                //                    visible: false
                //                    leftMargin: 30
                //                    lineopacity: 0
                //                    Container {
                //                        id: branchesRootContent
                //                    }
                //                }
                //                Divider {
                //                }
            }
        },
        // Branch
        ComponentDefinition {
            id: branch
            Container {
                id: branchInstance
                property StatementView parentStatement
                property Branch branchInfo
                function init(statement, info, statements) {
                    parentStatement = statement
                    branchInfo = info

                    clipboardChanged()
                    Qt._actiontrigger.getGlobalResources().clipboardChanged.connect(clipboardChanged)

                    // TODO Display info.description
                    labelDescription.text = "  " + branchInfo.name
                    for (var i = 0; i < statements.length; ++ i) {
                        appendChild(statements[i])
                    }

                    refreshTitlePosition()
                }
                // Create child statement and append at the end
                function createChild(type) {
                    appendChild(parentStatement.newBranchChild(branchInfo.key, type))
                }
                // Paste child statement and append at the end
                function pasteChild() {
                    appendChild(parentStatement.pasteBranchChild(branchInfo.key))
                }
                // Append statement at the end
                function appendChild(statement) {
                    branchRootContent.visible = true
                    var itemObject = item.createObject(0)
                    itemObject.init(statement, branchRootContent)
                    branchRootContent.add(itemObject)
                }
                function refreshTitlePosition() {
                    labelDescription.preferredWidth = (1 + branchRootContent.controls.length) * 60 + 30
                    labelDescription.pivotX = - labelDescription.preferredWidth / 2 + 30
                }
                function clipboardChanged() {
                    pasteButton.visible = branchInstance.pasteAvailable()
                }
                function pasteAvailable() {
                    console.log("pasteAvailable: ", branchInfo.type, true, Qt._actiontrigger.getGlobalResources().clipboardHasStatement(), Qt._actiontrigger.getGlobalResources().clipboardHasTrigger())
                    if (Qt._actiontrigger.getGlobalResources().clipboardHasStatement() == false) {
                        console.log("empty clipboard")
                        return false
                    } else if (branchInfo.type == "TRIGGER" && Qt._actiontrigger.getGlobalResources().clipboardHasTrigger() == false) {
                        console.log("clipboard doesn't contain a trigger")
                        return false
                    }
                    console.log("clipboard is available")
                    return true
                }
                layout: DockLayout {
                }
                // Branch title
                Label {
                    id: labelDescription
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    visible: false
                    onTextChanged: {
                        visible = text.length != 0
                    }

                    // Magic
                    preferredHeight: 60
                    rotationZ: 90
                }
                // Branches
                Indent {
                    size: 60
                    Container { // Container for Indent
                        Container {
                            id: branchRootContent
                            visible: false
                            onControlAdded: {
                                branchInstance.refreshTitlePosition()
                            }
                            onControlRemoved: {
                                branchInstance.refreshTitlePosition()
                            }
                        }
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            ImageButton {
                                defaultImageSource: "asset:///images/bb10/ic_add.png"
                                onClicked: {
                                    if (! Qt._premium.askPurchase()) {
                                        return
                                    }
                                    
                                    var currAction = tutorialSteps[step].action

                                    switch (currAction) {
                                        case "all":
                                            break
                                        case "addActionToBranch":
                                            if (branchInstance.parentStatement.type.match(tutorialSteps[step].statementType) != branchInstance.parentStatement.type || branchInstance.branchInfo.key.match(tutorialSteps[step].branchKey) != branchInstance.branchInfo.key) {
                                                allPurposeToast.cancel()
                                                allPurposeToast.body = "That's not correct!"
                                                allPurposeToast.show()
                                                console.log("Requirement not met: ", branchInstance.parentStatement.type, branchInstance.branchInfo.key)
                                                return
                                            }
                                            break
                                        default:
                                            allPurposeToast.cancel()
                                            allPurposeToast.body = "That's not correct!"
                                            allPurposeToast.show()
                                            return
                                    }

                                    // Add new statement to branch
                                    var page = statementPickerPage.createObject()

                                    if (currAction == "addActionToBranch") {
                                        // Don't rollback
                                        page.stepRollback = -1
                                    }

                                    // Check whether to show all actions or triggers only
                                    Qt._statementPickerPage_showTriggerOnly = branchInstance.branchInfo.type == "TRIGGER"
                                    page.selected.connect(branchInstance.createChild);
                                    Qt._pane.push(page)
                                }
                            }
                            ImageButton {
                                id: pasteButton
                                visible: false
                                defaultImageSource: "asset:///images/bb10/ic_paste.png"
                                onClicked: {
                                    if (! Qt._premium.askPurchase()) {
                                        return
                                    }
                                    if (! pasteAvailable()) {
                                        console.log("Attempted to paste when clipboard is empty")
                                        return
                                    }
                                    Qt.ProcedurePageActivity.start()
                                    Qt._actiontrigger.processEventLoop()
                                    branchInstance.pasteChild()
                                    Qt.ProcedurePageActivity.stop()
                                }
                            }
                        }
                    }
                }
            }
        },
        ComponentDefinition {
            id: statementPickerPage
            source: "asset:///StatementPicker.qml"
        },
        ComponentDefinition {
            id: statementPage
            source: "asset:///Statement.qml"
        }
    ]
}
