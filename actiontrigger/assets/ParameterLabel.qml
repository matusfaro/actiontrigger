import bb.cascades 1.2

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        base.name = model.name
        base.description = model.description
    }
    // LABEL
    ParameterBase {
        id: base
    }
    Space {
    }
}
