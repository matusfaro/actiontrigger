import bb.cascades 1.2

// Indentation - use a container within to get around the layout orientation
Container {
    property alias size: paddingContainer.preferredWidth
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        id: paddingContainer
        preferredWidth: 30
    }
}
