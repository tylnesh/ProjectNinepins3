import QtQuick 2.0

    Rectangle {
        id: root
        property color bgcolor: "lightgrey"
        property color downcolor: "yellow"
        property int pinWidth: 40
        property int pinHeight: pinWidth
        property bool isDown: false;

        signal clicked();

        radius: width * 0.5
        width: pinWidth
        height: 60
        color: isDown ? downcolor : bgcolor
        border.color: "black"
        opacity:      enabled  &&  !mouseArea.pressed? 1: 0.3 // disabled / pressed state

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                root.clicked()
            }
        }
    }




