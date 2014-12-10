import bb.cascades 1.2
import bb.system 1.2

import ActionTrigger 1.0

Page {
    id: pageSelectVariables
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    signal variableSelected(string value)
    function init() {
        Qt._pageSelectVariables = pageSelectVariables
        var variables = Qt.ProcedurePageProcedureView.getExposedVariableDefinitions()
        console.log("Populating model")
        for (var i = 0; i < variables.length; ++ i) {
            console.log(i)
            model.insert(variables[i])
        }
        model.resetFilter()
        activity.stop()
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
                text: "Select variable"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }
        // PARAMETER DESCRIPTION
        Label {
            multiline: true
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
            text: "Pick a variable that will be replaced by the value during runtime"
        }
        // Filter
        TextField {
            hintText: "Filter variables"
            onTextChanging: {
                activity.start()
                _listView_variables.dataModel.setFilter(text)
                activity.stop()
            }
        }
        ListView {
            id: _listView_variables
            scrollRole: ScrollRole.Main
            dataModel: RegexDataModel {
                id: model
                //                sortedAscending: true
                grouping: ItemGrouping.ByFullValue
                sortingKeys: [ 'statementName', 'name' ]
            }
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    NonStandardListItem {
                        id: root

                        // Filter
                        property string filter: root.ListItem.view.filter
                        onFilterChanged: {
                            maxHeight = Qt._actiontrigger.filter(root.title + root.description, filter) ? 1000 : 1
                        }

                        title: ListItemData.name
                        description: ListItemData.description
                        selectImage: "asset:///images/bb10/ic_add.png"
                        select.onTapped: {
                            root.ListItem.view.getActivity().start()
                            Qt._pageSelectVariables.variableSelected("{" + ListItemData.name + "}")
                            root.ListItem.view.getActivity().stop()
                            root.ListItem.view.getToast().cancel()
                            root.ListItem.view.getToast().body = ListItemData.name + " added"
                            root.ListItem.view.getToast().show()
                        }
                    }
                }
            ]
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2.5
            }
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
                page.topic = "selectVariable"
                page.open()
            }
        }
    ]
}
