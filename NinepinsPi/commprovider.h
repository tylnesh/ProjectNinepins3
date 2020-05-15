#ifndef COMMPROVIDER_H
#define COMMPROVIDER_H
#include "SerialComm.hpp"

#include <QObject>
#include <QByteArray>

struct State;


class CommProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint8_t points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(uint8_t rounds READ rounds WRITE setRounds NOTIFY roundsChanged)
    Q_PROPERTY(uint16_t score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(QByteArray pins READ pins WRITE setPins NOTIFY pinsChanged)

public:
    explicit CommProvider(QObject *parent = nullptr);

    Q_INVOKABLE void sendFullGameMessage();

    uint8_t points();
    uint8_t rounds();
    uint16_t score();
    QByteArray pins();

    void setPoints(uint8_t points);
    void setRounds(uint8_t rounds);
    void setScore(uint16_t score);
    void setPins(QByteArray pins);

signals:

    void pointsChanged();
    void roundsChanged();
    void scoreChanged();
    void pinsChanged();


public slots:

private:


    void updateProperties(const State &status);
    SerialComm *serial;
    uint16_t _score;
    uint8_t _rounds;
    uint8_t _points;
    QByteArray _pins;




};

#endif // COMMPROVIDER_H
