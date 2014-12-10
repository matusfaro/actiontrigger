import bb.cascades 1.2
import bb.system 1.2
import QtQuick 1.0

Page {

    property alias prop_listViewOverview: _listViewOverview
    property alias prop_showAll: _listViewOverview.showAll
    property alias prop_viewToggle: viewToggle
    id: _OverviewTab
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    onCreationCompleted: {
        console.log("overviewtab.qml onCreationCompleted")
    }
    function init() {
        dashboardPageTitle.text = "ActionTrigger"
        activity.stop()
    }
    content: DefaultPageContainer {
        DefaultContainer {
            DefaultActivity {
                id: activity
            }
            Label {
                id: dashboardPageTitle
                textStyle.base: SystemDefaults.TextStyles.BigText
                text: "asdsa"
            }
        }
        ListView {
            id: _listViewOverview
            property bool showAll: true
            scrollRole: ScrollRole.Main
            objectName: "listOverview"
            //                        dataModel: XmlDataModel {
            //                            source: "model.xml"
            //                        }
            layout: FlowListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            function getActivity() {
                return activity
            }
            scrollIndicatorMode: ScrollIndicatorMode.ProportionalBar
            listItemComponents: [
                ListItemComponent {
                    id: listItemComponent
                    type: "item"
                    Container {
                        id: overviewContainer
                        topPadding: 30
                        bottomPadding: 0
                        leftPadding: 30
                        rightPadding: leftPadding
                        minWidth: 354
                        minHeight: 354
                        visible: visibilityCache
                        horizontalAlignment: HorizontalAlignment.Center
                        property bool visibilityCache: overviewContainer.ListItem.view.showAll || ListItemData.favourite
                        onVisibilityCacheChanged: {
                            // Due to ListView bug, need to redraw listview for invisible containers to become visible.
                            overviewContainer.ListItem.view.visible = false
                            overviewContainer.ListItem.view.visible = true
                        }
                        layout: DockLayout {
                        }
                        attachedObjects: [
                            Timer {
                                id: isPressedTimer
                                interval: 200
                                running: false
                                repeat: false
                                onTriggered: {
                                    isPressedTimer.stop()
                                    block.isPressed = false
                                }
                            }
                        ]

                        // Background color
                        Container {
                            id: block
                            property bool isPressed: false
                            property bool isRunningCache: ListItemData.isRunning
                            property bool backgroundedCache: ListItemData.backgrounded
                            property bool isHoldingCache: overviewContainer.ListItem.active || overviewContainer.ListItem.selected
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            background: isPressed ? Color.DarkGreen : (isRunningCache ? (backgroundedCache ? Color.DarkGreen : Color.Green) : Color.Gray)
                            opacity: isRunningCache || isPressed ? 0.5 : 0.2
                            preferredWidth: isPressed ? 260 : 294
                            preferredHeight: isPressed ? 260 : 294
                        } // Border

                        // Flag
                        Label {
                            visible: false
                            text: ListItemData.flag
                            textStyle.fontSize: FontSize.XSmall
                            textStyle.fontStyle: FontStyle.Italic

                            // Positioning
                            minHeight: 64
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Bottom

                            onTextChanged: {
                                visible = text != ""
                            }
                        }

                        // Favourite star
                        Container {
                            visible: ListItemData.favourite
                            preferredWidth: 84
                            preferredHeight: preferredWidth
                            rightPadding: 34
                            bottomPadding: 34
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Bottom
                            ImageView {
                                visible: ListItemData.favourite
                                maxWidth: 50
                                maxHeight: maxWidth
                                verticalAlignment: VerticalAlignment.Center
                                imageSource: "asset:///images/bb10/ic_favorite.png"
                            }
                        }

                        // Content
                        Container {
                            id: itemRoot
                            topPadding: 30
                            bottomPadding: 0
                            leftPadding: 30
                            rightPadding: leftPadding
                            preferredWidth: block.preferredWidth
                            preferredHeight: block.preferredHeight

                            // Item highlight
                            Label {
                                multiline: true
                                text: ListItemData.name
                            }

                            // Log view - Not used anymore
                            //                            ListView {
                            //                                id: listViewLog
                            //                                visible: overviewContainer.ListItem.active || overviewContainer.ListItem.selected
                            //                                stickToEdgePolicy: ListViewStickToEdgePolicy.End
                            //                                preferredHeight: block.preferredHeight
                            //                                preferredWidth: block.preferredWidth
                            //                                dataModel: ListItemData.executionLog
                            //                                listItemComponents: [
                            //                                    ListItemComponent {
                            //                                        Label {
                            //                                            textStyle.fontSize: FontSize.XSmall
                            //                                            multiline: true
                            //                                            text: ListItemData
                            //                                        }
                            //                                    }
                            //                                ]
                            //                            }

                            gestureHandlers: [
                                TapHandler {
                                    onTapped: {
                                        // Is this check necessary??
                                        //if (! overviewContainer.ListItem.active && ! overviewContainer.ListItem.selected) {

                                        // Show button press animation
                                        block.isPressed = true
                                        isPressedTimer.start()

                                        // Start or stop procedure
                                        if (ListItemData.isRunning) {
                                            ListItemData.stop()
                                        } else {
                                            ListItemData.start()
                                        }
                                    }
                                }
                            ]
                        } // Container
                        contextActions: [
                            ActionSet {
                                actions: [
                                    ActionItem {
                                        title: "Open"
                                        imageSource: "asset:///images/bb10/ic_open.png"
                                        onTriggered: {
                                            // Open Procedure page
                                            var item = Qt._overviewInstance_listView_listLibrary.dataModel.data(overviewContainer.ListItem.view.selected())
                                            item.flag = "" // Clears flag, ie "DUPLICATE", "IMPORTED"
                                            var page = Qt._procedurePage.createObject()
                                            Qt.ProcedurePageProcedureView = item
                                            Qt._pane.push(page);
                                        }
                                    },
                                    ActionItem {
                                        title: "Toggle Favourite"
                                        imageSource: "asset:///images/bb10/ic_favorite.png"
                                        onTriggered: {
                                            var model = Qt._overviewInstance_listView_listLibrary.dataModel.data(overviewContainer.ListItem.view.selected())
                                            var newFavouriteVal = ! model.favourite
                                            model.favourite = newFavouriteVal
                                            favouritedToast.body = newFavouriteVal ? "Favourited" : "Unfavourited"
                                            favouritedToast.show()
                                        }
                                        attachedObjects: [
                                            SystemToast {
                                                id: favouritedToast
                                            }
                                        ]
                                    },
                                    DeleteActionItem {
                                        attachedObjects: [
                                            SystemToast {
                                                id: deleteToast
                                                body: qsTr("Deleted") + Retranslate.onLanguageChanged
                                            },
                                            SystemDialog {
                                                id: deleteDialog
                                                property variant selectedItem
                                                title: qsTr("Delete Item") + Retranslate.onLanguageChanged
                                                body: qsTr("This action will delete the selected item. This action cannot be reversed.") + Retranslate.onLanguageChanged
                                                onFinished: {
                                                    if (selectedItem != null && deleteDialog.result == SystemUiResult.ConfirmButtonSelection) {
                                                        Qt._overviewInstance_listView_listLibrary.dataModel.data(selectedItem).remove()
                                                        Qt._overviewInstance_listView_listLibrary.dataModel.removeAt(selectedItem)
                                                        deleteToast.show()
                                                    }
                                                    selectedItem = null;
                                                    overviewContainer.ListItem.view.getActivity().stop()
                                                }
                                            }
                                        ]
                                        onTriggered: {
                                            overviewContainer.ListItem.view.getActivity().start()
                                            deleteDialog.selectedItem = overviewContainer.ListItem.view.selected()
                                            deleteDialog.show()
                                        }
                                    }
                                ]
                            } // end of ActionSet
                        ]
                    } // Container
                } // ListItemComponent
            ]
        }
    } // end of Container

    actions: [
        ActionItem {
            id: viewToggle
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Favourites"
            imageSource: "asset:///images/bb10/ic_favorite.png"
            onTriggered: {
                _listViewOverview.showAll = ! _listViewOverview.showAll
                if (_listViewOverview.showAll) {
                    _listViewOverview.dataModel.sortingKeys = [ "group" ]
                    imageSource = "asset:///images/bb10/ic_favorite.png"
                    title = "Favourites"
                } else {
                    _listViewOverview.dataModel.sortingKeys = []
                    imageSource = "asset:///images/bb10/ic_all.png"
                    title = "All"
                }
            }
        }
        //    	ActionItem {
        //            ActionBar.placement: ActionBarPlacement.OnBar
        //            title: "Start All"
        //            imageSource: "asset:///images/bb10/ic_play.png"
        //        },
        //        ActionItem {
        //            ActionBar.placement: ActionBarPlacement.OnBar
        //            title: "Stop All"
        //            imageSource: "asset:///images/bb10/ic_stop.png"
        //        },
        //        ActionItem {
        //            ActionBar.placement: ActionBarPlacement.OnBar
        //            title: "Help"
        //            imageSource: "asset:///images/bb10/ic_help.png"
        //        }
    ]
}
