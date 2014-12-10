import bb.cascades 1.2
import bb.system 1.2
import bb.platform 1.2

Sheet {
    id: _sheetGetProfileConfirm
    signal accept()
    signal deny()
    function init(bbid_uid, bbid_screenname) {
        labelMessage.text = "<html>We require your permisison to use your personal information. Your Screen Name will be visible to the public and your Unique Blackberry Identifier will be used to authenticate your account. If you agree to the Terms of Service and Privacy Policy, confirm your submission of the following information:\n\n<strong>Public Screen Name:</strong>\n   " + bbid_screenname + "\n<strong>Blackberry ID:</strong>\n   " + bbid_uid + "\n\nPrivacy Policy and Terms of Service\n" + _GetProfileConfirm.legalUrl + "</html>"
    }
    Page {
        id: _GetProfileConfirm
        property variant result
        property string legalUrl: "http://actiontrigger.net/?page=legal"
//        property string legalUrl: "http://169.254.0.2/bbstore/actiontrigger.net/?page=legal"
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        DefaultPageContainer {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            DefaultContainer {
                Label {
                    id: servicesPageTitle
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: "Terms of Service"
                }
            }
            ScrollView {
                id: scrollView
                scrollRole: ScrollRole.Main
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 5
                }
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                Container {
                    Label {
                        id: labelMessage
                        textFormat: TextFormat.Html
                        multiline: true
                        content.flags: TextContentFlag.ActiveText
                    }
                    WebView {
                        id: webviewLegal
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
                        onCreationCompleted: {
                            url = _GetProfileConfirm.legalUrl
                        }
                        attachedObjects: LayoutUpdateHandler {
                            onLayoutFrameChanged: {
                                webviewLegal.minHeight = layoutFrame.height
                            }
                        }
                    }
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
                title: "Accept"
                imageSource: "asset:///images/bb10/ic_done.png"
                onTriggered: {
                    _sheetGetProfileConfirm.close()
                    accept()
                }
            },
            ActionItem {
                ActionBar.placement: ActionBarPlacement.OnBar
                title: "Cancel"
                imageSource: "asset:///images/bb10/ic_cancel.png"
                onTriggered: {
                    _sheetGetProfileConfirm.close()
                    deny()
                }
            }
//            ActionItem {
//                ActionBar.placement: ActionBarPlacement.OnBar
//                title: "Reload"
//                imageSource: "asset:///images/bb10/ic_reload.png"
//                onTriggered: {
//                    if (webviewLegal.url != _GetProfileConfirm.legalUrl) {
//                        webviewLegal.url = _GetProfileConfirm.legalUrl
//                    } else {
//                        webviewLegal.reload()
//                    }
//                }
//            }
        ]
    }
}