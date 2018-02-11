#include <EEPROM.h>
//#include <SoftwareSerial.h>
//#include "Car_2DC.h"
// Gamepad msg info
/*   Left           Right
      A               I
    C   D   G   H   K   L
      B               J

      E               M
      F               N
A,B,C,D,E,F,M,N continuous send cmd when pressed
*/
#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 9600
#define BLUETOOTH_BAUD_RATE 9600
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
  LEARN = 1,
  AUTO = 2
};

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
// Car_2DC_L293D car293d(1, 2);
// Car_2DC_L298N car298n(2, 4, 6, 7, 8, 9);
// Car_2DC_L2HBd car2hbd(3, 5, 6, 9);
Car_4WD car4wd(1, 2, 3, 4, 10);
PadMode manual_mode = LEFT_ANALOG_STICK;
CarMode control_mode = MANUAL;
int rom_addr = 0;
unsigned long begin_time;

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
  pinMode(INDICATOR_PIN, OUTPUT);
  digitalWrite(INDICATOR_PIN, LOW);
  Serial.setTimeout(300);
}

void loop()
{
  control_car();
}

void control_car()
{
  char msg[1];
  if (Serial.readBytes(msg, 1))
  {
    char cmd = msg[0];
    if (cmd == 'G')
    {
      control_mode = CarMode((control_mode + 1) % 3);
      switch (control_mode)
      {
        case MANUAL:
          digitalWrite(INDICATOR_PIN, LOW);
          break;
        case LEARN:
          clear_eeprom();
          rom_addr = 0;
          begin_time = 0;
          digitalWrite(INDICATOR_PIN, HIGH);
          break;
        case AUTO:
          digitalWrite(INDICATOR_PIN, HIGH);
          break;
      }
    }
    else
    {
      switch (control_mode)
      {
        case MANUAL:
          manual_control(cmd);
          break;
        case AUTO:
          if (cmd == 'H')
          {
            auto_control();
          }
          break;
        case LEARN:
          learning(cmd);
          break;
      }
    }
  }
  else if (control_mode == MANUAL && manual_mode == LEFT_PAD_KEY)
  {
    btcar->stop();
  }
  else if (control_mode == LEARN)
  {
    digitalWrite(INDICATOR_PIN, LOW);
    delay(500);
    digitalWrite(INDICATOR_PIN, HIGH);
  }
}

void manual_control(char cmd)
{
  if (cmd == 'H')
  {
    manual_mode = PadMode((manual_mode + 1) % 4);
  }
  else
  {
    switch (manual_mode)
    {
      case LEFT_ANALOG_STICK:
        leftstick_control(cmd);
        break;
      case LEFT_RIGHT_STICK:
        leftrightstick_control(cmd);
        break;
      case LEFT_PAD_KEY:
        leftkey_control(cmd);
        break;
      case RIGHT_PAD_KEY:
        rightkey_control(cmd);
        break;
    }
  }
}

void auto_control()
{
  for (int index = 0; index < EEPROM.length() ; index++) {
    if (EEPROM[index] == 0) {
      btcar->stop();
      break;
    }
    learn_control(EEPROM[index]);
    delay(EEPROM[++index]);
  }
}

void learning(char cmd)
{
  switch (cmd)
  {
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'H':
      if (rom_addr < EEPROM.length())
      {
        if (begin_time != 0)
          EEPROM.write(rom_addr++, millis() - begin_time);
        EEPROM.write(rom_addr++, cmd);
        learn_control(cmd);
        begin_time = millis();
      }
      break;
  }
}

void leftstick_control(char cmd)
{
  switch (cmd)
  {
    case 'W':
      long speed;
      speed = Serial.parseInt();

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
      break;
    case 'P':
      long angle;
      angle = Serial.parseInt();

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
      break;
  }
}

void leftrightstick_control(char cmd)
{
  switch (cmd)
  {
    case 'W':
      long speed;
      speed = Serial.parseInt();

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
    case 'S':
      long angle;
      angle = Serial.parseInt();
      car4wd.changeAngle(angle * 180 / 256);
      break;
  }
}

void leftkey_control(char cmd)
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

void rightkey_control(char cmd)
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
      btcar->stop();
      break;
  }
}

void learn_control(char cmd)
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
    case 'H':
      btcar->stop();
      break;
  }
}

void clear_eeprom()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}
