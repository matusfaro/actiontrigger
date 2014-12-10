import bb.cascades 1.2

Page {
    id: _Permissions
    DefaultPageContainer {
        topPadding: 20.0
        ListView {
            id: _listView_listPermissions
            objectName: "listPermissions"
            dataModel: GroupDataModel {
                id: _listView_dataModel_listPermissions
                sortingKeys: [ "name" ]
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
                    
                    StandardListItem {
                        id: procedureItem
                        title: ListItemData.name
                    }
                }
            ]
        }
    }
    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Enable All"
            imageSource: "asset:///images/bb10/ic_enable.png"
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Disable All"
            imageSource: "asset:///images/bb10/ic_disable.png"
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Help"
            imageSource: "asset:///images/bb10/ic_help.png"
        }
    ]
}

