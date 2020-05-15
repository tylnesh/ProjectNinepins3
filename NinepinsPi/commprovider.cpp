#include "commprovider.h"
#include "SerialComm.hpp"

CommProvider::CommProvider(QObject *parent) : QObject(parent)
{
    serial->subscribeToUpdates([this] (const State &status) { updateProperties(status); }); // Dugi's CPP black magic
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
