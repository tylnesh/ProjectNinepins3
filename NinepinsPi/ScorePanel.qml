import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi

Item {
    Rectangle {
        id: labelRect
        anchors.right:  parent.right
        anchors.top: parent.top
        width: 200
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

    Rectangle{
        id: roundsRect
        color: "lightgrey"
        width: labelRect.width / 2
        height: labelRect.height
        anchors.right: pointsRect.left
        anchors.top: parent.top


        Text {
            id: roundsText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.rounds
        }
    }

    Rectangle{
        id: pointsRect
        color: "lightgrey"
        width: roundsRect.width
        height: roundsRect.height
        anchors.right: scoreRect.left
        anchors.top: parent.top

        Text {
            id: pointsText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.points
        }
    }

    Rectangle{
        id: scoreRect
        color: "lightgrey"
        width: pointsRect.width
        height: pointsRect.height
        anchors.right: labelRect.left
        anchors.top: parent.top


        Text {
            id: scoreText
            anchors.centerIn: parent
            font.pointSize: 22
            text: CommProviderApi.CommProvider.score
        }
    }        }

