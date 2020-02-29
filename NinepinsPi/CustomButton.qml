import QtQuick 2.0

    Rectangle {
        id: root
        property int txtsize: 22
        property color bgcolor: "white"
        property string text: "Button"
        property color txtcolor: "black"
        property int buttonWidth: text.length * 30


        signal clicked();

        radius: 90
        width: buttonWidth
        height: 3* txtsize
        color: root.bgcolor
        border.color: "black"
        opacity:      enabled  &&  !mouseArea.pressed? 1: 0.3 // disabled/pressed state

        Text {
            anchors.centerIn: parent
            color: root.txtcolor
            font.pointSize: txtsize
            text: root.text
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                root.clicked()
            }
        }
    }



