import bb.cascades 1.2
import bb.system 1.2
import bb.platform 1.2
import ActionTrigger 1.0

Page {
    id: _Catalog
    property string portalUrl: "http://app.actiontrigger.net/"
    //    property string portalUrl: "http://169.254.0.2/bbstore/app.actiontrigger.net/"
    function init() {
        Qt._actiontrigger.identityResult.connect(webviewCatalog.getProfileStepTwo)
        sheetGetProfileConfirm.accept.connect(webviewCatalog.getProfileStepFour)
        sheetGetProfileConfirm.deny.connect(webviewCatalog.getProfileStepFailure)
    }
    property bool inited: false
    function firstTimeInit() {
        if (! inited) {
            webviewCatalog.url = portalUrl
            inited = true
        }
    }
    property string tosKey: "ACCEPTED_TOS_V_ONE_TEST5"
    function acceptTos() {
        Qt._actiontrigger.setPersistentVar(tosKey, "TRUE")
    }
    function hasAcceptedTos() {
        return (Qt._actiontrigger.getPersistentVar(tosKey) == "TRUE")
    }
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    DefaultPageContainer {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        DefaultActivity {
            id: activity
            running: false
            onRunningChanged: {
                visible = running
            }
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            layoutProperties: StackLayoutProperties {
                spaceQuota: 5
            }
            attachedObjects: [
                LayoutUpdateHandler {
                    onLayoutFrameChanged: {
                        webviewCatalog.preferredHeight = layoutFrame.height
                    }
                }
            ]
            WebView {
                minHeight: 500
                id: webviewCatalog
                onLoadProgressChanged: {
                    if (loadProgress >= 100) {
                        progressIndicator.visible = false;
                    } else {
                        progressIndicator.visible = true;
                    }
                    progressIndicator.value = loadProgress
                }
                settings {
                    background: Color.Black
                    webInspectorEnabled: false
                }
                property variant lastId
                property bool bbid_set: false
                property string bbid_uid
                property string bbid_screenname
                function getProfileStepOne() {
                    if (bbid_set) {
                        getProfileStepThree(bbid_uid, bbid_screenname)
                    } else {
                        Qt._actiontrigger.requestIdentity()
                    }
                }
                function getProfileStepTwo(result) {
                    console.log("identity result: ", JSON.stringify(result))
                    if (result.success) {
                        bbid_uid = result['urn:bbid:uid'],
                        bbid_screenname = result['urn:bbid:screenname']
                        bbid_set = true
                        getProfileStepThree(bbid_uid, bbid_screenname)
                    } else {
                        notice.title = "Unable to retrieve profile"
                        notice.body = result.error
                        notice.show()
                        // Oops, no personally identifiable information for you
                        getProfileStepFailure()
                    }
                }
                function getProfileStepThree(uid, screenname) {
                    if (hasAcceptedTos()) {
                        getProfileStepSuccess()
                    } else {
                        sheetGetProfileConfirm.init(bbid_uid, bbid_screenname)
                        sheetGetProfileConfirm.open()
                    }
                }
                function getProfileStepFour() {
                    // TOS accepted
                    acceptTos()
                    getProfileStepSuccess()
                }
                function getProfileStepSuccess() {
                    webviewCatalog.postMessage(JSON.stringify({
                                id: lastId,
                                message: {
                                    success: true,
                                    profile: {
                                        uid: bbid_uid,
                                        screenname: bbid_screenname
                                    }
                                }
                            }))
                    activity.stop()
                }
                function getProfileStepFailure() {
                    webviewCatalog.postMessage(JSON.stringify({
                                id: lastId,
                                message: {
                                    success: false
                                }
                            }))
                    activity.stop()
                }
                onMessageReceived: {
                    console.log("Recv msg from JS: ", JSON.stringify(message))
                    var data = JSON.parse(message.data)
                    if (data.id == undefined || data.call == undefined) {
                        console.log("Missing id or call in message", JSON.stringify(data))
                        return
                    }

                    if (data.call == 'proto_version') {
                        console.log("proto_version call")
                        console.log("Web client protocol version:", data.version)
                        webviewCatalog.postMessage(JSON.stringify({
                                    id: data.id,
                                    message: {
                                        version: 1
                                    }
                                }))
                    } else if (data.call == 'get_profile') {
                        console.log("get_profile call");
                        activity.start()
                        lastId = data.id
                        getProfileStepOne()
                    } else if (data.call == 'sign') {
                        console.log("sign call")
                        webviewCatalog.postMessage(JSON.stringify({
                                    id: data.id,
                                    message: {
                                        signature: Qt._actiontrigger.sign(data.content)
                                    }
                                }))
                    } else if (data.call == 'request_upload') {
                        console.log("request_upload call")
                        activity.start()
                        Qt._actiontrigger.populateProcedures(procedureUploadPickerDialog)
                        procedureUploadPickerDialog.exec()
                        if (procedureUploadPickerDialog.result == SystemUiResult.ConfirmButtonSelection) {
                            var procedure = Qt._actiontrigger.getProcedureFromDialogResult(procedureUploadPickerDialog)
                            webviewCatalog.postMessage(JSON.stringify({
                                        id: data.id,
                                        message: {
                                            success: true,
                                            name: procedure.name,
                                            content: Qt._actiontrigger.serializeProcedure(procedure)
                                        }
                                    }))
                        } else {
                            webviewCatalog.postMessage(JSON.stringify({
                                        id: data.id,
                                        message: {
                                            success: false
                                        }
                                    }))
                        }
                        activity.stop()
                    } else if (data.call == 'request_download') {
                        console.log("request_download call")
                        activity.start()
                        downloadDialog.exec()
                        if (downloadDialog.result == SystemUiResult.ConfirmButtonSelection) {
                            console.log("Downloading:", data.content)
                            noticeToast.cancel()
                            noticeToast.body = Qt._actiontrigger.importDataFromString(data.content)
                            noticeToast.show()
                            webviewCatalog.postMessage(JSON.stringify({
                                        id: data.id,
                                        message: {
                                            success: true
                                        }
                                    }))
                        } else {
                            webviewCatalog.postMessage(JSON.stringify({
                                        id: data.id,
                                        message: {
                                            success: false
                                        }
                                    }))
                        }
                        activity.stop()
                    } else {
                        console.log("Unknown call:", data.call)

                    }
                }
                attachedObjects: [
                    SystemToast {
                        id: noticeToast
                        body: ""
                    },
                    SystemDialog {
                        id: notice
                        title: "Unable to retrieve profile"
                        body: ""
                        cancelButton.label: undefined
                    },
                    GetProfileConfirm {
                        id: sheetGetProfileConfirm
                    },
                    SystemDialog {
                        id: downloadDialog
                        title: "Download request"
                        body: "Do you want to download this procedure?"
                        confirmButton.label: "Download"
                        confirmButton.enabled: true
                        cancelButton.label: "Cancel"
                        cancelButton.enabled: true
                        defaultButton: cancelButton
                    },
                    SystemListDialog {
                        id: procedureUploadPickerDialog
                        title: "Select Procedure to upload"
                        selectionMode: ListSelectionMode.Single
                        confirmButton.enabled: true
                        cancelButton.label: "Cancel"
                        cancelButton.enabled: true
                        selectionIndicator: ListSelectionIndicator.Highlight
                    }
                ]
            }
        }
        ProgressIndicator {
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1.0
            }
            visible: false
            id: progressIndicator
            topMargin: 10
            leftMargin: 10
            bottomMargin: 10
            fromValue: 0
            toValue: 100
            preferredWidth: 800
        }
    }
    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Reload"
            imageSource: "asset:///images/bb10/ic_reload.png"
            onTriggered: {
                if (webviewCatalog.url != portalUrl) {
                    webviewCatalog.url = portalUrl
                } else {
                    webviewCatalog.reload()
                }
            }
        }
    ]
}
