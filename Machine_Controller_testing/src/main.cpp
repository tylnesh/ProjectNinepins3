#include <Arduino.h>
#undef min
#undef max
#include <SPI.h>
#include <stdint.h>
#include <array>

//SERIAL COMMINICATION
UARTClass &Log = Serial;
UARTClass &Comm = Serial1;
#define DEBUG true

//WIRE
constexpr int WIRE = 1;

//RS485 SETTINGS
constexpr int TXControl = 53;
constexpr int COMM_DELAY = 50;

//MACHINE PINS
constexpr int PINCOUNT = 9;

constexpr int PXSENSOR[9] = {52, 50, 48, 46, 44, 42, 40, 38, 36};
constexpr int PXSENSOR_UP = 34;
constexpr int PXSENSOR_DOWN = 32;
constexpr int PXSENSOR_OVERLOAD = 30;

constexpr int MAG[9] = {51, 49, 47, 45, 43, 41, 39, 37, 35};

constexpr int GATE_RAMP = 26;
constexpr int ENGINE_RIGHT = 31;
constexpr int ENGINE_LEFT = 29;
constexpr int ENGINE_SLOW = 27;

//LED PINS
constexpr int BRIGHTNESS = 80;
static std::array<int, 12> LED = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
constexpr int LED_START = PINCOUNT + 0;
constexpr int LED_ERROR = PINCOUNT + 1;
constexpr int LED_YELLOW = PINCOUNT + 2;

//ENGINE SETTINGS
constexpr int OVERLOAD = 300;
constexpr int ENGINE_DOWN = 500;
constexpr int ENGINE_SLOWDOWN = 1900;

constexpr int LIGHT_TIME = 55;
constexpr int KNOT_THRESHOLD = 8;
constexpr int PINS_UP_THRESHOLD = 5;
constexpr int PINS_UP_TIME = 60000;
constexpr int PINS_WAIT_TIME = 1800;
constexpr int TIME_TO_SETTLE_TRESHOLD = 100000;
constexpr int TIME_TO_COUNT_POINTS_TRESHOLD = 4000;

//ARRAY INITIALISING TRICK
struct blankArray
{
  template <typename T, size_t Size>
  operator std::array<T, Size>() const
  {
    std::array<T, Size> array;
    for (unsigned int i = 0; i < Size; i++)
      array[i] = 0;
    return array;
  }
};

//STATE VARIABLES
enum class Game : uint8_t
{
  FULL_GAME = 1,
  PARTIAL_GAME = 2
};

enum class Command : uint8_t
{
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

static constexpr int messageLength = 24;

struct State;

union Message
{
  struct
  {
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

  uint8_t checksum() const
  {
    int pinsTotal = 0;
    for (auto &it : _pins)
      pinsTotal += it;
    return _counter + _wire + int(_cmd) + pinsTotal + _points + _rounds + _score;
  }

  bool verifyChecksum() const
  {
    return (_checksum == checksum());
  }

  operator bool() const
  {
    return (_cmd == Command::NO_MESSAGE);
  }

  Message(const State &state, Command cmd);
  Message()
  {
    for (auto &it : _bytes)
      it = 0;
  }
};

struct State
{
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

  void updatePoints()
  {
    for (size_t i = 0; i < pins.size(); i++)
      if (pins[i])
        currentPoints++;
    if (currentGameType == Game::PARTIAL_GAME)
    {
      currentPoints = currentPoints - prevPoints;
      if (currentPoints < 0)
        currentPoints = 0;
      prevPoints = currentPoints;
    }
  }

  void operator=(const Message &msg)
  {
    pins = msg._pins;
    currentPoints = msg._points;
    rounds = msg._rounds;
    score = msg._score;
  }

