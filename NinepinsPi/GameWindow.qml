import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi



Window{
    property string type: "Plná hra"
    property int gameWidth : 1280
    property int gameHeight : 800

    signal settingPins();
    signal endGame();

    id: root
    width: gameWidth
    height: gameHeight

    minimumWidth: 800
    minimumHeight: 600

    EditStatusWindow {
        id: editStatusWindow
        visible: false

        onChangeOK: {
            editStatusWindow.setVisible(false)
            root.setVisible(true)


        }
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
        }

        Rectangle {
            id: buttonRect
            width: gameRect.width * 1 / 3
            height: gameRect.height
            anchors.left: gameRect.left

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
                        editStatusWindow.setVisible(true);
                        root.setVisible(false);
                    }
                    text: "Úprava"
                }

                CustomButton {
                    id: gaffeButton
                    Layout.alignment: Layout.Center
                    Layout.margins: 5

                    bgcolor: "red"
                    txtcolor: "white"
                    onClicked: {

                    }
                    text: "Chyba"
                }

                CustomButton {
                    id: endButton
                    Layout.alignment: Layout.Center
                    Layout.margins: 5

                    bgcolor: "lightgrey"
                    txtcolor: "black"
                    onClicked: {
                        mainWindow.visible = true
                        root.visible = false
                        root.endGame()

                    }
                    text: "Koniec"
                }
            }
        }

        PinRectangle{}




    }
}
