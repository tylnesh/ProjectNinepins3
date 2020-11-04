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


function getPinStatus() {
    var pinStatus= [false,false,false,false,false,false,false,false,false];
    pinStatus[0] = pin0.isDown;
    pinStatus[1] = pin1.isDown;
    pinStatus[2] = pin2.isDown;
    pinStatus[3] = pin3.isDown;
    pinStatus[4] = pin4.isDown;
    pinStatus[5] = pin5.isDown;
    pinStatus[6] = pin6.isDown;
    pinStatus[7] = pin7.isDown;
    pinStatus[8] = pin8.isDown;

    return pinStatus;
}

function readPinStatus(i) {
    //return new Uint8Array(CommProviderApi.CommProvider.pins)[i]
    return CommProviderApi.CommProvider.pins[i]
}

function changePinStatus(i) {

    console.log("changePinStatus" + i)

    console.log(CommProviderApi.CommProvider.pins[i])

    //CommProviderApi.CommProvider.pin[i] = !CommProviderApi.CommProvider.pins[i]

    return !CommProviderApi.CommProvider.pins[i]


    //if ((new Uint8Array(CommProviderApi.CommProvider.pins)[i]) === true)
     //   new Uint8Array(CommProviderApi.CommProvider.pins)[i] = false
    //else
     //   new Uint8Array(CommProviderApi.CommProvider.pins)[i] = true
    //return new Uint8Array(CommProviderApi.CommProvider.pins)[i]
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

                if (isClickable) {
                isDown = changePinStatus(pinNum)
                }

               // isClickable ? {isDown = changePinStatus(pinNum)} : {}
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
