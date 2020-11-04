import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi

Item {
    property int panelHeight: isEditable ? 100 : 150
    property bool isEditable: false

    Rectangle {
        id: labelRect
        anchors.right: parent.right
        anchors.top: parent.top
        width: 300
        height: isEditable ? panelHeight * 3 / 2 : panelHeight
        color: "green"
        border.color: "black"
        Text {
            id: labelText
            anchors.centerIn: parent
            color: "white"
            font.pointSize: 22
            text: root.type
        }
    }

    /*************************************************************/
    /*****************Score label, numbers display****************/
    /***************and buttons that only show when***************/
    /*************the property isEditable is set to true**********/
    /*************************************************************/
    Rectangle {
        id: scoreRectLabel
        color: "black"
        border.color: "black"
        width: labelRect.width / 2
        height: panelHeight / 3
        anchors.right: labelRect.left
        anchors.top: parent.top

        Text {
            id: scoreRectLabelText
            anchors.centerIn: parent
            font.pointSize: 18
            color: "white"
            text: "Body"
        }
    }

    Rectangle {
        id: scoreRect
        color: "lightgrey"
        border.color: "black"
        width: pointsRect.width
        height: panelHeight * 2 / 3
        anchors.right: scoreRectLabel.right
        anchors.top: scoreRectLabel.bottom

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

            height: parent.height * 2 / 3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempScore = CommProviderApi.CommProvider.score
                tempScore < 9999 ? tempScore++ : tempScore
                CommProviderApi.CommProvider.setScore(tempScore)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height * 2 / 3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempScore = CommProviderApi.CommProvider.score
                tempScore > 0 ? tempScore-- : tempScore
                CommProviderApi.CommProvider.setScore(tempScore)
            }
        }
    }

    /*********************************************************/
    /***************** End of score label*********************/
    /*********************************************************/

    /*************************************************************/
    /*****************Points label, numbers display***************/
    /***************and buttons that only show when***************/
    /*************the property isEditable is set to true**********/
    /*************************************************************/
    Rectangle {
        id: pointsRectLabel
        color: "black"
        border.color: "black"
        width: labelRect.width / 2
        height: panelHeight / 3
        anchors.right: scoreRectLabel.left
        anchors.top: parent.top

        Text {
            id: pointsRectLabelText
            anchors.centerIn: parent
            font.pointSize: 18
            color: "white"
            text: "Kolky"
        }
    }

    Rectangle {
        id: pointsRect
        color: "lightgrey"
        border.color: "black"
        width: labelRect.width / 2
        height: panelHeight * 2 / 3
        anchors.right: scoreRect.left
        anchors.top: roundsRectLabel.bottom

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

            height: parent.height * 2 / 3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempPoints = CommProviderApi.CommProvider.points
                tempPoints < 9 ? tempPoints++ : tempPoints
                CommProviderApi.CommProvider.setPoints(tempPoints)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height * 2 / 3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempPoints = CommProviderApi.CommProvider.points

                tempPoints > 0 ? tempPoints-- : tempPoints

                CommProviderApi.CommProvider.setPoints(tempPoints)
            }
        }
    }

    /*********************************************************/
    /***************** End of pooints label*********************/
    /*********************************************************/

    /*************************************************************/
    /*****************Rounds label, numbers display***************/
    /***************and buttons that only show when***************/
    /*************the property isEditable is set to true**********/
    /*************************************************************/
    Rectangle {
        id: roundsRectLabel
        color: "black"
        border.color: "black"
        width: labelRect.width / 2
        height: panelHeight / 3
        anchors.right: pointsRect.left
        anchors.top: parent.top

        Text {
            id: roundsRectLabelText
            anchors.centerIn: parent
            font.pointSize: 18
            color: "white"
            text: "Hody"
        }
    }

    Rectangle {
        id: roundsRect
        color: "lightgrey"
        border.color: "black"
        width: labelRect.width / 2
        height: panelHeight * 2 / 3
        anchors.right: pointsRect.left
        anchors.top: roundsRectLabel.bottom

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

            height: parent.height * 2 / 3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempRounds = CommProviderApi.CommProvider.rounds
                tempRounds < 999 ? tempRounds++ : tempRounds
                CommProviderApi.CommProvider.setRounds(tempRounds)
            }
        }
        CustomButton {
            text: "-"
            anchors.top: parent.bottom
            anchors.right: parent.right

            height: parent.height * 2/3
            width: parent.width / 2

            bgcolor: "green"
            txtcolor: "white"

            visible: isEditable ? true : false
            onClicked: {
                var tempRounds = CommProviderApi.CommProvider.rounds
                tempRounds > 0 ? tempRounds-- : tempRounds
                CommProviderApi.CommProvider.setRounds(tempRounds)
            }
        }
    }
}
