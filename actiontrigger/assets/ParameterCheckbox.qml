import bb.cascades 1.2

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description
        checkboxValue.checked = model.value == "TRUE" ? true : false
        labelValue.text = model.value
        labelValue.active = true
    }
    function variableSelected(value) {
        labelValue.active = false
        checkboxValue.checked = false
        labelValue.active = true
        labelValue.text = value
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        // PARAMETER VALUE
        Label {
            text: "Value: "
            verticalAlignment: VerticalAlignment.Center
        }
        Label {
            property bool active: false
            id: labelValue
            verticalAlignment: VerticalAlignment.Center
            onTextChanged: {
                if (active) {
                    Qt.StatementPageParametersList[index].value = text
                }
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
        CheckBox {
            id: checkboxValue
            bottomMargin: 50
            verticalAlignment: VerticalAlignment.Center
            onCheckedChanged: {
                labelValue.text = checked ? "TRUE" : "FALSE"
            }
        }
        // VARIABLE
        ParameterVariable {
            visible: false
            id: paramButton
        }
    }
    Space {
    }
}
