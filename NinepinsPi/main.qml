import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import com.tylnesh.serialcomm 1.0

Window {
    id: mainWindow
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("NinepinsPi")

    SerialComm {
    id: serial
    }



    GameWindow {
    id: gameWindow
    gameWidth: mainWindow.width
    gameHeight: mainWindow.height

        onSettingPins: {
            console.log("Setting pins")
        }
    }

    Rectangle {
        id: buttonRect
        width: mainWindow.width * 1 / 3
        height: mainWindow.height
        anchors.left: mainWindow.left
        color: "lightgrey"

        ColumnLayout {
            anchors.centerIn: buttonRect
            CustomButton {

                id: fullgameButton
                Layout.alignment: Layout.Center
                bgcolor: "green"
                txtcolor: "white"

                //txtsize: txtsize * (gameWindow.width / gameWindow.minimumWidth)
               onClicked: {
                    gameWindow.type = fullgameButton.text
                    gameWindow.visible = true
                    mainWindow.visible = false

                }
                text: "Plná hra"
            }

            CustomButton {
                id: partialgameButton
                Layout.alignment: Layout.Center
                bgcolor: "green"
                txtcolor: "white"
                onClicked: {
                    gameWindow.type = partialgameButton.text
                    gameWindow.visible = true
                    mainWindow.visible = false
                }
                text: "Dorážka"
            }
        }
    }

    Rectangle {
        id: logoRect
        anchors.left: buttonRect.right
        width: mainWindow.width - buttonRect.width
        height: mainWindow.height

        Image {
            source: "images/logo"
            id: image1
            anchors.centerIn: logoRect
        }
    }
}
