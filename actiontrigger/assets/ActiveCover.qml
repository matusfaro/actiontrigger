import bb.cascades 1.2

Container {
    function init() {
        Qt._actiontrigger = _actiontrigger
        Qt._app = _app
        Qt._coverData = Qt._actiontrigger.getGlobalResources()

        Qt._coverData.coverTitleChangeRequest.connect(updateTitle)
        Qt._coverData.coverContentChangeRequest.connect(updateContent)

        updateTitle()
        updateContent()
    }
    function updateTitle() {
        console.log("Updating title: ", Qt._coverData.coverTitle)
        labelTitle.text = Qt._coverData.coverTitle
    }
    function updateContent() {
        console.log("Updating content: ", Qt._coverData.coverContent)
        labelContent.text = Qt._coverData.coverContent
    }
    onCreationCompleted: {
        console.log("ActiveCover.qml onCreationCompleted")
        init()
    }
    layout: DockLayout {
    }
//    background: Color.create("#121212")
        ImageView {
            imageSource: "asset:///logo/background1920x1186.png"
        }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        // Title
        Label {
            id: labelTitle
            text: "ActionTrigger"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.color: Color.create("#ebebeb")
            textStyle.fontSize: FontSize.PointValue
            textStyle.fontSizeValue: 6
        }
        // Content
        Label {
            id: labelContent
            
            text: "a
b
c
d
e
f
g
h
i
j
k
l
m
n
o
p
q
r
s
t
u
v
w
x
y
z"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textStyle.color: Color.create("#ebebeb")
            textStyle.fontSize: FontSize.PointValue
            textStyle.fontSizeValue: 6
            multiline: true
        }
    }
}