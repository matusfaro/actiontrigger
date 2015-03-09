import bb.cascades 1.2
import bb.platform 1.2

Sheet {
    content: Page {
        id: pageSupport
        titleBar: TitleBar {
            title: qsTr("Support") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    close()
                    destroy()
                }
            }
        }
        onCreationCompleted: {
            Qt._paymentmanager.purchaseFinished.connect(checkPurchases)
            Qt._paymentmanager.existingPurchasesFinished.connect(checkPurchases)
            Qt._pageSupport = pageSupport
            var premium = Qt._actiontrigger.hasPremium()
            if (premium == 1) {
                setPremiumView()
            } else if (premium == 2) {
                setBetaView()
            } else {
                setFreeView()
            }
        }
        function checkPurchases(reply) {
            if (Qt._actiontrigger.checkPurchase(reply)) {
                console.log("Receipt valid")
                setPremiumView()
            } else {
                console.log("Receipt invalid")
            }
        }
        function setBetaView() {
            updateStatus()
            freeContactContent.visible = false
            premiumContactContent.visible = false
            betaContactContent.visible = true
            freePurchaseContent.visible = false
            premiumPurchaseContent.visible = false
            betaPurchaseContent.visible = true
        }
        function setPremiumView() {
            updateStatus()
            betaContactContent.visible = false
            freeContactContent.visible = false
            premiumContactContent.visible = true
            betaPurchaseContent.visible = false
            freePurchaseContent.visible = false
            premiumPurchaseContent.visible = true
        }
        function setFreeView() {
            updateStatus()
            betaContactContent.visible = false
            freeContactContent.visible = true
            premiumContactContent.visible = false
            betaPurchaseContent.visible = false
            freePurchaseContent.visible = true
            premiumPurchaseContent.visible = false
        }
        function updateStatus() {
            var status = Qt._actiontrigger.premiumStatus()
            purchaseStatusLabel.text = status[1]
        }
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill

            DefaultPageContainer {
                
                // -- Contact info --
                Label {
                    text: "App information"
                }
                Label {
                    textStyle.fontSize: FontSize.Small
                    multiline: true
                    text: "<html>Website:
  <a href='http://actiontrigger.net/'>http://actiontrigger.net/</a>
Source code:
  <a href='https://github.com/matusfaro/actiontrigger'>https://github.com/matusfaro/actiontrigger</a>
</html>"
                }
                
                Divider {
                }

                // -- Contact info --
                Label {
                    text: "Contact information"
                }
                Label {
                    textStyle.fontSize: FontSize.Small
                    multiline: true
                    text: "Email:"
                }
                Container {
                    id: betaContactContent
                    visible: false
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Support"
                        onClicked: {
                            supportInvocationBeta.trigger("bb.action.SENDEMAIL")
                        }
                    }
                }
                Container {
                    id: premiumContactContent
                    visible: false
                    horizontalAlignment: HorizontalAlignment.Center

                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Support"
                        onClicked: {
                            supportInvocationPremium.trigger("bb.action.SENDEMAIL")
                        }
                    }
                }
                Container {
                    id: freeContactContent
                    visible: false
                    horizontalAlignment: HorizontalAlignment.Center

                    Button {
                        text: "Support"
                        onClicked: {
                            supportInvocationGeneral.trigger("bb.action.SENDEMAIL")
                        }
                    }
                }

                Divider {
                }

                // -- Purchase info --
                Label {
                    text: "Purchase details"

                    // Secret way to toggle premium, shh!
                    property int stage: 0
                    property double holdLast: 0
                    onTouchExit: {
                        var now = new Date().valueOf()
                        var holdTime = now - holdLast
                        holdLast = now
                        console.log("touch exit", holdTime)
                        if (holdTime > 4000 && holdTime < 6000) {
                            stage = stage + 1
                            if (stage == 3) {
                                stage = 0
                                Qt._actiontrigger.setPremium(! Qt._actiontrigger.hasPremium())
                                if (Qt._actiontrigger.hasPremium()) {
                                    pageSupport.setPremiumView()
                                } else {
                                    pageSupport.setFreeView()
                                }
                            }
                        } else {
                            stage = 0
                        }
                    }
                }
                Label {
                    id: purchaseStatusLabel
                    multiline: true
                    textStyle.fontStyle: FontStyle.Italic
                    horizontalAlignment: HorizontalAlignment.Center
                }

                Container {
                    id: betaPurchaseContent
                    visible: false

                    Label {
                        textStyle.fontSize: FontSize.Small
                        multiline: true
                        text: "Thank you for using this application. Please support the development by reporting bugs and emailing us your suggestions."
                    }
                }
                Container {
                    id: premiumPurchaseContent
                    visible: false

                    Label {
                        textStyle.fontSize: FontSize.Small
                        multiline: true
                        text: "Thank you for your contribution to support the development of this application."
                    }
                }
                Container {
                    id: freePurchaseContent
                    visible: false

                    Label {
                        textStyle.fontSize: FontSize.Small
                        multiline: true
                        text: "Please purchase this application. After the trial period is over, you will not be able to modify or create actiontriggers. This restriction is waived every Tuesday in order to keep the application usable while keeping an incentive to purchase."
                    }
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "Buy $4"
                        onClicked: {
                            Qt._paymentmanager.requestPurchase("SUPPORT", "", "Purchase")
                        }
                    }
                    //                    Label {
                    //                        textStyle.fontSize: FontSize.Small
                    //                        multiline: true
                    //                        text: "If you have already purchased this application, verify here when you are connected to the network."
                    //                    }
                    //                    Button {
                    //                        horizontalAlignment: HorizontalAlignment.Center
                    //                        text: "Check purchases"
                    //                        onClicked: {
                    //                            Qt._paymentmanager.requestExistingPurchases(true)
                    //                        }
                    //                    }
                }

                attachedObjects: [
                    Invocation {
                        id: supportInvocationGeneral
                        query {
                            mimeType: "text/plain"
                            invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                            uri: "mailto:actiontrigger-support-free@matus.mfaro.com"
                        }
                    },
                    Invocation {
                        id: supportInvocationPremium
                        query {
                            mimeType: "text/plain"
                            invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                            uri: "mailto:actiontrigger-support-premium@matus.mfaro.com"
                        }
                    },
                    Invocation {
                        id: supportInvocationBeta
                        query {
                            mimeType: "text/plain"
                            invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                            uri: "mailto:support-beta@actiontrigger.net"
                        }
                    }
                ]
            }
        }
    }
}
