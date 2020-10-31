import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi

Window {
    property string type: "Úprava stavu"
    property int gameWidth: 1280
    property int gameHeight: 800


    signal changeOK
    signal changeCancel
    signal settingPins

    id: root
    width: gameWidth
    height: gameHeight

    minimumWidth: 1000
    minimumHeight: 600

    Rectangle {

        id: gameRect
        width: parent.width
        height: parent.height
        color: "white"

        Rectangle {
            id: labelRect
            anchors.right: gameRect.right
            anchors.top: gameRect.top
            width: 250
            height: 100
            color: "green"
            Text {
                id: labelText
                anchors.centerIn: parent
                color: "white"
                font.pointSize: 22
                text: root.type
            }
        }

        Rectangle {
            id: roundsRect
            color: "lightgrey"
            width: labelRect.width / 2
            height: labelRect.height / 2
            anchors.right: pointsRect.left
            anchors.top: gameRect.top

            Text {
                id: rounds
                anchors.centerIn: parent
                font.pointSize: 22
                text: CommProviderApi.CommProvider.rounds
            }
        }

        Rectangle {
            id: pointsRect
            color: "lightgrey"
            width: roundsRect.width
            height: roundsRect.height
            anchors.right: scoreRect.left
            anchors.top: gameRect.top

            Text {
                id: points
                anchors.centerIn: parent
                font.pointSize: 22
                text: CommProviderApi.CommProvider.points
            }
        }

        Rectangle {
            id: scoreRect
            color: "lightgrey"
            width: pointsRect.width
            height: pointsRect.height
            anchors.right: labelRect.left
            anchors.top: gameRect.top

            Text {
                id: score
                anchors.centerIn: parent
                font.pointSize: 22
                text: CommProviderApi.CommProvider.score
            }
        }

        Rectangle {
            id: buttonRect
            width: gameRect.width * 1 / 3
            height: gameRect.height
            anchors.left: gameRect.left
            color: "lightgrey"

            ColumnLayout {
                anchors.centerIn: buttonRect
                CustomButton {

                    id: settingPinsButton
                    Layout.alignment: Layout.Center
                    bgcolor: "lightgrey"
                    txtcolor: "black"
                    onClicked: {
                        root.settingPins()
                    }
                    text: "Stavanie"
                }

                CustomButton {
                    id: changeStateButton
                    Layout.alignment: Layout.Center
                    bgcolor: "lightgrey"
                    txtcolor: "black"
                    onClicked: {

                    }
                    text: "Úprava"
                }
            }
        }

        PinRectangle{}
    }
}
