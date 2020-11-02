import QtQuick 2.0
import QtQuick.Layouts 1.12
import com.tylnesh.commprovider 1.0 as CommProviderApi


Rectangle{
id: pinRectangle
width: 2/3* parent.width
height: 2/3* parent.height
anchors.right: parent.right
anchors.bottom: parent.bottom


property bool isClickable: false
property var pinArray: [
    pin0.isDown,
    pin1.isDown,
    pin2.isDown,
    pin3.isDown,
    pin4.isDown,
    pin5.isDown,
    pin6.isDown,
    pin7.isDown,
    pin8.isDown
]

function readPinStatus(i) {
    return new Uint8Array(CommProviderApi.CommProvider.pins)[i]
}

function changePinStatus(i) {
    if ((new Uint8Array(CommProviderApi.CommProvider.pins)[i]) % 2)
        new Uint8Array(CommProviderApi.CommProvider.pins)[i] = 0
    else
        new Uint8Array(CommProviderApi.CommProvider.pins)[i] = 1
    return new Uint8Array(CommProviderApi.CommProvider.pins)[i]
}



color: "white"

    GridLayout{
        columns: 5
        anchors.centerIn: pinRectangle

        Rectangle{width:1}
        Rectangle{width:1}
        PinShape{
            id:pin0
            pinNum:0
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }
        }
        Rectangle{width:1}
        Rectangle{width:1}

        Rectangle{width:1}
        PinShape{
            id:pin1
            pinNum:1
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }        }
        Rectangle{width:1}
        PinShape{
            id:pin2
            pinNum:2
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }    }
        Rectangle{width:1}

        PinShape{
            id:pin3
            pinNum:3
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }        }
        Rectangle{width:1}
        PinShape{
            id:pin4
            pinNum:4
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }        }
        Rectangle{width:1}
        PinShape{
            id:pin5
            pinNum:5
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }        }

        Rectangle{width:1}
        PinShape{
            id:pin6
            pinNum:6
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }        }
        Rectangle{width:1}
        PinShape{
            id:pin7
            pinNum:7
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }            }
        Rectangle{width:1}

        Rectangle{width:1}
        Rectangle{width:1}
        PinShape{

            id:pin8
            pinNum:8
            isDown: readPinStatus(pinNum)
            onClicked: {
                isClickable ? {isDown = changePinStatus(pinNum)} : {}
            }
        }
        Rectangle{width:1}
        Rectangle{width:1}

    }
}
