import bb.cascades 1.2
import ActionTrigger 1.0

Page {
    property alias prop_listViewDebug: _listView_debug
    function init() {
    }
    content: DefaultPageContainer {
        DefaultContainer {
            DefaultActivity {
                id: activity
                running: false
            }
            Label {
                text: "Debug log"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }

        DropDown {
            id: priorityDropdown
            title: "Level"
            Option {
                value: "NONE"
                text: "None"
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
                Qt._actiontrigger.getGlobalResources().setLogLevel(selectedValue)
            }
        }

        ListView {
            id: _listView_debug
            scrollRole: ScrollRole.Main
            stickToEdgePolicy: ListViewStickToEdgePolicy.End
            dataModel: ArrayDataModel {
                id: _listView_debug_dataModel
            }
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
    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Clear"
            imageSource: "asset:///images/bb10/ic_clear_list.png"
            onTriggered: {
                _listView_debug_dataModel.clear()
            }
        }
    ]
}