  static State &get()
  {
    static State instance;
    return instance;
  }

private:
  State(const State &) = default;
  State() = default;
  void operator=(const State &) = delete;
};

//FORWARD DECLARATION
void sendMessage();
void startGame();
void debugPrintln(String str);
void startReceiving();
void startTransmitting();
bool checkPins();
void resetPinCount();
void readMsg(const Message &msg);
void settingPins(Game gameType);
void sendMessage(Command cmd);
void lightLed(uint8_t led, bool b);
void debugPrintln(String str);
bool checkGaffe();
bool respondToChangeStateCommand();
bool checkGaffe();




// Communications

void startReceiving()
{
  analogWrite(TXControl, 0); //Receiving
}

void startTransmitting()
{
  analogWrite(TXControl, 255); //Transmitting
}

Message::Message(const State &state, Command cmd)
{
  _counter = state.counter++;
  _wire = WIRE;
  _cmd = cmd;
  _pins = state.pins;
  _points = state.currentPoints;
  _rounds = state.rounds;
  _score = state.score;
  _checksum = checksum();
}

Message receiveMessage()
{
  startReceiving();
  Message msg;
  while (Comm.available())
  {
    debugPrintln("Receiving transmission!");
    Comm.readBytes(&(msg._bytes[0]), msg._bytes.size());
    if (msg._wire == WIRE && msg.verifyChecksum())
    {
      //sendMessage(Command::ACKNOWLEDGED);
      return msg;
    }
  }
  msg._cmd = Command::CHECKSUM_NOT_MATCH;
  return msg;
}

void sendMessage(Command cmd)
{
  Message msg(State::get(), cmd);
  Message rcvMsg;
  // do {
  startTransmitting();
  debugPrintln("Transmitting receission!");
  Comm.write(&(msg._bytes[0]), msg._bytes.size());
  delay(COMM_DELAY);
  //rcvMsg = receiveMessage();
  //} while (rcvMsg._cmd != Command::ACKNOWLEDGED); // TODO
}

void readMsg(const Message &msg)
{

  debugPrintln("Reading Status");
  switch (msg._cmd)
  {

  case Command::CHANGE_CMD:
  {
    debugPrintln("READ MSG: CHANGE_CMD");
    lightLed(LED_START, false);
    if (respondToChangeStateCommand())
    {
      lightLed(LED_ERROR, true);
      settingPins(State::get().currentGameType);
    }
    lightLed(LED_START, true);
    break;
  }

  case Command::SETTING_PINS:
  {
    debugPrintln("READ MSG: SETTING_PINS");
    settingPins(State::get().currentGameType);
    //TODO: isSettingPins solve

    delay(100);
    break;
  }

  case Command::CHECK_GAFFE:
  {
    debugPrintln("READ MSG: CHECK_GAFFE");
    lightLed(LED_START, false);
    lightLed(LED_ERROR, true);
    debugPrintln("The gaffe button has been pressed");
    State::get().isGaffe = checkGaffe();
    break;
  }

  case Command::END_GAME:
  {
    debugPrintln("READ MSG: END_GAME");
    if (State::get().currentGameType == Game::PARTIAL_GAME)
      State::get().score = 0;
    startGame();
    break;
  }

  default:
  {
    break;
  }
  }
}

// Debugging output
void debugPrintln(String str)
{
  if (DEBUG)
    Log.println(str);
}

void debugPrint(String str)
{
  if (DEBUG)
    Log.print(str);
}

// LED control
void lightLed(uint8_t led, bool b)
{
  if (b)
    analogWrite(LED[led], BRIGHTNESS);
  else
    analogWrite(LED[led], 0);
}

void lightAllLed(bool b)
{
  for (unsigned int i = 0; i < LED.size(); i++)
    lightLed(i, b);
}

void checkLED()
{
  debugPrintln("************CHECKING LEDS*************");
  for (unsigned int i = 0; i < LED.size(); i++)
  {
    lightLed(i, true);
    delay((i < PINCOUNT) ? LIGHT_TIME : LIGHT_TIME * 4);
    lightLed(i, false);
  }
}

void showCircle()
{
  debugPrintln("Animating circle of LEDs");
  for (int i = 0; i < PINCOUNT; i++)
  {
    if (State::get().pins[i] > 0)
      lightLed(i, true);
  }

  static std::array<uint8_t, 8> circle = {0, 1, 3, 6, 8, 7, 5, 2};

  for (auto it : circle)
  {
    lightLed(it, true);
    delay(LIGHT_TIME);
    lightLed(it, false);
  }

  for (auto it : circle)
    lightLed(it, true);
}

void blinkPinLeds()
{
  //Lights up all the pin leds

  debugPrintln("Let there be light");
  for (auto it : LED)
    lightLed(it, true);
  delay(2 * LIGHT_TIME);
  for (auto it : LED)
    lightLed(it, false);
  delay(2 * LIGHT_TIME);
  for (auto it : LED)
    lightLed(it, true);
}

void showFallenPins()
{

  /*  
Lights up leds corresponding to fallen pins. 
If the fallen pins form a circle and the game type is "Full Game", 
it displays an animated circle. If the player downs all the pins in one go,
it blinks several times.
*/
  uint8_t fallenCounter = 0;

  for (int i = 0; i < PINCOUNT; i++)
  {
    if (State::get().pins[i] > 0)
    {
      State::get().pins[i] = 1;
      fallenCounter++;
      lightLed(i, true);
      State::get().toMAG[i] = 1;
    }
    else
      lightLed(i, false);
  }

  if ((fallenCounter == 8) && (State::get().pins[4] == 0) && (State::get().currentGameType == Game::FULL_GAME))
  {
    showCircle();
  }

  if (fallenCounter == PINCOUNT)
  {
    blinkPinLeds();
  }
}

// Machine control
void stopEngines()
{
  digitalWrite(ENGINE_RIGHT, LOW);
  digitalWrite(ENGINE_SLOW, LOW);
  digitalWrite(ENGINE_LEFT, LOW);
}

void untieKnot()
{
  sendMessage(Command::KNOT_CMD); // Send Status that Arduino is listening
  debugPrintln("If the pins are untied, confirm it");
  while (true)
  {
    if (Message msg = receiveMessage())
    {
      debugPrintln("UNTIE KNOT: RECEIVED MESSAGE");
      if (msg._cmd == Command::SETTING_PINS)
        break;
    }
  }
}

void fixOverload()
{
  State::get().knotCounter++;
  debugPrint("knot counter: ");
  debugPrintln((String)State::get().knotCounter);
  stopEngines();
  delay(1000);
  digitalWrite(ENGINE_LEFT, HIGH);
  if (State::get().knotCounter == 3 || State::get().knotCounter == 6)
  {
    delay(2000);
  }
  else
  {
    delay(800);
  }

  if (State::get().knotCounter == 8)
  {
    stopEngines();
    debugPrintln("COME UNTIE MY KNOTS");
    untieKnot();
    State::get().overloadCounter = 0;
    State::get().knotCounter = 0;
  }

  digitalWrite(ENGINE_LEFT, LOW);
  delay(400);
  State::get().overloadCounter = 0;
}

bool respondToChangeStateCommand()
{
  debugPrintln("***********CHANGING STATE***********");
  while (true)
  {
    Message msg = receiveMessage();
    if (msg)
    {
      debugPrintln("CHANGING STATE: RECEIVED MESSAGE");
    }
    if (msg._cmd == Command::CHANGE_OK)
    {
      debugPrintln("***********CHANGING STATE = OK***********");
      State::get() = msg;
      showFallenPins();
      lightLed(LED_ERROR, false);
      return true;
    }

    if (msg._cmd == Command::CHANGE_CANCEL)
    {
      debugPrintln("***********CHANGING STATE = CANCEL***********");

      lightLed(LED_ERROR, false);
      return false;
    }
  }
}

void lowerPins()
{
  //Lowering the pins
  digitalWrite(ENGINE_RIGHT, HIGH);
  delay(ENGINE_DOWN);
  digitalWrite(ENGINE_SLOW, HIGH);
  delay(ENGINE_SLOWDOWN);
  digitalWrite(ENGINE_SLOW, LOW);
}

bool checkSettingPins()
{
  //checking if the pins don't fall while setting down - returns true if all are standing, false if at least one has fallen

  int countStandingPins = 0;
  for (int i = 0; i < PINCOUNT; i++)
  {
    if (digitalRead(PXSENSOR[i]) == LOW)
    {
      countStandingPins++;
    }
  }
  if (countStandingPins == PINCOUNT)
    return true;
  else
    return false;
}

bool checkPins()
{

  //checking which pins have fallen - returns true if all are standing, false if at least one has fallen

  if (Message msg = receiveMessage())
  {
    debugPrintln("CHECK PINS: RECEIVED MSG");
    readMsg(msg);
  }

  int countStandingPins = 0;
  for (int i = 0; i < PINCOUNT; i++)
  {
    if (digitalRead(PXSENSOR[i]) == LOW)
    {
      countStandingPins++;
    }
    else
    {
      State::get().pins[i] = true;
    }
  }
  if (countStandingPins == PINCOUNT)
    return true;
  else
    return false;
}

// Game control
bool checkGaffe()
{
  debugPrintln("Checking whether the ball hit the gutter");
  auto tempPins = State::get().pins;
  lightLed(LED_ERROR, true);

  while (true)
  {
    checkPins();
    showFallenPins();
    if (Message msg = receiveMessage())
    {
      switch (msg._cmd)
      {
      case Command::GAFFE_CANCELED:
        debugPrintln("No gaffe");
        lightLed(LED_START, false);
        lightLed(LED_YELLOW, false);
        lightLed(LED_ERROR, false);
        return false;

      case Command::GAFFE_CONFIRMED:
        debugPrintln("gaffe confirmed");
        if (State::get().currentGameType == Game::FULL_GAME)
        {
          debugPrintln("FULL GAME: resetting pin count");
          resetPinCount();
          lightLed(LED_ERROR, true);
          return true;
        }
        else
        {
          debugPrintln("PARTIAL GAME: loading previous pins state");
          for (int i = 0; i < PINCOUNT; i++)
          {
            State::get().toMAG[i] = State::get().pins[i] = tempPins[i];
            lightLed(i, false);
          }
          lightLed(LED_ERROR, true);
          showFallenPins();
          return true;
        }

      default:
        break;
      }
    }
  }
}

void resetPinCount()
{
  //resetting pin count
  debugPrintln("************ZEROING THE PINS*************");
  for (int i = 0; i < PINCOUNT; i++)
    State::get().pins[i] = 0;
}

// Complicated procedures
void settingPins(Game gameType)
{
  debugPrintln(" Setting Pins procedure");
  lightLed(LED_START, false);
  debugPrintln(" Waiting for pins to come up ");

  State::get().pinsUpCounter = 0;

  while (State::get().pinsUpCounter < PINS_UP_THRESHOLD)
  {
    debugPrintln("Starting pinsUpCounter");
    uint32_t pinsUpTimer = millis();
    uint8_t upSensor = 0;
    bool pinsUp = false;

    while (millis() < (pinsUpTimer + PINS_UP_TIME))
    {
      digitalWrite(ENGINE_RIGHT, HIGH);
      //CHECKING IF THE ENGINES ARE BEING OVERLOADED
      if (digitalRead(PXSENSOR_OVERLOAD) == HIGH)
      {
        State::get().overloadCounter++;
        debugPrintln("OVERLOAD");
      }

      //CHECKING IF THE OVERLOAD HAS REACHED THE THRESHOLD
      if (State::get().overloadCounter > OVERLOAD)
      {
        debugPrintln("OVERLOAD REACHED");
        stopEngines();
        fixOverload();
        digitalWrite(ENGINE_RIGHT, HIGH);
      }

      //CHECKING IF THE PINS ARE KNOTTED UP
      if (State::get().knotCounter > KNOT_THRESHOLD)
      {
        stopEngines();
        Serial.print("COME UNTIE MY KNOTS");
        untieKnot();
        State::get().overloadCounter = 0;
        State::get().knotCounter = 0;
      }

      //CHECKING IF THE PINS ARE UP
      if (digitalRead(PXSENSOR_UP) == LOW)
        upSensor++;
      if (upSensor > 3)
      {
        stopEngines();
        debugPrintln("PINS ARE UP");
        pinsUp = true;
        break;
      }
    }

    //if pins are up, we set them down to the ground
    if (pinsUp)
    {
      stopEngines();
      delay(PINS_WAIT_TIME);

      if (gameType == Game::FULL_GAME)
      {
        resetPinCount();
        debugPrintln("LOWERING THE PINS");
        lowerPins();
      }

      if (gameType == Game::PARTIAL_GAME)
      {
        uint8_t MAGcounter = 0; // variable to store the number of magnetically locked pins
        for (int i = 0; i < PINCOUNT; i++)
        {
          MAGcounter += State::get().toMAG[i];
        }

        if (MAGcounter < PINCOUNT)
        {
          for (int i = 0; i < PINCOUNT; i++)
          {
            if (State::get().toMAG[i] > 0)
            {
              digitalWrite(MAG[i], HIGH);
            }
          }
          digitalWrite(ENGINE_RIGHT, HIGH);
          delay(ENGINE_DOWN);
          for (int i = 0; i < PINCOUNT; i++)
          {
            if (State::get().toMAG[i] > 0)
            {
              digitalWrite(MAG[i], LOW);
            }
          }
          digitalWrite(ENGINE_SLOW, HIGH);
          delay(ENGINE_SLOWDOWN);
          digitalWrite(ENGINE_SLOW, LOW);
        }

        else
        {
          for (int i = 0; i < PINCOUNT; i++)
          {
            digitalWrite(MAG[i], LOW);
            State::get().toMAG[i] = 0;
            State::get().pins[i] = 0;
          }
          lightAllLed(false);
          lowerPins();
        }
      }
    }

    uint32_t pinsSettleTime = millis();
    while (millis() < pinsSettleTime + TIME_TO_SETTLE_TRESHOLD)
    {
      if (digitalRead(PXSENSOR_DOWN) == LOW)
      {
        stopEngines();
        if (!checkSettingPins())
          settingPins(gameType);
        lightLed(LED_START, true);
        return;
      }
    }
  }
}

// Game logic
void game(Game gType)
{
  State::get().currentGameType = gType;
  lightLed(LED_ERROR, false);
  if (State::get().currentGameType == Game::FULL_GAME)
  {
    debugPrintln("FULL GAME");
    State::get().rounds = 0;
    State::get().score = 0;
  }

  if (State::get().currentGameType == Game::PARTIAL_GAME)
  {
    debugPrintln("PARTIAL GAME");
    State::get().rounds = 0; //score only zeores in full game, as partial game is a continuation of the full game.
  }

  lightAllLed(false);

  while (true)
  {
    State::get().isGaffe = false;
    State::get().noneFallen = true;
    State::get().isGaffe = false;
    int gateSensorCounter = 0;
    lightLed(LED_ERROR, false);
    if (State::get().currentGameType == Game::FULL_GAME)
    {
      resetPinCount(); // pin count doesn not reset between rounds in partialgame
      lightAllLed(false);
    }
    lightLed(LED_START, true);

    //Waiting for the ball to go through the gate, or command from the user
    while (gateSensorCounter < 10 && !State::get().isGaffe)
    {
      if (Message msg = receiveMessage())
        readMsg(msg);
      if (digitalRead(GATE_RAMP) == HIGH)
        gateSensorCounter++;
    }

    lightLed(LED_START, false);

    State::get().rounds++;
    debugPrintln("Waiting 4 seconds to count points");
    uint32_t timeToCountPoints = millis();
    while ((millis() < timeToCountPoints + TIME_TO_COUNT_POINTS_TRESHOLD) && !State::get().isGaffe)
    {
      State::get().noneFallen = checkPins();
      showFallenPins();
    }

    //if the ball didn't touch the sides, count points
    if (!State::get().isGaffe)
      State::get().updatePoints();
    sendMessage(State::get().currentGameType == Game::FULL_GAME ? Command::FULL_GAME : Command::PARTIAL_GAME);

    if (State::get().currentGameType == Game::FULL_GAME)
    {
      State::get().pins = {0, 0, 0, 0, 0, 0, 0, 0, 0};

      //if at least one pin has fallen, run settingPins function
      if (!State::get().noneFallen)
      {
        settingPins(State::get().currentGameType);
      }
    }
  }
}

void startGame()
{

  checkLED();
  if (State::get().currentGameType == Game::PARTIAL_GAME)
    settingPins(Game::FULL_GAME);
  lightLed(LED_ERROR, true);
  lightLed(LED_START, true);
  debugPrintln("Project Ninepins - wired version");
  while (true)
  {
    Message msg = receiveMessage();
    switch (msg._cmd)
    {
    case Command::FULL_GAME:
      debugPrintln("Starting Full Game");
      game(Game::FULL_GAME);
      break;
    case Command::PARTIAL_GAME:
      debugPrintln("Starting Partial Game");
      game(Game::PARTIAL_GAME);
      break;
    default:
      break;
    }
  }
}


// Mandatory arduino functions

void setup() {

  Log.begin(9600);
  Comm.begin(57600);

  startReceiving();
  
  //Setting PXSENSORS pins to INPUT
  for(int i = 0; i < PINCOUNT; i++) pinMode(PXSENSOR[i], INPUT);
  pinMode(PXSENSOR_UP, INPUT);
  pinMode(PXSENSOR_DOWN, INPUT);
  pinMode(PXSENSOR_OVERLOAD, INPUT);

  pinMode(GATE_RAMP, INPUT);

  //Setting locking MAGNET pins to OUTPUT

  for(int i = 0; i < PINCOUNT; i++) pinMode(MAG[i], OUTPUT);

  //Setting ENGINE control pins to OUTPUT
  pinMode(ENGINE_RIGHT, OUTPUT);
  pinMode(ENGINE_LEFT, OUTPUT);
  pinMode(ENGINE_SLOW, OUTPUT);

  //Setting LED pins to OUTPUT 
  for(int i = 0; i < PINCOUNT; i++) pinMode(LED[i], OUTPUT);
  pinMode(LED_START, OUTPUT);
  pinMode(LED_ERROR, OUTPUT) ;

  lightAllLed(false);

  //Keeping ENGINE pins OFF
  digitalWrite(ENGINE_LEFT, LOW);
  digitalWrite(ENGINE_RIGHT, LOW);
  digitalWrite(ENGINE_SLOW, LOW);

  for (int i = 0; i < PINCOUNT; i++) digitalWrite(MAG[i], LOW);

  State::get().currentGameType = Game::FULL_GAME;


}
void loop() {
  checkLED();
  if (State::get().currentGameType == Game::FULL_GAME){
    settingPins(Game::FULL_GAME);
  } 
  lightLed(LED_START, true);
  lightLed(LED_ERROR, true);
  startGame();
  
} 
