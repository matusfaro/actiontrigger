import bb.cascades 1.2

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description
        textValue.text = model.value
        textValue.active = true
    }
    function variableSelected(value) {
        textValue.text += value
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    // PARAMETER VALUE
    TextArea {
        property bool active: false
        id: textValue
        inputMode: TextAreaInputMode.Text
        hintText: "Enter a value"
        horizontalAlignment: HorizontalAlignment.Fill
        onTextChanging: {
            if (active) {
                Qt.StatementPageParametersList[index].value = text;
            }
        }
    }
    // VARIABLE
    ParameterVariable {
        visible: false
        id: paramButton
    }
    Space {
    }
}
