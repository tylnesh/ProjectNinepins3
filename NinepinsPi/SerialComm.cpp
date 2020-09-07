#include "SerialComm.hpp"
#include <QDebug>
#include <QEventLoop>
#include <QtSerialPort/QSerialPort>

constexpr int WIRE = 1;

Message::Message(const State& state, Command cmd) {
   _counter = state.counter++;
   _wire = WIRE;
   _cmd = cmd;
   _pins = state.pins;
   _points = state.currentPoints;
   _rounds = state.rounds;
   _score = state.score;
   _checksum = checksum();
}


SerialComm::SerialComm(QObject *parent)
    : QObject(parent)
{

    _serial = new QSerialPort();
    _serial->setPortName("/dev/ttyUSB0");
   // _serial->setPortName("/dev/pts/1");
    if (_serial->open(QIODevice::ReadWrite))
    {
        _serial->setBaudRate(QSerialPort::Baud57600);
        _serial->setDataBits(QSerialPort::Data8);
        _serial->setParity(QSerialPort::NoParity);
        _serial->setStopBits(QSerialPort::OneStop);
        _serial->setFlowControl(QSerialPort::NoFlowControl);

        connect(_serial, &QSerialPort::readyRead, this, &SerialComm::handleReadyRead);
        connect(_serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                this, &SerialComm::handleError);

    } else qDebug() << "RS485 error msg: " << _serial->errorString();



}

void SerialComm::handleReadyRead()
{
    _serialReadData.append(_serial->readAll());
    readMessage();
}


//void SerialComm::handleTimeout()
//{
//    if (serialReadData.isEmpty()) {
//    } else {

//        if (serialReadData.size() < statusLength)  return;
//        Status *received = reinterpret_cast<Status*>(serialReadData.data());

//        uint16_t tempChecksum =
//                std::accumulate(received->bytes.begin(), received->bytes.end() - sizeof(Status::checksum), uint16_t(0));


//        qDebug() << "Receiving msg...";
//        qDebug() << "Wire: " << received->wire;
//        qDebug() << "Cmd: " << received->cmd;
//        qDebug() << "Pins: "; for (size_t i = 0; i < PINS; i++) qDebug() << received->pins[i];
//        qDebug() << "Rounds: " << received->rounds;
//        qDebug() << "Score: " << received->score;
//        qDebug() << "Checksum: " << received->checksum;



//        if (received->checksum == tempChecksum)
//        {
//            if (received->cmd == uint8_t(currentGameType)) *msg = *received;
//            switch(msg->cmd){
//            case REQ_REPEAT:
//                requestedRepeat = true;
//                break;
//            case ACKNOWLEDGED:
//                waitingForAck = false;
//                break;
//            default: onSendScore(msg);
//            }
//        }
//        serialReadData = "";
//    }



//}

void SerialComm::handleError(QSerialPort::SerialPortError serialPortError)
{
    qDebug() << serialPortError;
}



SerialComm::~SerialComm(){

    _serial->close();
}

void SerialComm::subscribeToUpdates(std::function<void(const State& message)> callback) {
    _updateCallback = callback;
}
// v GUI ked vytvorim instanciu tejtto triedy ,tak zavolam prvykrat
// v konstruktore toto subscribe a do parametra dam lambda funkciu,
// ktora vola metodu na prekreslenie gui

State SerialComm::status() const {
    return _state; // calling a private constructor of class State blablabla
}

void SerialComm::updateState(const State& state) {
   sendMessage(Command::CHANGE_CMD);
   _state = state;
   sendMessage(Command::CHANGE_OK);

}

void SerialComm::readMessage() {
    Message msg;
    // TODO: read it
    if (_serialReadData.length() == uint8_t(msg._bytes.size())) {
        for (int i = 0; i < messageLength; i++){
            msg._bytes[i] = _serialReadData.at(i);
           // qDebug() << _serialReadData.at(i);
        }
    }

    _serialReadData = "";

    if (msg.verifyChecksum()) {
        _state = msg;
        _currentCmd = msg._cmd;
        _updateCallback(_state);


        if (msg._cmd == Command::FULL_GAME) qDebug() << "FULL_GAME command received";
        if (msg._cmd == Command::PARTIAL_GAME) qDebug() << "PARTIAL_GAME received";
        if (msg._cmd == Command::SETTING_PINS) qDebug() << "SETTING_PINS reveived";
        if (msg._cmd == Command::END_GAME) qDebug() << "END_GAME received";
        if (msg._cmd == Command::CHECKSUM_NOT_MATCH) qDebug() << "CHECKSUM_NOT_MATCH received";


        //if (msg._cmd == Command::ACKNOWLEDGED) qDebug() << "Received an ACK";
    } else {
        qDebug() << "Requesting repeat";
        sendMessage(Command::REQ_REPEAT);
    }
}

void SerialComm::sendMessage(Command command) {
    Message msg(_state, command);

    // TODO: send it

    Message rcvMsg;

       qDebug() <<"Transmitting receission!";
       QByteArray serialWriteData;

       for (unsigned long i = 0; i < msg._bytes.size(); i++){
           serialWriteData.append(msg._bytes.at(i));
       }

       _serial->write(serialWriteData, serialWriteData.length());



}

