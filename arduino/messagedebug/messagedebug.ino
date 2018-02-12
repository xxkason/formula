#include <SoftwareSerial.h>
#include <EEPROM.h>

#define GAMEPAD_BAUD_RATE 9600
#define BLUETOOTH_BAUD_RATE 115200
#define INDICATOR_PIN 2

enum PadMode
{
  LEFT_ANALOG_STICK = 0,
  LEFT_RIGHT_STICK = 1,
  LEFT_PAD_KEY = 2,
  RIGHT_PAD_KEY = 3
};

enum CarMode
{
  MANUAL = 0,
  RECORD = 1,
  REPLAY = 2
};

SoftwareSerial softSerial(10, 11); // RX, TX
PadMode padMode = LEFT_ANALOG_STICK;
CarMode carMode = MANUAL;
unsigned long beginTime;
int currentAngle = 90;

void setup() {
  Serial.begin(BLUETOOTH_BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  softSerial.begin(GAMEPAD_BAUD_RATE);
  Serial.println("Hello, monitor");
  pinMode(INDICATOR_PIN, OUTPUT);
  digitalWrite(INDICATOR_PIN, LOW);
  softSerial.setTimeout(300);
}

void loop() {
  //processMessage();
  printSerialMsg();
}

void printSerialMsg()
{
  char msg[1];
  if (softSerial.readBytes(msg, 1))
  {
    char cmd = msg[0];
    char temp[16];
    String tmp;
    Serial.println(cmd);
    switch (cmd)
    {
      case 'W':
        long speed;
        speed = softSerial.parseInt();
        Serial.print("first int parse, speed = ");
        Serial.println(speed);
        //softSerial.readBytesUntil('\n', temp, 16);
        tmp = softSerial.readStringUntil('\n');
        Serial.print("first int parse, tmp = ");
        Serial.println(tmp);
        break;
      case 'Q':
        // clear the initial message "Qxxx"
        // softSerial.readBytesUntil('S', temp, 16);
        tmp = softSerial.readStringUntil('S');
        Serial.print("Q message parse, tmp = ");
        Serial.println(tmp);
        // clear end

        long angle;
        angle = softSerial.parseInt();
        Serial.print("Q message parse, angle = ");
        Serial.println(angle);
        break;
    }
  }
}

void processMessage()
{
  char msg[1];
  if (softSerial.readBytes(msg, 1))
  {
    char cmd = msg[0];
    debugOutput("Serial port get following command: ");
    Serial.println(cmd);
    if (cmd == 'G')
    {
      carMode = CarMode((carMode + 1) % 3);
      switch (carMode)
      {
        case MANUAL:
          digitalWrite(INDICATOR_PIN, LOW);
          debugOutput("switch to manual mode");
          break;
        case RECORD:
          digitalWrite(INDICATOR_PIN, HIGH);
          debugOutput("switch to learning mode");
          break;
        case REPLAY:
          digitalWrite(INDICATOR_PIN, HIGH);
          debugOutput("switch to auto mode");
          break;
      }
    }
    else
    {
      switch (carMode)
      {
        case MANUAL:
          manualControl(cmd);
          break;
        case REPLAY:
          if (cmd == 'H') // 'H' is the command to start replay in REPLAY mode
            debugOutput("Car Replay started");
          break;
        case RECORD:
          if (cmd == 'H') // 'H' is the command to start record in RECORD mode
            record();
          break;
      }
    }
  }
  else if (carMode == MANUAL && padMode == LEFT_PAD_KEY)
    debugOutput("Car stopped for serial timeout");
  else if (carMode == RECORD)
    //Serial.println("Blink the led to indicate this is learn mode now");
    ledBlink(INDICATOR_PIN, 500);
}

// void autoControl()
// {
//   for (int index = 0; index < EEPROM.length() ; index++) {
//     if (EEPROM[index] == 0) {
//       Serial.print("EEPROM[index] = ");
//       Serial.println(EEPROM[index]);
//       break;
//     }
//     Serial.println(char(EEPROM[index]));
//     learn_control(EEPROM[index]);
//     byte high = EEPROM.read(++index);
//     byte low = EEPROM.read(++index);
//     int duration = word(high, low);
//     Serial.println(duration);
//     delay(duration);
//   }
// }

void record()
{
  digitalWrite(INDICATOR_PIN, LOW); // turn off the led to indicate user eeprom clear is started
  debugOutput("clear eeprom started");
  clearEEPROM();
  digitalWrite(INDICATOR_PIN, HIGH); // turn on the led to indicate user eeprom clear is done
  debugOutput("clear eeprom finished");
  beginTime = 0;

  while (true)
  {
    char learnCmd[1];
    if (softSerial.readBytes(learnCmd, 1))
    {
      char cmdValue = learnCmd[0];
      if (cmdValue == 'G')
      {
        // stop learning
        debugOutput("Car stopped for learning terminated");
        digitalWrite(INDICATOR_PIN, LOW);
        break;
      }
      switch (cmdValue)
      {
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'H':
          if (beginTime != 0)
          {
            int duration = millis() - beginTime;
            debugOutput("Write time duration to eeprom.");
            Serial.print("duration is: ");
            Serial.println(duration);
          }
          debugOutput("Write command to eeprom.");
          Serial.print("command is: ");
          Serial.println(cmdValue);
          rightPadKey(cmdValue);
          beginTime = millis();
          break;
      }
    }
    else
      ledBlink(INDICATOR_PIN, 500);
  }
}

void manualControl(char cmd)
{
  if (cmd == 'H') // switch gamepad control mode
  {
    padMode = PadMode((padMode + 1) % 4);
  }
  else
  {
    switch (padMode)
    {
      case LEFT_ANALOG_STICK:
        leftAnalogStick(cmd);
        break;
      case LEFT_RIGHT_STICK:
        leftrightAnalogStick(cmd);
        break;
      case LEFT_PAD_KEY:
        leftPadKey(cmd);
        break;
      case RIGHT_PAD_KEY:
        rightPadKey(cmd);
        break;
    }
  }
}

void leftAnalogStick(char cmd)
{
  if (cmd == 'W')
  {
    long speed;
    long angle;
    speed = softSerial.parseInt();
    angle = softSerial.parseInt();

    debugOutput(speed);
    if (speed < 127)
      debugOutput("Go forward with full speed");
    else if (speed > 127)
      debugOutput("Go backward with full speed");
    else if (speed == 127)
      debugOutput("Car stopped");

    debugOutput(angle);
    debugOutput("Car wheeled to specified angle");
  }
}

void leftrightAnalogStick(char cmd)
{
  char temp[6];
  switch (cmd)
  {
    case 'W':
      long speed;
      speed = softSerial.parseInt();

      // clear the followed message "Pxxx\n"
      softSerial.readBytesUntil('\n', temp, 4);
      // clear end

      debugOutput(speed);
      if (speed < 127)
        debugOutput("Go forward with full speed");
      else if (speed > 127)
        debugOutput("Go backward with full speed");
      else if (speed == 127)
        debugOutput("Car stopped");
      break;
    case 'Q':
      // clear the initial message "Qxxx"
      softSerial.readBytesUntil('S', temp, 3);
      // clear end
      long angle;
      angle = softSerial.parseInt();
      debugOutput(angle);
      debugOutput("Car wheeled to specified angle");
      break;
  }
}

void leftPadKey(char cmd)
{
  switch (cmd)
  {
    case 'f':
    case 'A':
      debugOutput("Run Forward");
      break;
    case 'b':
    case 'B':
      debugOutput("Run Backward");
      break;
    case 'l':
    case 'C':
      debugOutput("Turn Left");
      break;
    case 'r':
    case 'D':
      debugOutput("Turn Right");
      break;
  }
}

void rightPadKey(char cmd)
{
  switch (cmd)
  {
    case 'I':
      debugOutput("Run Forward");
      break;
    case 'J':
      debugOutput("Run Backward");
      break;
    case 'K':
      debugOutput("Turn Left");
      break;
    case 'L':
      debugOutput("Turn Right");
      break;
    case 'E':
    case 'H':
      debugOutput("Car Stopped");
      break;
  }
}

void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    delay(10);
  }
}

