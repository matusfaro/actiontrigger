import bb.cascades 1.2

/**
 * When using as list item with selection, don't forget to set this:
 * opacity: procedureItem.ListItem.active || procedureItem.ListItem.selected ? 0.5 : 1.0
 */

Container {
    property bool showDivider: true
    property alias padding: root.leftPadding
    property alias title: labelTitle.text
    property alias flag: labelFlag.text
    property alias description: labelDescription.text
    property alias tap: selectAllHandler // list item tap handler
    property alias showSelect: selectImage.visible
    property alias select: selectHandler // select image tap handler
    property alias selectImage: selectImage.imageSource
    property bool expandDescOnTap: true

    Divider {
        topMargin: 0
        bottomMargin: 0
        visible: showDivider
    }
    Container {
        id: root
        leftPadding: 30
        layout: StackLayout {
        }
        Container {
            minHeight: 80
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                layout: StackLayout {
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    // PARAMETER NAME
                    Label {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                        id: labelTitle
                        verticalAlignment: VerticalAlignment.Center
                        multiline: true
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        visible: false
                        onTextChanged: {
                            visible = text.length != 0
                        }
                        topMargin: 0
                        bottomMargin: topMargin
                    }

                    // FLAG
                    Label {
                        id: labelFlag
                        visible: false
                        textStyle.fontSize: FontSize.XSmall
                        textStyle.fontStyle: FontStyle.Italic
                        verticalAlignment: VerticalAlignment.Center
                        onTextChanged: {
                            visible = text != ""
                        }
                    }
                }

                // PARAMETER DESCRIPTION
                Label {
                    id: labelDescription
                    multiline: false
                    textStyle.base: SystemDefaults.TextStyles.SubtitleText
                    visible: false
                    onTextChanged: {
                        visible = text.length != 0
                    }
                    topMargin: 0
                    bottomMargin: topMargin
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            if (expandDescOnTap)
                                labelDescription.multiline = ! labelDescription.multiline
                        }
                    }
                ]

            }
            ImageView {
                id: selectImage
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                scalingMethod: ScalingMethod.AspectFit
                imageSource: "asset:///images/bb10/ic_next.png"
                gestureHandlers: [
                    TapHandler {
                        id: selectHandler
                    }
                ]
            }
        }
    }
    Divider {
        topMargin: 0
        bottomMargin: 0
        id: divider
        visible: showDivider
    }
    gestureHandlers: [
        TapHandler {
            id: selectAllHandler
        }
    ]
}
