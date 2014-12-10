import bb.cascades 1.2
import bb.system 1.2
import bb.cascades.pickers 1.0

import ActionTrigger 1.0

Page {
    property alias prop_listView_dataModel_listLibrary: _listView_dataModel_listLibrary
    property alias prop_listView_listLibrary: _listView_listLibrary
    property alias prop_filterTextField: filterTextField
    id: _MyLibrary
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    DefaultPageContainer {
        DefaultContainer {
            DefaultActivity {
                id: activity
                running: false
            }
            Label {
                text: "Library"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }

        // Filter
        TextField {
            id: filterTextField
            hintText: "Filter procedures"
            onTextChanging: {
                activity.start()
                _listView_listLibrary.dataModel.setFilter(text)
                activity.stop()
            }
        }

        ListView {
            id: _listView_listLibrary
            scrollRole: ScrollRole.Main
            objectName: "listLibrary"
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            dataModel: RegexDataModel {
                id: _listView_dataModel_listLibrary
                grouping: ItemGrouping.ByFullValue
                sortingKeys: [ "group" ]
            }
            // DEBUG ONLY
            //                        dataModel: XmlDataModel {
            //                            id: _listView_dataModel_listLibrary
            //                            source: "model.xml"
            //                        }
            onTriggered: {
                // Open Procedure page
                var item = dataModel.data(indexPath)
                item.flag = "" // Clears flag, ie "DUPLICATE", "IMPORTED"
                var page = Qt._procedurePage.createObject()
                Qt.ProcedurePageProcedureView = item
                Qt._pane.push(page);
            }

            multiSelectAction: MultiSelectActionItem {
            }
            function getActivity() {
                return activity
            }
            multiSelectHandler {
                status: qsTr("None selected") + Retranslate.onLanguageChanged

                // The actions that can be performed in a multi-select sessions are set up in the actions list.
                actions: [
                    ActionItem {
                        title: "Toggle Favourite"
                        imageSource: "asset:///images/bb10/ic_favorite.png"
                        onTriggered: {
                            var selectedItems = _listView_listLibrary.selectionList()
                            if (selectedItems.length > 0) {
                                var newFavouriteVal = ! _listView_listLibrary.dataModel.data(selectedItems[0]).favourite
                                for (var i = selectedItems.length - 1; i >= 0; i --) {
                                    _listView_listLibrary.dataModel.data(selectedItems[i]).favourite = newFavouriteVal
                                }
                                favouritedSelectedToast.body = newFavouriteVal ? "Favourited" : "Unfavourited"
                                favouritedSelectedToast.show()
                            }
                        }
                        attachedObjects: [
                            SystemToast {
                                id: favouritedSelectedToast
                            }
                        ]
                    },
                    ActionItem {
                        ActionBar.placement: ActionBarPlacement.InOverflow
                        title: "Export Selected"
                        imageSource: "asset:///images/bb10/ic_entry.png"
                        onTriggered: {
                            exportFilePicker.selectedItems = _listView_listLibrary.selectionList()
                            exportFilePicker.open()
                            activity.start()
                        }
                        attachedObjects: [
                            FilePicker {
                                id: exportFilePicker
                                property variant selectedItems
                                title: qsTr("Export")
                                mode: FilePickerMode.Saver
                                filter: [ "*.at" ]
                                onFileSelected: {
                                    var itemsList = new Array()
                                    for (var i = selectedItems.length - 1; i >= 0; i --) {
                                        itemsList.push(_listView_listLibrary.dataModel.data(selectedItems[i]))
                                    }
                                    exportedToast.body = Qt._actiontrigger.exportData(selectedFiles + ".at", itemsList);
                                    activity.stop()
                                    exportedToast.cancel()
                                    exportedToast.show()
                                }
                                onCanceled: {
                                    activity.stop()
                                    exportedToast.cancel()
                                    exportedToast.body = "Cancelled"
                                    exportedToast.show()
                                }
                            },
                            SystemToast {
                                id: exportedToast
                            }
                        ]
                    },
                    DeleteActionItem {
                        title: qsTr("Delete") + Retranslate.onLanguageChanged
                        onTriggered: {
                            if (! Qt._premium.askPurchase()) {
                                return
                            }
                            if (_listView_listLibrary.selectionList().length > 0) {
                                activity.start()
                                deleteDialogMultiple.selectedItems = _listView_listLibrary.selectionList()
                                deleteDialogMultiple.show()
                            }
                        }
                    }
                ]
            }
            onSelectionChanged: {
                // The status text of the multi-select handler is updated to show how
                // many items are currently selected.
                if (selectionList().length > 1) {
                    multiSelectHandler.status = selectionList().length + qsTr(" items selected");
                } else if (selectionList().length == 1) {
                    multiSelectHandler.status = qsTr("1 item selected");
                } else {
                    multiSelectHandler.status = qsTr("None selected");
                }
            }
            attachedObjects: [
                SystemToast {
                    id: deleteToastMultiple
                },
                SystemDialog {
                    id: deleteDialogMultiple
                    property variant selectedItems
                    title: qsTr("Delete Item") + Retranslate.onLanguageChanged
                    body: qsTr("This action will delete the selected item(s).  This action cannot be un-done.") + Retranslate.onLanguageChanged
                    onFinished: {
                        if (deleteDialogMultiple.result == SystemUiResult.ConfirmButtonSelection) {
                            deleteToastMultiple.body = "Deleted all";
                            for (var i = selectedItems.length - 1; i >= 0; i --) {
                                if (! _listView_listLibrary.dataModel.data(selectedItems[i]).isRunning) {
                                    _listView_listLibrary.dataModel.data(selectedItems[i]).remove();
                                    _listView_listLibrary.dataModel.removeAt(selectedItems[i]);
                                } else {
                                    deleteToastMultiple.body = "Some procedures could not be deleted because they are running";
                                }
                            }
                            deleteToastMultiple.show()
                        }
                        activity.stop()
                    }
                }
            ]

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        opacity: ListItem.active || ListItem.selected ? 0.5 : 1.0
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
                                text: ListItemData.flag
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

                        contextActions: [
                            ActionSet {
                                actions: [
                                    ActionItem {
                                        title: "Open"
                                        imageSource: "asset:///images/bb10/ic_open.png"
                                        onTriggered: {
                                            // Open Procedure page
                                            var item = Qt._libraryInstance_listView_listLibrary.dataModel.data(procedureItem.ListItem.view.selected())
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
                                            var model = Qt._libraryInstance_listView_listLibrary.dataModel.data(procedureItem.ListItem.view.selected())
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
                                    ActionItem {
                                        ActionBar.placement: ActionBarPlacement.InOverflow
                                        title: "Export"
                                        imageSource: "asset:///images/bb10/ic_entry.png"
                                        onTriggered: {
                                            exportSingleFilePicker.selectedItem = procedureItem.ListItem.view.selected()
                                            exportSingleFilePicker.open()
                                            procedureItem.ListItem.view.getActivity().start()
                                        }
                                        attachedObjects: [
                                            FilePicker {
                                                id: exportSingleFilePicker
                                                property variant selectedItem
                                                title: qsTr("Export")
                                                mode: FilePickerMode.Saver
                                                onFileSelected: {
                                                    var itemsList = new Array()
                                                    itemsList.push(Qt._libraryInstance_listView_listLibrary.dataModel.data(selectedItem))
                                                    exportedSingleToast.body = Qt._actiontrigger.exportData(selectedFiles + ".at", itemsList)
                                                    procedureItem.ListItem.view.getActivity().stop()
                                                    exportedSingleToast.cancel()
                                                    exportedSingleToast.show()
                                                }
                                                onCanceled: {
                                                    procedureItem.ListItem.view.getActivity().stop()
                                                    exportedSingleToast.cancel()
                                                    exportedSingleToast.body = "Cancelled"
                                                    exportedSingleToast.show()
                                                }
                                            },
                                            SystemToast {
                                                id: exportedSingleToast
                                            }
                                        ]
                                    },
                                    ActionItem {
                                        ActionBar.placement: ActionBarPlacement.InOverflow
                                        title: "Duplicate"
                                        imageSource: "asset:///images/bb10/ic_copy.png"
                                        onTriggered: {
                                            procedureItem.ListItem.view.getActivity().start()
                                            var item = Qt._libraryInstance_listView_listLibrary.dataModel.data(procedureItem.ListItem.view.selected())
                                            duplicateToast.body = Qt._actiontrigger.duplicate(item)
                                            duplicateToast.show()
                                            procedureItem.ListItem.view.getActivity().stop()
                                        }
                                        attachedObjects: [
                                            SystemToast {
                                                id: duplicateToast
                                            }
                                        ]
                                    },
                                    // TODO fix bug in multi select handler where sometimes one procedure does not get deleted
                                    MultiSelectActionItem {
                                        multiSelectHandler: procedureItem.ListItem.view.multiSelectHandler

                                        onTriggered: {
                                            // Set the list views multi select handler as active.
                                            multiSelectHandler.active;
                                        }
                                    },
                                    DeleteActionItem {
                                        attachedObjects: [
                                            SystemToast {
                                                id: deleteToast
                                            },
                                            SystemDialog {
                                                id: deleteDialog
                                                title: qsTr("Delete Item") + Retranslate.onLanguageChanged
                                                body: qsTr("This action will delete the selected item. This action cannot be reversed.") + Retranslate.onLanguageChanged
                                            }
                                        ]
                                        onTriggered: {
                                            if (! Qt._premium.askPurchase()) {
                                                return
                                            }
                                            procedureItem.ListItem.view.getActivity().start()
                                            var selectedItem = procedureItem.ListItem.view.selected()
                                            deleteDialog.exec()
                                            if (deleteDialog.result == SystemUiResult.ConfirmButtonSelection && selectedItem != null && Qt._libraryInstance_listView_listLibrary.dataModel.data(selectedItem) != null) {
                                                deleteToast.body = "Deleted";
                                                if (! Qt._libraryInstance_listView_listLibrary.dataModel.data(selectedItem).isRunning) {
                                                    Qt._libraryInstance_listView_listLibrary.dataModel.data(selectedItem).remove()
                                                    Qt._libraryInstance_listView_listLibrary.dataModel.removeAt(selectedItem)
                                                } else {
                                                    deleteToast.body = "Could not delete because it is running";
                                                }
                                                deleteToast.show()
                                            }
                                            selectedItem = null;
                                            procedureItem.ListItem.view.getActivity().stop()
                                        }
                                    }
                                ]
                            } // end of ActionSet
                        ]
                    }
                }
            ]
        }

    }

    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "New"
            imageSource: "asset:///images/bb10/ic_add.png"
            onTriggered: {
                if (! Qt._premium.askPurchase()) {
                    return
                }
                activity.start()
                filterTextField.text = ""
                var page = procedurePage.createObject()
                // First time tutorial
                //                if (Qt._actiontrigger.getPersistentVar("FIRST_TIME_NEW_PROCEDURE") != "TRUE") {
                //                    runTutorialDialog.exec()
                //                    if (runTutorialDialog.result == SystemUiResult.ConfirmButtonSelection) {
                //                        page.tutorial = {}
                //                    }
                //                    Qt._actiontrigger.setPersistentVar("FIRST_TIME_NEW_PROCEDURE", "TRUE")
                //                }
                Qt.ProcedurePageProcedureView = Qt._actiontrigger.createProcedure()
                page.isNew = true
                activity.stop()
                Qt._pane.push(page);
            }
            attachedObjects: [
                SystemDialog {
                    id: runTutorialDialog
                    title: "Tutorial"
                    body: "This is the first time you have created a procedure. Do you want to start with a tutorial?"
                }
            ]
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            title: "Import"
            imageSource: "asset:///images/bb10/ic_add_entry.png"
            onTriggered: {
                if (! Qt._premium.askPurchase()) {
                    return
                }
                importFilePicker.open()
                activity.start()
            }
            attachedObjects: [
                FilePicker {
                    id: importFilePicker
                    title: qsTr("Import")
                    mode: FilePickerMode.Picker
                    filter: [ "*.at" ]
                    onFileSelected: {
                        importedToast.body = Qt._actiontrigger.importDataFromFile(selectedFiles)
                        activity.stop()
                        importedToast.cancel()
                        importedToast.show()
                    }
                    onCanceled: {
                        activity.stop()
                        importedToast.cancel()
                        importedToast.body = "Cancelled"
                        importedToast.show()
                    }
                },
                SystemToast {
                    id: importedToast
                }
            ]
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            title: "Export All"
            imageSource: "asset:///images/bb10/ic_entry.png"
            onTriggered: {
                exportAllFilePicker.open()
                activity.start()
            }
            attachedObjects: [
                FilePicker {
                    id: exportAllFilePicker
                    title: qsTr("Export")
                    mode: FilePickerMode.Saver
                    filter: [ "*.at" ]
                    onFileSelected: {
                        exportedToast.body = Qt._actiontrigger.exportData(selectedFiles + ".at");
                        activity.stop()
                        exportedAllToast.cancel()
                        exportedAllToast.show()
                    }
                    onCanceled: {
                        activity.stop()
                        exportedAllToast.cancel()
                        exportedAllToast.body = "Cancelled"
                        exportedAllToast.show()
                    }
                },
                SystemToast {
                    id: exportedAllToast
                }
            ]
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            title: "Help"
            imageSource: "asset:///images/bb10/ic_help.png"
            onTriggered: {
                var page = helpSheet.createObject()
                page.topic = "library"
                page.open()
            }
        }
        //        DeleteActionItem {
        //            ActionBar.placement: ActionBarPlacement.InOverflow
        //            title: "Delete All"
        //        }
    ]
}