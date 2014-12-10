import bb.cascades 1.2

Container {
    property alias color: root.background
    property alias lineopacity: root.opacity
    property alias size: root.leftPadding
    layout: DockLayout {
    }
    Container {
        id: root
        background: Color.Gray
        leftPadding: 3
        verticalAlignment: VerticalAlignment.Fill
    }
}
