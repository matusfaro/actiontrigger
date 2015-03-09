import bb.cascades 1.2
import ActionTrigger 1.0
import bb.system 1.2
import bb.platform 1.2

TabbedPane {
    function init() {
        Qt._actiontrigger = _actiontrigger
        ////// HEADLESS DISABLED
        //Qt._headlessConnector = Qt._actiontrigger.getGlobalResources().getHeadlessConnector()
        //Qt._headlessConnector.invoke()
        Qt._premium = premium

        var resetApplication = false
        var preventAutoStart = false

        // App crash detection?
        if (false) {
            if (Qt._actiontrigger.hasDetectedCrash() == 1) {
                // Restore to defaults?
                restoreDefaultsDialog.exec()
                if (restoreDefaultsDialog.result == SystemUiResult.ConfirmButtonSelection) {
                    restoreDefaultsConfirmDialog.exec()
                    if (restoreDefaultsConfirmDialog.result == SystemUiResult.ConfirmButtonSelection) {
                        resetApplication = true
                        console.log("setting reset to true")
                    }
                }

                // Prevent auto start?
                if (resetApplication == false) {
                    preventAutoStartDialog.exec()
                    if (preventAutoStartDialog.result == SystemUiResult.ConfirmButtonSelection) {
                        preventAutoStart = true
                        console.log("setting prevent to true")
                    }
                }
            }
        }

        // Set globals
        Qt._paymentmanager = paymentmanager
        Qt._overviewPage_showAll = false
        Qt._libraryInstance_listView_listLibrary = libraryInstance.prop_listView_listLibrary
        Qt._overviewInstance_listView_listLibrary = overviewTabInstance.prop_listViewOverview
//        Qt._debugInstance_listView_listDebug = debugTabInstance.prop_listViewDebug
//        Qt._headlessInstance_listView_listLibrary = headlessTabInstance.prop_listViewHeadless
        Qt._app = _app
        Qt._procedurePage = procedurePage
        Qt._pane = paneOverview
        Qt._supportSheet = supportSheet

        // Payment service test only
        /*Qt._paymentmanager.setConnectionMode(0);*/

        // Setup debug logging
//        Qt._actiontrigger.getGlobalResources().setDebugDataModel(Qt._debugInstance_listView_listDebug.dataModel)
        
        // Check for existing purchases
        if (! Qt._actiontrigger.hasPremium()) {
            Qt._paymentmanager.requestExistingPurchases()
        }

        // Enable user facing messages
        Qt._actiontrigger.displayUserMessage.connect(displayUserMessage);
        Qt._actiontrigger.postNotification.connect(postNotification);
        
        // Connect to headless
        ////// HEADLESS DISABLED
        //Qt._headlessConnector.start()

        // Set data models
        Qt._overviewInstance_listView_listLibrary.dataModel = libraryInstance.prop_listView_dataModel_listLibrary
//        Qt._headlessInstance_listView_listLibrary.dataModel = libraryInstance.prop_listView_dataModel_listLibrary
        Qt._app.asyncLoad(resetApplication, preventAutoStart, splashLogDataModel, libraryInstance.prop_listView_dataModel_listLibrary, loadAnimationEnd)
    }
    function postLoad() {
        tabDashboard.enabled = true
        tabLibrary.enabled = true
        tabTutorials.enabled = true
    }

    function displayUserMessage(msg) {
        //        allPurposeToast.cancel()
        allPurposeToast.body = msg
        allPurposeToast.show()
    }
    function postNotification(data) {
        globalNotificationDialog.title = data.title
        globalNotificationDialog.body = data.body
        globalNotificationDialog.repeat = data.repeat
        globalNotificationDialog.soundUrl = data.soundUrl
        globalNotificationDialogButton.label = data.buttonLabel
        globalNotificationDialog.show()
    }

    onCreationCompleted: {
        console.log("main.qml onCreationCompleted")
        splashSheet.open()
    }

    Menu.definition: MenuDefinition {
        // HELP
        helpAction: HelpActionItem {
            onTriggered: {
                helpSheet.createObject().open();
            }
        }
        // SUPPORT
        actions: [
            ActionItem {
                title: qsTr("Support") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/bb10/ic_feedback.png"
                onTriggered: {
                    supportSheet.createObject().open();
                }
            }
        ]
        //        // SETTINGS
        //        settingsAction: SettingsActionItem {
        //            onTriggered: {
        //                var sheet = settingsSheet.createObject();
        //                sheet.init();
        //                sheet.open()
        //            }
        //        }
    }
    showTabsOnActionBar: false
    Tab {
        enabled: false
        id: tabDashboard
        title: "Dashboard"
        imageSource: "asset:///images/bb10/ic_home.png"
        NavigationPane {
            id: paneOverview
            OverviewTab {
                id: overviewTabInstance
            }
            onPushTransitionEnded: {
                if (page && page.init) {
                    page.init()
                }
            }
            onPopTransitionEnded: {
                if (page) {
                    if (page.cleanup) {
                        page.cleanup()
                    }
                    /*
                     * DO NOT DESTROY THE PAGE! FIX ME!
                     * Actions and triggers and possibly more become the children of the page and get destroyed as well.
                     */
                    //page.destroy()
                }
            }
        }
        onTriggered: {
            Qt._pane = paneOverview
            Qt._overviewInstance_listView_listLibrary.dataModel.sortingKeys = [ "group" ]
            libraryInstance.prop_filterTextField.text = ""
            //            libraryInstance.prop_listView_listLibrary.scrollRole = ScrollRole.None
            //            overviewTabInstance.prop_listViewOverview.scrollRole = ScrollRole.Main
            overviewTabInstance.prop_listViewOverview.dataModel.resetFilter()
        }
    }
    
    Tab {
        enabled: false
        id: tabLibrary
        title: "Library"
        imageSource: "asset:///images/bb10/ic_edit_list.png"
        NavigationPane {
            id: paneLibrary
            Library {
                id: libraryInstance
            }
            onPushTransitionEnded: {
                if (page && page.init) {
                    page.init()
                }
            }
            onPopTransitionEnded: {
                if (page) {
                    if (page.cleanup) {
                        page.cleanup()
                    }
                    /*
                     * DO NOT DESTROY THE PAGE! FIX ME!
                     * Actions and triggers and possibly more become the children of the page and get destroyed as well.
                     */
                    //page.destroy()
                }
            }
        }
        onTriggered: {
            Qt._pane = paneLibrary
            //            overviewTabInstance.prop_listViewOverview.scrollRole = ScrollRole.None
            //            libraryInstance.prop_listView_listLibrary.scrollRole = ScrollRole.Main
            overviewTabInstance.prop_listViewOverview.dataModel.sortingKeys = [ "group" ]
            overviewTabInstance.prop_showAll = true
            overviewTabInstance.prop_viewToggle.imageSource = "asset:///images/bb10/ic_favorite.png"
            overviewTabInstance.prop_viewToggle.title = "Favourites"
        }
    }
    
//    Tab {
//        enabled: true
//        id: tabHeadless
//        title: "Headless Monitor"
//        imageSource: "asset:///images/bb10/ic_diagnostics.png"
//        NavigationPane {
//            id: paneHeadless
//            Headless {
//                id: headlessTabInstance
//            }
//            onPushTransitionEnded: {
//                if (page && page.init) {
//                    page.init()
//                }
//            }
//            onPopTransitionEnded: {
//                if (page) {
//                    if (page.cleanup) {
//                        page.cleanup()
//                    }
//                    /*
//                     * DO NOT DESTROY THE PAGE! FIX ME!
//                     * Actions and triggers and possibly more become the children of the page and get destroyed as well.
//                     */
//                    //page.destroy()
//                }
//            }
//        }
//        onTriggered: {
//            Qt._pane = paneHeadless
//            Qt._headlessInstance_listView_listLibrary.dataModel.sortingKeys = [ "isRunningAsString", "backgroundedAsString" ]
//        }
//    }

    Tab {
        title: "Tutorials"
        enabled: false
        id: tabTutorials
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/bb10/ic_info.png"
        NavigationPane {
            id: paneTutorials
            TutorialList {
                id: tutorialInstance
            }
            onPushTransitionEnded: {
                if (page && page.init) {
                    page.init()
                }
            }
            onPopTransitionEnded: {
                if (page) {
                    if (page.cleanup) {
                        page.cleanup()
                    }
                    /*
                     * DO NOT DESTROY THE PAGE! FIX ME!
                     * Actions and triggers and possibly more become the children of the page and get destroyed as well.
                     */
                    //page.destroy()
                }
            }
        }
        onTriggered: {
            Qt._pane = paneTutorials
        }
    }

    Tab {
        title: "Online Portal"
        ActionBar.placement: ActionBarPlacement.OnBar
        imageSource: "asset:///images/bb10/ic_browser.png"
        NavigationPane {
            id: paneCatalog
            Catalog {
                id: catalogInstance
            }
            onPushTransitionEnded: {
                if (page && page.init) {
                    page.init()
                }
            }
            onPopTransitionEnded: {
                if (page) {
                    if (page.cleanup) {
                        page.cleanup()
                    }
                    /*
                     * DO NOT DESTROY THE PAGE! FIX ME!
                     * Actions and triggers and possibly more become the children of the page and get destroyed as well.
                     */
                    //page.destroy()
                }
            }
        }
        onTriggered: {
            catalogInstance.firstTimeInit()
        }
    }

    
//        Tab {
//            enabled: true
//            id: tabDebug
//            title: "Debug"
//            imageSource: "asset:///images/bb10/ic_diagnostics.png"
//            NavigationPane {
//                
//                id: paneDebug
//                Debug {
//                    id: debugTabInstance
//                }
//                onPushTransitionEnded: {
//                    if (page && page.init) {
//                        page.init()
//                    }
//                }
//                onPopTransitionEnded: {
//                    if (page) {
//                        if (page.cleanup) {
//                            page.cleanup()
//                        }
//                        /*
//                         * DO NOT DESTROY THE PAGE! FIX ME!
//                         * Actions and triggers and possibly more become the children of the page and get destroyed as well.
//                         */
//                        //page.destroy()
//                    }
//                }
//            }
//            onTriggered: {
//                Qt._pane = paneDebug
//            }
//        }


    /*// Unimplemented
     * Tab {
     * title: "Permissions"
     * enabled: false
     * ActionBar.placement: ActionBarPlacement.OnBar
     * imageSource: "asset:///images/bb10/ic_lock.png"
     * Permissions {
     * }
     * }
     */

    attachedObjects: [
        SystemToast {
            autoUpdateEnabled: true
            id: allPurposeToast
        },
        NotificationDialog {
            id: globalNotificationDialog
            repeat: true
            buttons: SystemUiButton {
                id: globalNotificationDialogButton
            }
        },
        PaymentManager {
            id: paymentmanager
            windowGroupId: Application.mainWindow.groupId
            onExistingPurchasesFinished: {
                Qt._actiontrigger.checkPurchase(reply)
            }
            onPurchaseFinished: {
                Qt._actiontrigger.checkPurchase(reply)
            }
        },
        SystemDialog {
            id: restoreDefaultsDialog
            title: "Reset application"
            body: "Unexpected shutdown detected. If this is not the first time you are seeing this message, you can choose to reset the application, settings and all procedures to factory defaults."
            confirmButton.label: qsTr("Reset")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Continue")
            cancelButton.enabled: true
            defaultButton: cancelButton
        },
        SystemDialog {
            id: restoreDefaultsConfirmDialog
            title: "Reset application"
            body: "Are you sure? \n\n Warning: this will erase all user data and settings!"
            confirmButton.label: qsTr("Reset")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel")
            cancelButton.enabled: true
            defaultButton: cancelButton
        },
        SystemDialog {
            id: preventAutoStartDialog
            title: "Prevent auto start"
            body: "Would you like to prevent actionTriggers from starting automatically on startup?"
            confirmButton.label: qsTr("Prevent")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Continue")
            cancelButton.enabled: true
            defaultButton: cancelButton
        },
        ComponentDefinition {
            id: procedurePage
            source: "asset:///Procedure.qml"
        },
        ComponentDefinition {
            id: helpSheet
            Help {
            }
        },
        ComponentDefinition {
            id: supportSheet
            Support {
            }
        },
        //        ComponentDefinition {
        //            id: settingsSheet
        //            Settings {
        //            }
        //        },
        Sheet {
            id: splashSheet
            peekEnabled: true
            Page {
                DefaultPageContainer {
                    Label {
                        id: loadingText
                        textStyle.fontSize: FontSize.XXLarge
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Top
                        text: "Loading... "
                    }
                    ListView {
                        id: splashLogListView
                        scrollRole: ScrollRole.None
                        stickToEdgePolicy: ListViewStickToEdgePolicy.End
                        dataModel: ArrayDataModel {
                            id: splashLogDataModel
                        }
                        listItemComponents: [
                            ListItemComponent {
                                Label {
                                    bottomMargin: 0
                                    topMargin: 0
                                    text: ListItemData
                                }

                            }
                        ]
                        onCreationCompleted: {
                            loadAnimationStart.play();
                        }
                    }

                    translationY: 0
                    animations: [
                        TranslateTransition {
                            id: loadAnimationStart
                            fromX: 3000
                            toX: 0
                            easingCurve: StockCurve.QuinticOut
                            duration: 2000
                            target: loadingText
                            onEnded: {
                                init()
                            }
                        },
                        TranslateTransition {
                            id: loadAnimationEnd
                            toY: -3000
                            easingCurve: StockCurve.QuinticIn
                            duration: 500
                            target: loadingText
                            onStarted: {
                                loadingText.text = loadingText.text + "done"
                            }
                            onEnded: {
                                splashSheet.close()
                                postLoad()
                                overviewTabInstance.init()
                            }
                        }
                    ]
                }
            }
        },
        /*
         * Trial period checker
         */
        SystemDialog {
            id: premium
            function isAllowed() {
                var status = Qt._actiontrigger.premiumStatus()
                console.log("Premium status: ", status)
                return status[0]
            }
            function askPurchase() {
                if (isAllowed()) {
                    return true
                }
                console.log("exec()")
                premium.exec()
                if (premium.result == SystemUiResult.ConfirmButtonSelection) {
                    console.log("Purchase")
                    supportSheet.createObject().open();
                    return isAllowed()
                } else {
                    console.log("Cancelled")
                    return false
                }
            }
            title: "Trial period expired"

            body: "Modifications are disallowed after trial expires with the exception of Tuesdays.

Please visit the support page to purchase the application."
            confirmButton.label: qsTr("Support page")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel")
            cancelButton.enabled: true
            defaultButton: cancelButton
        }
    ]
}