void ledBlink(int ledPin, int period)
{
  digitalWrite(ledPin, LOW);
  delay(period);
  digitalWrite(ledPin, HIGH);
}

void debugOutput(String msg)
{
  String strCarMode = "";
  String strPadMode = "";
  switch (carMode)
  {
    case MANUAL:
      strCarMode = "MANUAL";
      break;
    case RECORD:
      strCarMode = "RECORD";
      break;
    case REPLAY:
      strCarMode = "REPLAY";
      break;
  }
  Serial.print("[");
  Serial.print(strCarMode);
  Serial.print("] - ");
  if (carMode == MANUAL)
  {
    switch (padMode)
    {
      case LEFT_ANALOG_STICK:
        strPadMode = "LEFT_ANALOG_STICK";
        break;
      case LEFT_RIGHT_STICK:
        strPadMode = "LEFT_RIGHT_STICK";
        break;
      case LEFT_PAD_KEY:
        strPadMode = "LEFT_PAD_KEY";
        break;
      case RIGHT_PAD_KEY:
        strPadMode = "RIGHT_PAD_KEY";
        break;
    }
    Serial.print("[");
    Serial.print(strPadMode);
    Serial.print("] - ");
  }
  Serial.println(msg);
}

void debugOutput(int value)
{
  String strCarMode = "";
  String strPadMode = "";
  switch (carMode)
  {
    case MANUAL:
      strCarMode = "MANUAL";
      break;
    case RECORD:
      strCarMode = "RECORD";
      break;
    case REPLAY:
      strCarMode = "REPLAY";
      break;
  }
  Serial.print("[");
  Serial.print(strCarMode);
  Serial.print("] - ");
  if (carMode == MANUAL)
  {
    switch (padMode)
    {
      case LEFT_ANALOG_STICK:
        strPadMode = "LEFT_ANALOG_STICK";
        break;
      case LEFT_RIGHT_STICK:
        strPadMode = "LEFT_RIGHT_STICK";
        break;
      case LEFT_PAD_KEY:
        strPadMode = "LEFT_PAD_KEY";
        break;
      case RIGHT_PAD_KEY:
        strPadMode = "RIGHT_PAD_KEY";
        break;
    }
    Serial.print("[");
    Serial.print(strPadMode);
    Serial.print("] - ");
  }
  Serial.print("Get a variable with value: ");
  Serial.println(value);
}
