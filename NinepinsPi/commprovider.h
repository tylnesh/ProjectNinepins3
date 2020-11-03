#ifndef COMMPROVIDER_H
#define COMMPROVIDER_H
#include "SerialComm.hpp"

#include <QObject>
#include <QByteArray>

struct State;


class CommProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(int rounds READ rounds WRITE setRounds NOTIFY roundsChanged)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(QVector<bool> pins READ pins WRITE setPins NOTIFY pinsChanged)

public:
    explicit CommProvider(QObject *parent = nullptr);

    Q_INVOKABLE void sendFullGameMessage();
    Q_INVOKABLE void sendSettingPinsMessage();
    Q_INVOKABLE void sendEndGameMessage();

    int points();
    int rounds();
    int score();
    QVector<bool> pins();

    Q_INVOKABLE void setPoints(uint8_t points);
    Q_INVOKABLE void setRounds(uint8_t rounds);
    Q_INVOKABLE void setScore(uint16_t score);
    Q_INVOKABLE void setPins(QVector<bool> pins);

signals:

    void pointsChanged();
    void roundsChanged();
    void scoreChanged();
    void pinsChanged();


public slots:

private:

    void updateProperties(const State &status);

    SerialComm *serial;


    uint16_t _score = 0;
    uint8_t _rounds = 0;
    uint8_t _points = 0;
    QVector<bool> _pins = {1,1,1,1,1,1,1,1,1};



};

#endif // COMMPROVIDER_H
