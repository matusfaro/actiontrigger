import bb.cascades 1.2
import bb.cascades.pickers 1.0

Container {
    property int index
    function init() {
        var model = Qt.StatementPageParametersList[index]
        paramButton.visible = model.showParams
        base.name = model.name
        base.description = model.description

        var filePickerMode = model.getDetails()[0]
        console.log("Found filePickerMode: ", filePickerMode)
        switch (filePickerMode) {
            default:
            case "0":
                filePicker.mode = FilePickerMode.Picker
                break
            case "1":
                filePicker.mode = FilePickerMode.Saver
                break
            case "2":
                filePicker.mode = FilePickerMode.PickerMultiple
                break
            case "3":
                filePicker.mode = FilePickerMode.SaverMultiple
                break
        }

        var fileType = model.getDetails()[1]
        console.log("Found filetype: ", fileType)
        switch (fileType) {
            case "1":
                filePicker.type = FileType.Picture
                break
            case "2":
                filePicker.type = FileType.Document
                break
            case "4":
                filePicker.type = FileType.Music
                break
            case "8":
                filePicker.type = FileType.Video
                break
            case "16":
                filePicker.type = FileType.Other
                break
            default:
                break
        }

        var filter = model.getDetails()[2]
        console.log("Found filter: ", fileType)
        if (filter)
            filePicker.filter = [ String(filter) ]

        var pathAppend = model.getDetails()[3]
        console.log("Found pathAppend: ", pathAppend)
        if (pathAppend)
            filePicker.pathAppend = pathAppend

        textValue.text = model.value
        textValue.active = true
    }
    function variableSelected(value) {
        textValue.text = value
    }
    // PARAMETER NAME DESCRIPTION
    ParameterBase {
        id: base
    }
    DefaultContainer {
        // PARAMETER VALUE
        TextField {
            property bool active: false
            id: textValue
            hintText: "Enter a value"
            bottomMargin: 50
            horizontalAlignment: HorizontalAlignment.Fill
            onTextChanging: {
                if (active) {
                    Qt.StatementPageParametersList[index].value = text;
                }
            }
        }
        // VARIABLE
        Button {
            imageSource: "asset:///images/bb10/ic_doctype_picture.png"
            maxWidth: 1
            onClicked: {
                filePicker.open()
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
        FilePicker {
            property string pathAppend: ""
            id: filePicker
            title: qsTr("File Picker")
            viewMode: FilePickerViewMode.Default
            sortBy: FilePickerSortFlag.Default
            sortOrder: FilePickerSortOrder.Default
            onFileSelected: {
                var filesString = selectedFiles[0] + pathAppend
                for (var i = 1; i < selectedFiles.length; i ++) {
                    filesString += "," + selectedFiles[i] + pathAppend
                }
                textValue.text = filesString
            }
        }
    ]
}
