import bb.cascades 1.2

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description
        var value = model.value
        labelValue.text = value
        sliderValue.fromValue = model.getDetails()[0]
        sliderValue.toValue = model.getDetails()[1]
        sliderValue.stepSize = model.getDetails()[2]
        sliderValue.value = parseFloat(value, 10)
        labelValue.text = value
        sliderValue.active = true
    }
    function variableSelected(value) {
        sliderValue.active = false
        sliderValue.value = sliderValue.fromValue
        labelValue.text = value
        Qt.StatementPageParametersList[index].value = value
        sliderValue.active = true
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        Label {
            id: labelValue
            text: "a"
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
    Slider {
        property bool active: false
        property int stepSize
        id: sliderValue
        onImmediateValueChanged: {
            if (active) {
                labelValue.text = (immediateValue - (immediateValue % stepSize))
            }
        }
        onValueChanged: {
            if (active) {
                labelValue.text = (value - (value % stepSize))
                Qt.StatementPageParametersList[index].value = (value - (value % stepSize))
            }
        }
    }
    Space {
    }
}
