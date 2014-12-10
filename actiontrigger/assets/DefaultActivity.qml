import bb.cascades 1.2

ActivityIndicator {
    preferredHeight: 90
    preferredWidth: 90
    verticalAlignment: VerticalAlignment.Center
    running: true
    onRunningChanged: {
        visible = running
    }
}