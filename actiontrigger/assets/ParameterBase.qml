import bb.cascades 1.2

Container {
    property alias name: labelName.text
    property alias description: labelDescription.text
    Divider {
    }
    // PARAMETER NAME
    Label {
        id: labelName
        multiline: true
        textStyle.base: SystemDefaults.TextStyles.TitleText
        visible: false
        onTextChanged: {
            visible = text.length != 0 
        }
    }
    // PARAMETER DESCRIPTION
    Label {
        id: labelDescription
        multiline: true
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
        visible: false
        onTextChanged: {
            visible = text.length != 0 
        }
    }
}
