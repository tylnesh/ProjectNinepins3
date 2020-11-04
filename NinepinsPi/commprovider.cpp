#include "commprovider.h"
#include "SerialComm.hpp"
#include <QDebug>

CommProvider::CommProvider(QObject *parent) : QObject(parent)
{
    serial = new SerialComm();
    serial->subscribeToUpdates([this] (const State &status) { updateProperties(status); }); // Dugi's CPP black magic

    _pins = QVector<bool>(9);
    _pins.fill(false);


}


void CommProvider::sendFullGameMessage(){
    serial->sendMessage(Command::FULL_GAME);
}

void CommProvider::sendSettingPinsMessage(){
    serial->sendMessage(Command::SETTING_PINS);
}

void CommProvider::sendEndGameMessage(){
    serial->sendMessage(Command::END_GAME);

}


void CommProvider::updateProperties(const State &status){

   //_pins  = status.pins;
   for (unsigned int i = 0; i < status.pins.size(); i++){
       _pins[i] = int8_t(status.pins.at(i));
   }

   _score = status.score;
   _rounds = status.rounds;
   _points = status.currentPoints;
}


int CommProvider::points(){
    return (int) _points;
}
int CommProvider::rounds(){
    return (int) _rounds;
}

int CommProvider::score(){
    return (int) _score;
}
QVector<bool> CommProvider::pins(){
    return _pins;
}

void CommProvider::setPoints(uint8_t points){
    if (_points != points) {
        _points = points;
            qDebug() << "setting points with setPoints function";
        emit pointsChanged();
    }
}

void CommProvider::setRounds(uint8_t rounds){
    if (_rounds != rounds) {
        _rounds = rounds;
        emit roundsChanged();
    }
}
void CommProvider::setScore(uint16_t score){
    if (_score != score) {
        _score = score;
        emit scoreChanged();
    }
}
void CommProvider::setPins(QVector<bool> pins){



    if (_pins != pins) {
        _pins = pins;
        bool pinBools[9];

        for (int i = 0; i<9;i++) {
            pinBools[i] = pins.at(i);
            qDebug() << "pin" << i << " pinBools" << pinBools[i] << " " << "QML pins " << pins.at(i);
        }
        emit pinsChanged();
    }
    }


