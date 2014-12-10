import bb.cascades 1.2

Container {
    property int index
    property Option customOption
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description
        var value = model.value
        var atLeastOneOptionChecked = false 
        // First description is the name that appears on dropdown
        dropdownValue.title = model.getDetails()[0]
        for (var i = 0; i < model.getOptionCount(); ++ i) {
            var typeOption = model.getOption(i)
            var option = factoryOption.createObject()
            option.text = typeOption[0]
            option.value = typeOption[1]
            if (value == typeOption[1]) {
                option.selected = true
                atLeastOneOptionChecked = true 
            }
            dropdownValue.add(option)
        }
        if(!atLeastOneOptionChecked){
            populateCustomOption()
            customOption.text = value
            customOption.value = value
            customOption.selected = true
        }
        dropdownValue.active = true
    }
    function populateCustomOption() {
        customOption = factoryOption.createObject()
        dropdownValue.add(customOption)
    }
    function variableSelected(value) {
        if (customOption == null) {
            populateCustomOption()
        }
        customOption.text = value
        customOption.value = value
        customOption.selected = true
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        DropDown {
            property bool active: false
            id: dropdownValue
            bottomMargin: 50
            horizontalAlignment: HorizontalAlignment.Fill
            onSelectedValueChanged: {
                if (active) {
                    Qt.StatementPageParametersList[index].value = selectedValue;
                }
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
    attachedObjects: [
        ComponentDefinition {
            id: factoryOption
            Option {
            }
        }
    ]
}
