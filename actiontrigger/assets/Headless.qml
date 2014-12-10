import bb.cascades 1.2
import ActionTrigger 1.0

Page {
    property alias prop_listViewHeadless: _listView_headless
    function init() {
        Qt._headlessConnector = Qt._actiontrigger.getGlobalResources().getHeadlessConnector()
        Qt._headlessConnector.notifyChangeIsConnected.connect(handleStateChange)
        handleStateChange()
    }
    function handleStateChange() {
        console.log("isConnected:", Qt._headlessConnector.isConnected)
        if (Qt._headlessConnector.isConnected) {
            headlessStatusLabel.text = "Connected and running in background"
            stopButton.enabled = true;
        } else {
            headlessStatusLabel.text = "Not connected"
//            stopButton.enabled = false;
        }
    }
    content: DefaultPageContainer {
        DefaultContainer {
            DefaultActivity {
                id: activity
                running: false
            }
            Label {
                text: "Headless Monitor"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }

        // Runtime info
        Label {
            id: headlessStatusLabel
            multiline: true
            textStyle.fontStyle: FontStyle.Italic
            horizontalAlignment: HorizontalAlignment.Center
            text: "Starting..."
        }

        Divider {
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {

                orientation: LayoutOrientation.LeftToRight
            }
            Button {
                id: startButton
//                visible: false
                enabled: true
                text: "Start (Debug only)"
                onClicked: {
                    headlessStatusLabel.text = "Not connected"
                    Qt._headlessConnector.start()
                    handleStateChange()
                }
            }
            Button {
                id: stopButton
                enabled: true
                text: "Force Stop"
                onClicked: {
                    headlessStatusLabel.text = "Shutting down..."
                    Qt._headlessConnector.kill()
                    handleStateChange()
                }
            }
        }

        Divider {
        }

        ListView {
            id: _listView_headless
            scrollRole: ScrollRole.Main
            objectName: "listHeadless"
            // DEBUG ONLY
            //                                    dataModel: XmlDataModel {
            //                                        id: _listView_dataModel_listLibrary
            //                                        source: "model.xml"
            //                                    }
            onTriggered: {
                // Open Procedure page
                var item = dataModel.data(indexPath)
                item.flag = "" // Clears flag, ie "DUPLICATE", "IMPORTED"
                var page = Qt._procedurePage.createObject()
                Qt.ProcedurePageProcedureView = item
                Qt._pane.push(page);
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
                    Container {
                        opacity: ListItemData.isRunning ? 1.0 : 0.3
                        id: procedureItem

                        DefaultContainer {
                            preferredWidth: Infinity
                            horizontalAlignment: HorizontalAlignment.Left
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }

                            ImageView {
                                visible: ListItemData.favourite
                                maxWidth: 50
                                maxHeight: maxWidth
                                verticalAlignment: VerticalAlignment.Center
                                imageSource: "asset:///images/bb10/ic_favorite.png"
                            }

                            Label {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1
                                }
                                multiline: true
                                verticalAlignment: VerticalAlignment.Center
                                id: buttonID
                                text: ListItemData.name
                            }

                            Label {
                                visible: false
                                text: ListItemData.isRunning ? (ListItemData.backgrounded ? "Running in background" : "Running"): (ListItemData.backgrounded ? "Stopped in background" : "Stopped") 
                                textStyle.fontSize: FontSize.XSmall
                                textStyle.fontStyle: FontStyle.Italic
                                verticalAlignment: VerticalAlignment.Center
                                onTextChanged: {
                                    visible = text != ""
                                }
                            }

                            ImageView {
                                horizontalAlignment: HorizontalAlignment.Right
                                verticalAlignment: VerticalAlignment.Center
                                imageSource: "asset:///images/bb10/ic_next.png"
                            }
                        }
                    }
                }
            ]
        }

    }
}