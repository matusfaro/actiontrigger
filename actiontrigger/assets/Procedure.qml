import bb.cascades 1.2
import ActionTrigger 1.0
import bb.system 1.2
import bb.platform 1.2
import bb.cascades.pickers 1.0

Page {
    property bool isNew: false
    property variant tutorialSteps: [ {
            action: "all",
            text: ""
        } ]

    property int step: 0
    property string activeField: "all"
    property bool active: false
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    id: root
    function init() {
        Qt._ProcedurePage = root

        // Set externally accessible objects
        Qt.ProcedurePageActivity = activity

        // Load procedure
        pageTitle.text = Qt.ProcedurePageProcedureView.name
        procedureNameTextField.text = Qt.ProcedurePageProcedureView.name
        groupNameTextField.text = Qt.ProcedurePageProcedureView.group
        for (var i = 0; i < priorityDropdown.count(); i ++) {
            if (priorityDropdown.at(i).value == Qt.ProcedurePageProcedureView.priority) {
                priorityDropdown.setSelectedIndex(i)
                break
            }
        }
        var loglevel = Qt.ProcedurePageProcedureView.loglevel
        if (loglevel == "WARNING") {
            toggleLoglevel.checked = false
        } else {
            toggleLoglevel.checked = true
        }
        //        for (var i = 0; i < dropdownLogLevel.count(); i ++) {
        //            if (dropdownLogLevel.at(i).value == loglevel) {
        //                dropdownLogLevel.setSelectedIndex(i)
        //                break
        //            }
        //        }
        favouriteCheckbox.checked = Qt.ProcedurePageProcedureView.favourite
        autoStartCheckbox.checked = Qt.ProcedurePageProcedureView.autoStart
        runBackgroundCheckbox.checked = Qt.ProcedurePageProcedureView.backgrounded
        runForeverCheckbox.checked = ! Qt.ProcedurePageProcedureView.runOnce
        editor.init(Qt.ProcedurePageProcedureView)

        listViewRunLog.dataModel = Qt.ProcedurePageProcedureView.executionLog
        listViewRunLog.dataModel.itemAdded.connect(onItemAddedVarList)
        //        if(listViewRunLog.dataModel.childCount(0) > 0){
        onItemAddedVarList();
        //        }

        listViewVarList.dataModel = Qt.ProcedurePageProcedureView.variableList
        listViewVarList.dataModel.itemAdded.connect(onItemAddedRunLog)
        //        if(listViewVarList.dataModel.childCount(0) > 0){
        onItemAddedRunLog();
        //        }

        Qt.ProcedurePageProcedureView.notifyChangeIsRunning.connect(onIsRunningChanged);

        if (Qt.ProcedurePageProcedureView.isRunning) {
            segmentRun.selected = true
            editable.visible = false
            settings.visible = false
            viewable.visible = true
        } else {
            segmentEdit.selected = true
            viewable.visible = false
            if (isNew) {
                editable.visible = false
                segmentSettings.selected = true
                settings.visible = true
                // Set text as empty
                procedureNameTextField.text = "";
            } else {
                settings.visible = false
                segmentEdit.selected = true
                editable.visible = true
            }
        }
        root.active = true
        if (isNew) {
            procedureNameTextField.requestFocus();
        }

        activity.stop()
    }
    function onItemAddedVarList() {
        varListEmptyLabel.visible = false
    }
    function onItemAddedRunLog() {
        runLogEmptyLabel.visible = false
    }
    function resetVarListAndRunLog() {
        varListEmptyLabel.visible = true
        runLogEmptyLabel.visible = true
    }
    function firstTimeOpen() {
        // focus name field
    }
    function resizeListView(myListview, size) {
        myListview.preferredHeight = myListview.dataModel.size() * size
    }
    function onIsRunningChanged() {
        if (Qt.ProcedurePageProcedureView.isRunning) {
            active = false
            segmentRun.selected = true
            Qt._actiontrigger.processEventLoop()
            active = true
            editable.visible = false
            settings.visible = false
            viewable.visible = true
        }
    }
    DefaultPageContainer {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }

        DefaultContainer {
            DefaultActivity {
                id: activity
            }
            Label {
                id: pageTitle
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }

        // Tutorial hint
        Label {
            textStyle.color: Color.Green
            multiline: true
            visible: tutorialSteps[step].action == "editTab" || tutorialSteps[step].action == "settingsTab" || tutorialSteps[step].action == "runTab" || tutorialSteps[step].action == "logTab" || tutorialSteps[step].action == "variablesTab" || tutorialSteps[step].action == "all"
            text: tutorialSteps[step].text
        }

        SegmentedControl {
            id: segmentEditTry
            enabled: root.active
            Option {
                enabled: root.active && (tutorialSteps[step].action == "editTab" || tutorialSteps[step].action == "all")
                id: segmentEdit
                value: "EDIT"
                text: "Edit"
                onSelectedChanged: {
                    if (root.active) {
                        if (selected) {
                            // Tutorial next step
                            if (tutorialSteps[step].action == "editTab") {
                                step ++
                            }

                            if (viewable.visible)
                                Qt.ProcedurePageProcedureView.stop()
                            viewable.visible = false
                            settings.visible = false
                            editable.visible = true
                        }
                    }
                }
            }
            Option {
                enabled: root.active && (tutorialSteps[step].action == "settingsTab" || tutorialSteps[step].action == "all")
                id: segmentSettings
                value: "SETTINGS"
                text: "Settings"
                onSelectedChanged: {
                    if (root.active) {
                        if (selected) {
                            // Tutorial next step
                            if (tutorialSteps[step].action == "settingsTab") {
                                step ++
                            }

                            if (viewable.visible)
                                Qt.ProcedurePageProcedureView.stop()
                            viewable.visible = false
                            editable.visible = false
                            settings.visible = true
                        }
                    }
                }
            }
            Option {
                enabled: root.active && (tutorialSteps[step].action == "runTab" || tutorialSteps[step].action == "all")
                id: segmentRun
                value: "RUN"
                text: "Run"
                onSelectedChanged: {
                    if (root.active) {
                        if (selected) {
                            // Tutorial next step
                            if (tutorialSteps[step].action == "runTab") {
                                step ++
                            }

                            editable.visible = false
                            settings.visible = false
                            resetVarListAndRunLog()
                            Qt.ProcedurePageProcedureView.start(true)
                            //                            dropdownLogLevel.setSelectedOption(loglevelOptionDebug)
                            toggleLoglevel.checked = true
                            viewable.visible = true
                        }
                    }
                }
            }
        }

        // Tutorial hint
        Label {
            textStyle.color: Color.Green
            multiline: true
            visible: tutorialSteps[step].action == "openStatement"
            text: tutorialSteps[step].text
        }

        // Settings
        ScrollView {
            id: settings
            scrollRole: ScrollRole.Main
            visible: false
            verticalAlignment: VerticalAlignment.Fill
            Container {
                enabled: root.active

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "name"
                    text: tutorialSteps[step].text
                }

                // NAME
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Name"
                        verticalAlignment: VerticalAlignment.Center
                    }
                    TextField {
                        id: procedureNameTextField
                        // Special case: Optional constraint
                        enabled: root.active && (tutorialSteps[step].action == "name" || tutorialSteps[step].action == "all")
                        inputMode: TextFieldInputMode.Text
                        onTextChanging: {
                            if (root.active) {

                                // Tutorial next step
                                if (tutorialSteps[step].action == "name" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }

                                if (text == "") {
                                    Qt.ProcedurePageProcedureView.name = "No name"
                                    pageTitle.text = Qt.ProcedurePageProcedureView.name
                                } else {
                                    Qt.ProcedurePageProcedureView.name = procedureNameTextField.text
                                    pageTitle.text = Qt.ProcedurePageProcedureView.name
                                }
                            }
                        }
                        input {
                            flags: TextInputFlag.SpellCheck
                            submitKey: SubmitKey.None
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "group"
                    text: tutorialSteps[step].text
                }

                // GROUP
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Group"
                        verticalAlignment: VerticalAlignment.Center
                    }
                    TextField {
                        id: groupNameTextField
                        enabled: root.active && (tutorialSteps[step].action == "group" || tutorialSteps[step].action == "all")
                        inputMode: TextFieldInputMode.Text
                        onTextChanging: {
                            if (root.active) {

                                // Tutorial next step
                                if (tutorialSteps[step].action == "group" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }

                                Qt.ProcedurePageProcedureView.group = groupNameTextField.text
                            }
                        }
                        input {
                            flags: TextInputFlag.SpellCheck
                            submitKey: SubmitKey.None
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "background"
                    text: tutorialSteps[step].text
                }

                // RUN IN BACKGROUND
                DefaultContainer {
                    ////// HEADLESS DISABLED //////
                    visible: false
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Run in background"
                        preferredWidth: 9000
                    }
                    CheckBox {
                        id: runBackgroundCheckbox
                        enabled: root.active && (tutorialSteps[step].action == "background" || tutorialSteps[step].action == "all")
                        onCheckedChanged: {
                            // Tutorial next step
                            if (tutorialSteps[step].action == "background" && checked == tutorialSteps[step].constraint) {
                                step ++
                            }

                            if (root.active) {
                                Qt.ProcedurePageProcedureView.backgrounded = checked
                            }
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "loop"
                    text: tutorialSteps[step].text
                }

                // RUN FOREVER
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Loop Actions Forever"
                        preferredWidth: 9000
                    }
                    CheckBox {
                        id: runForeverCheckbox
                        enabled: root.active && (tutorialSteps[step].action == "loop" || tutorialSteps[step].action == "all")
                        onCheckedChanged: {
                            // Tutorial next step
                            if (tutorialSteps[step].action == "loop" && checked == tutorialSteps[step].constraint) {
                                step ++
                            }

                            if (root.active) {
                                Qt.ProcedurePageProcedureView.runOnce = ! checked
                            }
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "priority"
                    text: tutorialSteps[step].text
                }

                // Priority
                DefaultContainer {
                    DropDown {
                        id: priorityDropdown
                        enabled: root.active && (tutorialSteps[step].action == "priority" || tutorialSteps[step].action == "all")
                        title: "Runtime Priority"
                        Option {
                            value: "LOWEST"
                            text: "Lowest"
                        }
                        Option {
                            value: "NORMAL"
                            text: "Normal"
                        }
                        Option {
                            value: "HIGHEST"
                            text: "Time Critical"
                        }
                        onSelectedValueChanged: {
                            if (root.active) {
                                // Tutorial next step
                                if (tutorialSteps[step].action == "priority" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }

                                Qt.ProcedurePageProcedureView.priority = selectedValue
                            }
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "favourite"
                    text: tutorialSteps[step].text
                }

                // FAVOURITE
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Favourite"
                        preferredWidth: 9000
                    }
                    CheckBox {
                        id: favouriteCheckbox
                        enabled: root.active && (tutorialSteps[step].action == "favourite" || tutorialSteps[step].action == "all")
                        onCheckedChanged: {
                            if (root.active) {
                                // Tutorial next step
                                if (tutorialSteps[step].action == "favourite" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }

                                Qt.ProcedurePageProcedureView.favourite = checked
                            }
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "autostart"
                    text: tutorialSteps[step].text
                }

                // AUTO_START
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Start On App Startup"
                        preferredWidth: 9000
                    }
                    CheckBox {
                        id: autoStartCheckbox
                        enabled: root.active && (tutorialSteps[step].action == "autostart" || tutorialSteps[step].action == "all")
                        onCheckedChanged: {
                            if (root.active) {
                                // Tutorial next step
                                if (tutorialSteps[step].action == "autostart" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }

                                Qt.ProcedurePageProcedureView.autoStart = checked
                            }
                        }
                    }
                }

                // Tutorial hint
                Label {
                    textStyle.color: Color.Green
                    multiline: true
                    visible: tutorialSteps[step].action == "shortcut"
                    text: tutorialSteps[step].text
                }

                // HOMESCREEN SHORTCUT
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Home Screen Shortcut"
                        preferredWidth: 9000
                    }
                    Button {
                        id: shortcutButton
                        enabled: root.active && (tutorialSteps[step].action == "shortcut" || tutorialSteps[step].action == "all")
                        text: "Add"
                        function addShortcutWithIcon(icon) {
                            var result = myHomeScreen.addShortcut(icon, Qt.ProcedurePageProcedureView.name, "actiontrigger://" + Qt.ProcedurePageProcedureView.uuid)
                            iconResultToast.cancel()
                            if (result) {
                                iconResultToast.body = "Shortcut added";
                                shortcutDone(true);
                            } else {
                                iconResultToast.body = "Failed: Invalid image"
                                shortcutDone(false);
                            }
                            iconResultToast.show()
                        }
                        function shortcutDone(success) {
                            if (success) {
                                // Tutorial next step
                                if (tutorialSteps[step].action == "autostart" && text.match(tutorialSteps[step].constraint) == text) {
                                    step ++
                                }
                            }

                            iconResultToast.show()
                        }
                        onClicked: {
                            if (root.active) {
                                dialogSelectIcon.exec()
                                var icon
                                if (dialogSelectIcon.result == SystemUiResult.ConfirmButtonSelection) {
                                    iconPicker.open()
                                    iconResultToast.body = "Shortcut added"
                                    shortcutDone(true);
                                } else {
                                    shortcutButton.addShortcutWithIcon("asset:///logo/ProcedureIcon480x480.png");
                                }
                            }
                        }
                        attachedObjects: [
                            HomeScreen {
                                id: myHomeScreen
                            },
                            SystemToast {
                                id: iconResultToast
                            },
                            SystemDialog {
                                id: dialogSelectIcon
                                title: "Shortcut Icon"
                                body: "Do you want to select a picture for your shortcut?"
                                confirmButton.label: qsTr("Select Icon")
                                confirmButton.enabled: true
                                cancelButton.label: qsTr("Default Icon")
                                cancelButton.enabled: true
                                defaultButton: cancelButton
                            },
                            FilePicker {
                                id: iconPicker
                                title: qsTr("Icon picker")
                                mode: FilePickerMode.Picker
                                type: FileType.Picture
                                viewMode: FilePickerViewMode.Default
                                sortBy: FilePickerSortFlag.Default
                                sortOrder: FilePickerSortOrder.Default
                                onFileSelected: {
                                    shortcutButton.addShortcutWithIcon(selectedFiles[0])
                                }
                                onCanceled: {
                                    iconResultToast.cancel()
                                    iconResultToast.body = "Shortcut cancelled"
                                    iconResultToast.show()
                                }
                            }
                        ]
                    }
                }
            }
        }

        // Edit
        ScrollView {
            id: editable
            scrollRole: ScrollRole.Main
            visible: false
            verticalAlignment: VerticalAlignment.Fill
            Container {
                enabled: root.active

                // ACTIONS
                Label {
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    text: "Actions"
                }

                Indent {
                    Container { // Container for Indent
                        Editor {
                            id: editor
                        }
                    }
                }
            }
        }

        // Run
        Container {
            visible: false
            id: viewable
            verticalAlignment: VerticalAlignment.Fill

            SegmentedControl {
                id: segmentLogVar
                Option {
                    id: segmentLog
                    text: "Log"
                    onSelectedChanged: {
                        if (active) {
                            if (selected) {
                                // Tutorial next step
                                if (tutorialSteps[step].action == "logTab") {
                                    step ++
                                }

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
                                // Tutorial next step
                                if (tutorialSteps[step].action == "variablesTab") {
                                    step ++
                                }

                                runlog.visible = false
                                runvar.visible = true
                            }
                        }
                    }
                }
            }

            Container {
                id: runlog
                DefaultContainer {
                    Label {
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        text: "Run Log (toggle debug)"
                    }
                    // LogLevel
                    ToggleButton {
                        id: toggleLoglevel
                        onCheckedChanged: {
                            if (active) {
                                if (checked) {
                                    Qt.ProcedurePageProcedureView.loglevel = "TRACE"
                                } else {
                                    Qt.ProcedurePageProcedureView.loglevel = "WARNING"
                                }
                            }
                        }
                    }
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

    actions: [
        //        ActionItem {
        //            id: actionItemNewTrigger
        //            enabled: root.active
        //            ActionBar.placement: ActionBarPlacement.OnBar
        //            title: "New Trigger"
        //            imageSource: "asset:///images/bb10/ic_add.png"
        //            onTriggered: {
        //                var page = servicesPage.createObject()
        //                Qt._servicesPage_type = "triggers"
        //                Qt._pane.push(page)
        //            }
        //        },
        //        ActionItem {
        //            id: actionItemNewAction
        //            enabled: root.active
        //            ActionBar.placement: ActionBarPlacement.OnBar
        //            title: "New Action"
        //            imageSource: "asset:///images/bb10/ic_add.png"
        //            onTriggered: {
        //                var page = servicesPage.createObject()
        //                Qt._servicesPage_type = "actions"
        //                Qt._pane.push(page)
        //            }
        //        }
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Help"
            imageSource: "asset:///images/bb10/ic_help.png"
            onTriggered: {
                var page = helpSheet.createObject()
                page.topic = "procedure"
                page.open()
            }
        }
        //        DeleteActionItem {
        //            ActionBar.placement: ActionBarPlacement.InOverflow
        //            title: "Delete Procedure"
        //        }
    ]
}
