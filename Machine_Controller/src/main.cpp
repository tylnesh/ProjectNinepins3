#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include <array>

//SERIAL COMMINICATION
UARTClass &Log =  Serial;
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
constexpr int LED_START = PINCOUNT + 1;
constexpr int LED_ERROR = PINCOUNT + 2;
constexpr int LED_YELLOW = PINCOUNT + 3;

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

//FORWARD DECLARATION
void sendMessage();
void startGame();
void debugPrintln(String str);
void startReceiving();
void startTransmitting();

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


//STATE VARIABLES

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
   CHECK_GUTTER = 61,
   ACKNOWLEDGED = 43,
   REQ_REPEAT = 44,
   KNOT_CMD = 70
};

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

struct State {
   std::array<uint8_t, PINCOUNT> toMAG = blankArray();
   std::array<uint8_t, PINCOUNT> pins = blankArray();
   
   mutable uint32_t counter = 0;

   uint8_t prevPoints = 0;
   uint8_t currentPoints = 0;
   uint8_t knotCounter = 0;
   uint8_t overloadCounter = 0;
   uint8_t rounds = 0;
   uint16_t score = 0;

   bool noneFallen = true;

   Game currentGameType;

   void updatePoints() {
      for (size_t i = 0; i < pins.size(); i++) if (pins[i]) currentPoints++;
      if (currentGameType == Game::PARTIAL_GAME) {
        currentPoints = currentPoints - prevPoints;
        if (currentPoints < 0) currentPoints = 0;
        prevPoints = currentPoints; 
      } 
   }

   void operator=(const Message& msg) {
      pins =  msg._pins;
      currentPoints =  msg._points;
      rounds = msg._rounds;
      score = msg._score;
   }

   static State& get() {
      static State instance;
      return instance;
   }
private:
   State(const State&) = default;
   State() = default;
   void operator=(const State&) = delete;
};

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

Message receiveMessage() {
   startReceiving();
   Message msg;
   while(Comm.available()){
      debugPrintln("Receiving transmission!");
      Comm.readBytes(&(msg._bytes[0]), msg._bytes.size());
      if (msg._wire == WIRE && msg.verifyChecksum()) 
         return msg;
   }
   return msg;
}

void sendMessage(Command cmd) {  
   Message msg(State::get(), cmd);
   Message rcvMsg;
   do {
      startTransmitting();
      debugPrintln("Transmitting receission!");
      Comm.write(&(msg._bytes[0]), msg._bytes.size());
      delay(COMM_DELAY);
      rcvMsg = receiveMessage(); 
   } while (rcvMsg._cmd != Command::ACKNOWLEDGED);
}

void debugPrintln(String str){
  if (DEBUG) Log.println(str);
}

void debugPrint(String str){
  if (DEBUG) Log.print(str);
}

void startReceiving(){
  analogWrite(TXControl, 0); //Receiving
}

void startTransmitting(){
  analogWrite(TXControl, 255); //Transmitting
}

void lightLed (uint8_t led, bool b){
  if (b) analogWrite(LED[led], BRIGHTNESS);
  else analogWrite(LED[led], 0);
}

void lightAllLed(bool b){
  for (unsigned int i = 0; i < LED.size(); i++) lightLed(i, b);
}

void checkLED(){
  debugPrintln("************CHECKING LEDS*************");
  for (unsigned int i = 0; i < LED.size(); i++) {
   lightLed(i, true);
   delay((i < PINCOUNT) ? LIGHT_TIME : LIGHT_TIME * 4);
   lightLed(i, false);
  }
}

void stopEngines() {
  digitalWrite(ENGINE_RIGHT, LOW);
  digitalWrite(ENGINE_SLOW, LOW);
  digitalWrite(ENGINE_LEFT, LOW);
}

void untieKnot() { 
   sendMessage(Command::KNOT_CMD); // Send Status that Arduino is listening
   debugPrintln("If the pins are untied, confirm it");
   while (true) {
      if(Message msg = receiveMessage()) {
         debugPrintln("UNTIE KNOT: RECEIVED MESSAGE");
         if (msg._cmd == Command::SETTING_PINS) break;
      }
   }
}

