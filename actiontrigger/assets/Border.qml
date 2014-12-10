import bb.cascades 1.2

// Border - use a container within to get around the dock layout
Container {
    property alias sizeLeft: lineLeft.preferredWidth
    property alias sizeRight: lineRight.preferredWidth
    property alias sizeTop: lineTop.preferredHeight
    property alias sizeBottom: lineBottom.preferredHeight
    property alias maxLengthLeft: lineLeft.maxHeight
    property alias maxLengthRight: lineRight.maxHeight
    property alias maxLengthTop: lineTop.maxWidth
    property alias maxLengthBottom: lineBottom.maxWidth
    layout: DockLayout {
    }
    Container {
        id: lineLeft
        background: Color.Gray
        preferredWidth: 0
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Left
    }
    Container {
        id: lineBottom
        background: Color.Gray
        preferredHeight: 0
        verticalAlignment: VerticalAlignment.Bottom
        horizontalAlignment: HorizontalAlignment.Fill
    }
    Container {
        id: lineRight
        background: Color.Gray
        preferredWidth: 0
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Right
    }
    Container {
        id: lineTop
        background: Color.Gray
        preferredHeight: 0
        verticalAlignment: VerticalAlignment.Top
        horizontalAlignment: HorizontalAlignment.Fill
    }
}
