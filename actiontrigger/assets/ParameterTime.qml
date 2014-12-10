import bb.cascades 1.2

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description
        labelValue.text = model.value
        picker.value = picker.dateFromTime(model.value)
        picker.active = true
    }
    function variableSelected(value) {
        if (picker.active) {
            labelValue.text = value
            Qt.StatementPageParametersList[index].value = value
        }
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        Label {
            id: labelValue
            text: ""
            textStyle.textAlign: TextAlign.Center
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
        // VARIABLE
        ParameterVariable {
            visible: false
            id: paramButton
        }
    }
    // TIME
    DateTimePicker {
        property bool active: false
        id: picker
        mode: DateTimePickerMode.Timer
        onValueChanged: {
            variableSelected(value.toLocaleTimeString())
        }
    }
    Space {
    }
}