void fixOverload(){
  State::get().knotCounter++;
  debugPrint("knot counter: ");
  debugPrintln((String) State::get().knotCounter);
  stopEngines();
  delay(1000);
  digitalWrite(ENGINE_LEFT, HIGH);
  if(State::get().knotCounter == 3 || State::get().knotCounter == 6) {
   delay(2000);
  } else {
   delay(800);
   }
  
  if (State::get().knotCounter == 8) {
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

void showCircle(){
  debugPrintln("Animating circle of LEDs");
  for (int i = 0; i < PINCOUNT; i++)
  {
    if (State::get().pins[i] > 0) lightLed(i, true);
  }

   static std::array<uint8_t, 8> circle = {0,1,3,6,8,7,5,2};

   for (auto it : circle) {
     lightLed(it, true);
     delay(LIGHT_TIME);
     lightLed(it,false);
   }
   
   for (auto it : circle) lightLed(it, true);
}

//Lights up all the pin leds
void blinkPinLeds(){
  debugPrintln("Let there be light");
  for (auto it : LED) lightLed(it, true);
  delay(2*LIGHT_TIME);
  for (auto it : LED) lightLed(it, false);
  delay(2*LIGHT_TIME);
  for (auto it : LED) lightLed(it, true);
}


/*  
Lights up leds corresponding to fallen pins. 
If the fallen pins form a circle and the game type is "Full Game", 
it displays an animated circle. If the player downs all the pins in one go,
it blinks several times.
*/
void showFallenPins(){
  uint8_t fallenCounter = 0;
  
  for (int i = 0; i < PINCOUNT; i++)
  {
    if (State::get().pins[i] > 0)
    {
      State::get().pins[i] = 1;
      fallenCounter++;
      lightLed(i, true);
      State::get().toMAG[i]++;
    } else lightLed(i, false);
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


bool respondToChangeStateCommand(){
  debugPrintln("***********CHANGING STATE***********");
  while (true)
  {
    Message msg = receiveMessage();
    if(msg) {
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


bool checkGaffe(){
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
        debugPrintln("No gutter");
        lightLed(LED_START, false);
        lightLed(LED_YELLOW, false);
        lightLed(LED_ERROR, false);
        return false;
      
      case Command::GAFFE_CONFIRMED:
        debugPrintln("Gutter confirmed");
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
          for (int i = 0; i < PINCOUNT; i++) {
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

//Lowering the pins
void lowerPins(){
  digitalWrite(ENGINE_RIGHT, HIGH);
  delay(ENGINE_DOWN);
  digitalWrite(ENGINE_SLOW, HIGH);
  delay(ENGINE_SLOWDOWN);
  digitalWrite(ENGINE_SLOW, LOW);
}

//resetting pin count
void resetPinCount(){
   debugPrintln("************ZEROING THE PINS*************");
   for (int i = 0; i < PINCOUNT; i++) State::get().pins[i] = 0;
}

  //checking if the pins don't fall while setting down - returns true if all are standing, false if at least one has fallen
bool checkSettingPins() {
    int countStandingPins = 0;
    for (int i = 0; i < PINCOUNT; i++)
    {
      if (digitalRead(PXSENSOR[i]) != LOW) 
      {
        countStandingPins++;
      }
    }
    if (countStandingPins == PINCOUNT)  return true;
    else return false;
}

//checking which pins have fallen - returns true if all are standing, false if at least one has fallen
bool checkPins() {
  if( Message msg = receiveMessage()) { 
    debugPrintln("CHECK PINS: RECEIVED MSG");
    readMsg(); 
    }
  
    int countStandingPins = 0;
    for (int i = 0; i < PINCOUNT; i++)
    {
      if (digitalRead(PXSENSOR[i]) != LOW) 
      {
        countStandingPins++;
      }
      else {
        State::get().pins[i] = true;
      }
    }
    if (countStandingPins == PINCOUNT)  return true;
    else return false;
}


void readMsg(const Message& msg)
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
            settingPins(currentGameType);
         }
         lightLed(LED_START, true);
         break;
      }

      case Command::SETTING_PINS:
      {
         debugPrintln("READ MSG: SETTING_PINS");
         isSettingPins = true;
         settingPins(currentGameType);
      
         delay(100);
         break;
      }

      case Command::CHECK_GUTTER:
      {
         debugPrintln("READ MSG: CHECK_GUTTER");
         lightLed(LED_START, false);
         lightLed(LED_ERROR, true);
         gutterButtonPressed = true;
         debugPrintln("The gutter button has been pressed");
         isGutter = checkGutter();
         break;
      }

      case Command::END_GAME:
      {
         debugPrintln("READ MSG: END_GAME");
         if(currentGameType == Game::PARTIAL_GAME) State::get().score = 0;
         startGame();
         break;
      }
    
      default:
      {
         newState = false;
         break;
      }
    }
}