import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi

Window {
    id: mainWindow
    visible: true
    minimumWidth: 1280
    minimumHeight: 800
    title: qsTr("NinepinsPi")

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
                Layout.margins: 5

                bgcolor: "green"
                txtcolor: "white"
                onClicked: {
                    CommProviderApi.CommProvider.sendFullGameMessage()
                    gameWindow.type = fullgameButton.text
                    gameWindow.visible = true
                    mainWindow.visible = false
                }
                text: "Plná hra"
            }

            CustomButton {
                id: partialgameButton
                Layout.alignment: Layout.Center
                Layout.margins: 5

                bgcolor: "green"
                txtcolor: "white"
                onClicked: {
                    gameWindow.type = partialgameButton.text
                    gameWindow.visible = true
                    mainWindow.visible = false
                }
                text: "Dorážka"
            }

            CustomButton {
                id: practicemodeButton
                Layout.alignment: Layout.Center
                Layout.margins: 5

                bgcolor: "green"
                txtcolor: "white"
                onClicked: {
                    try {
                        CommProviderApi.CommProvider.sendPracticemodeMessage() // TODO: Implement sendPracticemodeMessage()
                    } catch (error) {}

                    gameWindow.type = practicemodeButton.text
                    gameWindow.visible = true
                    mainWindow.visible = false
                }
                text: "Tréning"
            }

            CustomButton {
                id: shutdownButton
                Layout.alignment: Layout.Center
                Layout.margins: 80

                bgcolor: "red"
                txtcolor: "white"
                onClicked: {
                    shutdown() //TODO: Implement shutdown - for debug purposes close window, for production shut down raspi
                }
                text: "Vypnúť"
            }
        }
    }


    // Right side of the main menu window - just the logo.
    Rectangle {
        id: logoRect
        anchors.left: buttonRect.right
        width: mainWindow.width - buttonRect.width
        height: mainWindow.height

        Image {
            id: image1
            source: "images/logo"
            anchors.centerIn: logoRect
        }
    }



    // Game window
    GameWindow {
        id: gameWindow
        gameWidth: mainWindow.width
        gameHeight: mainWindow.height

        onSettingPins: {
            console.log("Setting pins")
            CommProviderApi.CommProvider.sendSettingPinsMessage()
        }

        onEndGame: {
            console.log("Ending game")
            CommProviderApi.CommProvider.sendEndGameMessage()
        }
    }





}
