import bb.cascades 1.2
import bb.system 1.2

// OBSOLETE
Page {
    property string type
    signal selected(string service, string type)

    id: _Services
    function init() {
        Qt._servicesPage = _Services
        Qt._servicesPage_type = type
        if (Qt._servicesPage_type == "TRIGGER") {
            infoList.dataModel = Qt._actiontrigger.getTriggerInfoList();
        } else {
            infoList.dataModel = Qt._actiontrigger.getActionInfoList();
        }
        infoList.dataModel.resetFilter()
        servicesPageTitle.text = "Select " + (Qt._servicesPage_type == "TRIGGER" ? "Trigger" : "Action")
        textFieldFilter.hintText = "Filter " + (Qt._servicesPage_type == "TRIGGER" ? "TRIGGER" : "ACTION")
        activity.stop()
    }
    DefaultPageContainer {
        topPadding: 20.0
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
        // Filter
        TextField {
            id: textFieldFilter
            onTextChanging: {
                activity.start()
                infoList.dataModel.setFilter(text)
                activity.stop()
            }
        }
        ListView {
            id: infoList
            objectName: "listServices"
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    NonStandardListItem {
                        id: root
                        title: ListItemData.name + ((Qt._servicesPage_type == "ACTION" && ListItemData.isTrigger) ? " (Trigger)" : "")
                        description: ListItemData.description
                        selectImage: "asset:///images/bb10/ic_add.png"
                        select.onTapped: {
                            root.ListItem.view.getActivity().start()

                            // Send signal
                            Qt._servicesPage.selected(ListItemData.serviceType, ListItemData.type)

                            // Handle received signals
                            Qt._actiontrigger.processEventLoop

                            root.ListItem.view.getActivity().stop()
                            root.ListItem.view.getToast().cancel()
                            root.ListItem.view.getToast().body = ListItemData.name + (Qt._servicesPage_type == "TRIGGER" ? " trigger added" : " action added")
                            root.ListItem.view.getToast().show()
                        }
                    }
                }
            ]
            function getToast() {
                return toastAdded
            }
            function getActivity() {
                return activity
            }
            attachedObjects: [
                SystemToast {
                    id: toastAdded
                }
            ]
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