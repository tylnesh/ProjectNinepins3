import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi

Item {
    property int panelHeight:  isEditable ? 50 : 100
    property bool isEditable: false

    Rectangle {
        id: labelRect
        anchors.right:  parent.right
        anchors.top: parent.top
        width: 300
        height: isEditable ? panelHeight*2 : panelHeight
        color: "green"
        border.color:"black"
        Text {
            id: labelText
            anchors.centerIn: parent
            color: "white"
            font.pointSize: 22
            text: root.type
        }
    }

    Rectangle{
        id: roundsRect
        color: "lightgrey"
        border.color:"black"
        width: labelRect.width / 2
        height: panelHeight
        anchors.right: pointsRect.left
        anchors.top: parent.top


        Text {
            id: roundsText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.rounds
        }

        CustomButton {
            text: "+"
            anchors.top: parent.bottom
            anchors.left: parent.left

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.rounds++)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.rounds--)
            }
        }

    }



    Rectangle{
        id: pointsRect
        color: "lightgrey"
        border.color:"black"
        width: roundsRect.width
        height: panelHeight
        anchors.right: scoreRect.left
        anchors.top: parent.top

        Text {
            id: pointsText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.points
        }

        CustomButton {
            text: "+"
            anchors.top: parent.bottom
            anchors.left: parent.left

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.points++)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.points--)
            }
        }
    }

    Rectangle{
        id: scoreRect
        color: "lightgrey"
        border.color:"black"
        width: pointsRect.width
        height: panelHeight
        anchors.right: labelRect.left
        anchors.top: parent.top


        Text {
            id: scoreText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.score
        }

        CustomButton {
            text: "+"
            anchors.top: parent.bottom
            anchors.left: parent.left

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.score++)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height/2
            width: parent.width/2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                CommProviderApi.CommProvider.setRounds(CommProviderApi.CommProvider.score--)
            }
        }
    }        }

