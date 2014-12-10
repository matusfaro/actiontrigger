import bb.cascades 1.2

Container {
    property int index

    property int lastSelectedIndex: -1
    function init() {
        var model = Qt.StatementPageParametersList[index]
        base.name = model.name
        base.description = model.description
        var value = model.value

        for (var i = 0; i < model.getOptionCount(); ++ i) {
            var typeOption = model.getOption(i)
            var option = factoryOption.createObject()
            option.text = typeOption[0]
            option.description = typeOption[1]
            option.value = typeOption[2]
            if (value == option.value) {
                lastSelectedIndex = i
                option.selected = true
            } else {
                optionDeselected(i)
            }
            dropdownFilter.add(option)
        }
        if (lastSelectedIndex != -1)
            optionSelected(lastSelectedIndex)

        dropdownFilter.active = true
    }

    function optionSelected(optionIndex) {
        var controls = Qt.StatementPageParametersList[index].getOption(optionIndex);
        // Iterate over controls that need to be
        for (var i = 3; i < controls.length; ++ i) {
            parametersContainer.at(controls[i]).visible = true
        }
    }

    function optionDeselected(optionIndex) {
        var controls = Qt.StatementPageParametersList[index].getOption(optionIndex);
        // Iterate over controls that need to be
        for (var i = 3; i < controls.length; ++ i) {
            parametersContainer.at(controls[i]).visible = false
        }
    }

    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        DropDown {
            property bool active: false
            id: dropdownFilter
            bottomMargin: 50
            horizontalAlignment: HorizontalAlignment.Fill
            onSelectedValueChanged: {
                if (active) {
                    if (lastSelectedIndex != -1)
                        optionDeselected(lastSelectedIndex)
                    lastSelectedIndex = parseInt(selectedIndex)
                    optionSelected(selectedIndex)
                    Qt.StatementPageParametersList[index].value = selectedValue;
                }
            }
        }
    }
    Space {
    }
    attachedObjects: [
        ComponentDefinition {
            id: factoryOption
            Option {
            }
        }
    ]
}
