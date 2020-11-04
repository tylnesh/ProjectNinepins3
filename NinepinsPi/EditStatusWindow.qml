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
    signal changeState

    id: root
    width: gameWidth
    height: gameHeight

    minimumWidth: 1000
    minimumHeight: 600

    onChangeOK: {
    var pinStatus = pinRect.getPinStatus()
    CommProviderApi.CommProvider.setPins(pinStatus)
    }

    Rectangle {

        id: gameRect
        width: parent.width
        height: parent.height
        color: "white"

        ScorePanel{
            id: scorePanel
            anchors.top: parent.top
            anchors.right: parent.right
            isEditable: true
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
                    Layout.margins: 5

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
                    Layout.margins: 5

                    bgcolor: "lightgrey"
                    txtcolor: "black"
                    onClicked: {
                        root.changeOK()
                    }
                    text: "Úprava"
                }
            }
        }

        PinRectangle{
            id:pinRect
        isClickable: true
        }
    }
}
