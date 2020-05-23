#include "commprovider.h"
#include "SerialComm.hpp"

CommProvider::CommProvider(QObject *parent) : QObject(parent)
{
    serial = new SerialComm();
    serial->subscribeToUpdates([this] (const State &status) { updateProperties(status); }); // Dugi's CPP black magic
}


void CommProvider::sendFullGameMessage(){
    serial->sendMessage(Command::FULL_GAME);
}

void CommProvider::sendSettingPinsMessage(){
    serial->sendMessage(Command::SETTING_PINS);
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
    return _points;
}
int CommProvider::rounds(){
    return _rounds;
}

int CommProvider::score(){
    return (int)_score;
}
QByteArray CommProvider::pins(){
    return _pins;
}

void CommProvider::setPoints(uint8_t points){
    if (_points != points) {
        _points = points;
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
void CommProvider::setPins(QByteArray pins){
    if (_pins != pins) {
        _pins = pins;
        emit pinsChanged();
    }

}
