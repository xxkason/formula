#include <EEPROM.h>
//#include <SoftwareSerial.h>
//#include "Car_2DC.h"

/* Gamepad msg info
     Left           Right
      A               I
    C   D   G   H   K   L
      B               J

      E               M
      F               N
A,B,C,D,E,F,M,N continuous send cmd when pressed */

#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 9600
#define BLUETOOTH_BAUD_RATE 9600
#define CAR_MODE_INDICATOR_PIN 2
#define PAD_MODE_INDICATOR_PIN 13
//#define PROXIMITY_SENSOR_PIN1

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

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
// Car_2DC_L293D car293d(1, 2);
// Car_2DC_L298N car298n(2, 4, 6, 7, 8, 9);
// Car_2DC_L2HBd car2hbd(3, 5, 6, 9);
Car_4WD car4wd(1, 2, 3, 4, 10);
PadMode padMode = LEFT_ANALOG_STICK;
CarMode carMode = MANUAL;
int romAddr = 0;
unsigned long beginTime;

void setup()
{
  Serial.begin(GAMEPAD_BAUD_RATE);
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }
  // softSerial.begin(SOFT_BAUD_RATE);
  // Serial.println("Hello, monitor");
  car4wd.attachWheel();
  btcar = &car4wd;
  pinMode(CAR_MODE_INDICATOR_PIN, OUTPUT);
  digitalWrite(CAR_MODE_INDICATOR_PIN, LOW);
  pinMode(PAD_MODE_INDICATOR_PIN, OUTPUT);
  digitalWrite(PAD_MODE_INDICATOR_PIN, LOW);
  Serial.setTimeout(300);
}

void loop()
{
  processMessage();
}

void processMessage()
{
  char msg[1];
  if (Serial.readBytes(msg, 1))
  {
    char cmd = msg[0];
    if (cmd == 'G') // switch car mode
    {
      carMode = CarMode((carMode + 1) % 3);
      switch (carMode)
      {
        case MANUAL:
          digitalWrite(CAR_MODE_INDICATOR_PIN, LOW);
          break;
        case RECORD:
          digitalWrite(CAR_MODE_INDICATOR_PIN, HIGH);
          break;
        case REPLAY:
          digitalWrite(CAR_MODE_INDICATOR_PIN, HIGH);
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
            replay();
          break;
        case RECORD:
          if (cmd == 'H') // 'H' is the command to start record in RECORD mode
            record();
          break;
      }
    }
  }
  else if (carMode == MANUAL && padMode == LEFT_PAD_KEY)
    btcar->stop();
  else if (carMode == RECORD)
    ledBlink(CAR_MODE_INDICATOR_PIN, 1000);
}

void replay()
{
  for (int index = 0; index < EEPROM.length() ; index++) {
    if (EEPROM[index] == 0) {
      btcar->stop();
      break;
    }
    rightPadKey(EEPROM[index]);
    byte high = EEPROM.read(++index);
    byte low = EEPROM.read(++index);
    int duration = word(high, low);
    delay(duration);
  }
}

void record()
{
  digitalWrite(CAR_MODE_INDICATOR_PIN, LOW); // turn off the led to indicate user eeprom clear is started
  clearEEPROM();
  digitalWrite(CAR_MODE_INDICATOR_PIN, HIGH); // turn on the led to indicate user eeprom clear is done
  beginTime = 0;
  romAddr = 0;

  while (true)
  {
    char learnCmd[1];
    if (Serial.readBytes(learnCmd, 1))
    {
      char cmdValue = learnCmd[0];
      if (cmdValue == 'G')
      {
        // stop learning
        btcar->stop();
        digitalWrite(CAR_MODE_INDICATOR_PIN, LOW);
        break;
      }
      switch (cmdValue)
      {
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'H':
          if (romAddr < EEPROM.length())
          {
            if (beginTime != 0)
            {
              int duration = millis() - beginTime;
              EEPROM.write(romAddr++, highByte(duration));
              EEPROM.write(romAddr++, lowByte(duration));
            }
            EEPROM.write(romAddr++, cmdValue);
            rightPadKey(cmdValue);
            beginTime = millis();
          }
          break;
      }
    }
    else
      ledBlink(CAR_MODE_INDICATOR_PIN, 500);
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
      speed = Serial.parseInt();
      angle = Serial.parseInt();

      /*
        127 is the center position value,
        top position is min value = 0,
        bottom position is max value = 255,
        easy way: <127 go forward, =127 stop, >127 go backward
        analog speed change:
        0: go forward with full speed
        127: speed is 0
        255: go backward with full speed
        go forward speed formula: (127-speed)/(127-0)*255
        go backward speed formula: speed/(255-127)*255
      */
      if (speed < 127)
      {
        //btcar->changeSpeed(255 - 2 * speed);
        btcar->run(FOR);
      }
      else if (speed > 127)
      {
        //btcar->changeSpeed(2 * speed);
        btcar->run(BACK);
      }
      else if (speed == 127)
      {
        btcar->stop();
      }
      
      /*
        128 is the center position value,
        left position is min value = 0,
        right position is max value = 255,
        analog speed change:
        0: map to servo 0
        128: map to servo 90
        255: map to servo 180
      */
      car4wd.changeAngle(angle * 180 / 256);
  }
}

void leftrightAnalogStick(char cmd)
{
  char temp[6];
  switch (cmd)
  {
    case 'W':
      long speed;
      speed = Serial.parseInt();

      // clear the followed message "Pxxx\n"
      Serial.readBytesUntil('\n', temp, 4);
      // clear end

      if (speed < 127)
      {
        btcar->changeSpeed(255 - 2 * speed);
        btcar->run(FOR);
      }
      else if (speed > 127)
      {
        btcar->changeSpeed(2 * speed);
        btcar->run(BACK);
      }
      else if (speed == 127)
      {
        btcar->stop();
      }
      break;
    case 'Q':
      
      // clear the initial message "Qxxx"
      Serial.readBytesUntil('S', temp, 3);
      // clear end

      long angle;
      angle = Serial.parseInt();
      car4wd.changeAngle(angle * 180 / 256);
      break;
  }
}

void leftPadKey(char cmd)
{
  switch (cmd)
  {
    case 'f':
    case 'A':
      btcar->run(FOR);
      //Serial.println("Run Forward");
      break;
    case 'b':
    case 'B':
      btcar->run(BACK);
      //Serial.println("Run Backward");
      break;
    case 'l':
    case 'C':
      btcar->turn(BACK);
      //Serial.println("Turn Left");
      break;
    case 'r':
    case 'D':
      btcar->turn(FOR);
      //Serial.println("Turn Right");
      break;
  }
}

void rightPadKey(char cmd)
{
  switch (cmd)
  {
    case 'I':
      btcar->run(FOR);
      //Serial.println("Run Forward");
      break;
    case 'J':
      btcar->run(BACK);
      //Serial.println("Run Backward");
      break;
    case 'K':
      car4wd.changeAngle(car4wd.currentAngle() - 90);
      //Serial.println("Turn Left");
      break;
    case 'L':
      car4wd.changeAngle(car4wd.currentAngle() + 90);
      //Serial.println("Turn Right");
      break;
    case 'E':
    case 'H':
      btcar->stop();
      break;
  }
}

void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void ledBlink(int ledPin, int period)
{
  digitalWrite(ledPin, LOW);
  delay(period);
  digitalWrite(ledPin, HIGH);
}
