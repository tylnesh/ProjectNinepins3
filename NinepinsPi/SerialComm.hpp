#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QDebug>

enum class Game: uint8_t {FULL_GAME = 1, PARTIAL_GAME = 2};
enum class Command : uint8_t {
   NO_MESSAGE = 0,
   FULL_GAME = 24,
   PARTIAL_GAME = 25,
   CHANGE_OK = 41,
   CHANGE_CANCEL = 42,
   CHANGE_CMD = 40,
   SETTING_PINS = 3,
   END_GAME = 50,
   GAFFE_CONFIRMED = 66,
   GAFFE_CANCELED = 65,
   CHECKSUM_NOT_MATCH = 6,
   FULL_GAME_MSG = 1,
   CHECK_GAFFE = 61,
   ACKNOWLEDGED = 43,
   REQ_REPEAT = 44,
   KNOT_CMD = 70
};

constexpr int PINCOUNT = 9;
static constexpr int messageLength = 24;

struct State;
union Message {
   struct {
      uint32_t _counter;
      uint8_t _wire;
      Command _cmd;
      std::array<uint8_t, PINCOUNT> _pins;
      uint8_t _points;
      uint8_t _rounds;
      uint16_t _score;
      uint32_t _checksum;

   };
   std::array<uint8_t, messageLength> _bytes;

   uint8_t checksum() const {
     int pinsTotal = 0;
     for (auto& it : _pins)
       pinsTotal += it;
     return _counter + _wire + int(_cmd) + pinsTotal + _points + _rounds + _score;
   }

   bool verifyChecksum() const {
      return (_checksum == checksum());
   }

   operator bool() const {
      return (_cmd == Command::NO_MESSAGE);
   }

   Message(const State& state, Command cmd);
   Message() {
      for (auto& it : _bytes) it = 0;
   }
};


//ARRAY INITIALISING TRICK
struct blankArray {
  template <typename T, size_t Size>
  operator std::array<T, Size>() const {
    std::array<T, Size> array;
    for (unsigned int i = 0; i < Size; i++)
      array[i] = 0;
    return array;
  }
};

struct State {
   std::array<uint8_t, PINCOUNT> toMAG = blankArray();
   std::array<uint8_t, PINCOUNT> pins = blankArray();

   mutable uint32_t counter = 0;

   uint8_t prevPoints = 0;
   uint8_t currentPoints = 0;
   uint8_t knotCounter = 0;
   uint8_t overloadCounter = 0;
   uint8_t pinsUpCounter = 0;
   uint8_t rounds = 0;
   uint16_t score = 0;

   bool noneFallen = true;
   bool isGaffe = false;

   Game currentGameType;

   void updatePoints() {
      for (size_t i = 0; i < pins.size(); i++) if (pins[i]) currentPoints++;
      if (currentGameType == Game::PARTIAL_GAME) {
        if (currentPoints < prevPoints)
            currentPoints = 0;
        else
            currentPoints = currentPoints - prevPoints;
        prevPoints = currentPoints;
      }
   }

   void operator=(const Message& msg) {
      pins =  msg._pins;
      currentPoints =  msg._points;
      rounds = msg._rounds;
      score = msg._score;
   }

//   static State& get() {
//	  static State instance;
//	  return instance;
//   }
//private:
//   State(const State&) = default;
//   State() = default;
//   void operator=(const State&) = delete;
};


// Class definition itself



class SerialComm : public QObject
{
    Q_OBJECT




public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();
    void subscribeToUpdates(std::function<void(const State& message)> callback);
    void sendMessage(Command command);
    State status() const;
    void updateState(const State& state);

public slots:
    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleReadyRead();

private:
    std::function<void(const State& message)> _updateCallback = std::function<void(const State& message)>();
    State _state;
    QSerialPort *_serial;
    QByteArray _serialReadData;

    void readMessage();

};

#endif // SERIALCOMM_H
