import bb.cascades 1.2

Button {
    imageSource: "asset:///images/bb10/ic_entry.png"
    maxWidth: 1
    onClicked: {
        var page = selectVariablePage.createObject();
        page.variableSelected.connect(variableSelected)
        Qt._pane.push(page);
    }
    attachedObjects: [
        ComponentDefinition {
            id: selectVariablePage
            source: "asset:///SelectVariable.qml"
        }
    ]
}
