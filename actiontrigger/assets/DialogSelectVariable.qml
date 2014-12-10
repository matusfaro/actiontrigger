import bb.cascades 1.2
Dialog {
    id: dialog
    property int index
    property alias varList: model
    Container {
        // Semi transparent background
        background: Color.create(0.0, 0.0, 0.0, .5)
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {
        }
        Container {
            // Padding and maximum size
            maxWidth: 700
            rightPadding: 100
            topPadding: 100
            leftPadding: rightPadding
            bottomPadding: topPadding
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: DockLayout {
            }
            
            // This Container contains the title and body of the dialog box.
            Container {
                topPadding: 5
                bottomPadding: 23
                leftPadding: 23
                background: Color.White
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                
                Label {
                    text: "Select variable"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    horizontalAlignment: HorizontalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                
                ListView {
                    dataModel: ArrayDataModel {
                        id: model
                    }
                    listItemComponents: [
                        ListItemComponent {
                            StandardListItem {
                                title: ListItem.name
                                description: ListItem.description
                            }
                        }
                    ]
                    onSelectionChanged: {
                        Qt.StatementPageParametersList[index].value = model.data(indexPath).name
                        buttonClose.clicked()
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.5
                    }
                }
                
                Button {
                    id: buttonClose
                    text: "Cancel"
                    onClicked: {
                        dialog.close()
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
            }  
        }  
    }  
}
