//#include <SoftwareSerial.h>
//#include "Car_2DC.h"
// Gamepad msg info
/*   Left           Right
      A               I
    C   D   G   H   K   L
      B               J

      E               M
      F               N
*/
#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 9600
#define BLUETOOTH_BAUD_RATE 9600

enum PadMode
{
  LEFT_ANALOG_STICK = 0,
  LEFT_RIGHT_STICK = 1,
  LEFT_PAD_KEY = 2,
  RIGHT_PAD_KEY = 3
};

enum CarMode
{
  AUTO = 0,
  MANUAL = 1,
  LEARN = 2
};

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
// Car_2DC_L293D car293d(1, 2);
// Car_2DC_L298N car298n(2, 4, 6, 7, 8, 9);
// Car_2DC_L2HBd car2hbd(3, 5, 6, 9);
Car_4WD car4wd(1, 2, 3, 4, 10);
bool autoMode = false;
PadMode manual_mode = LEFT_ANALOG_STICK;
CarMode control_mode = MANUAL;


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
  Serial.setTimeout(300);
}

void loop()
{
  manual_control();
}

void manual_control()
{
  char cmd[1];
  if (Serial.readBytes(cmd, 1))
  {
    if (cmd[0] == 'G')
    {
      manual_mode = PadMode((manual_mode + 1) % 4);
    }
    else
    {
      char msg = cmd[0];
      switch (manual_mode)
      {
        case LEFT_ANALOG_STICK:
          leftstick_control(msg);
          break;
        case LEFT_RIGHT_STICK:
          leftrightstick_control(msg);
          break;
        case LEFT_PAD_KEY:
          leftkey_control(msg);
          break;
        case RIGHT_PAD_KEY:
          rightkey_control(msg);
          break;
      }
    }
  }
  else if (manual_mode == LEFT_PAD_KEY)
  {
    btcar->stop();
  }
}

void leftstick_control(char cmd)
{
  switch(cmd)
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
      else if (speed ==127)
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
  switch(cmd)
  {
    case 'W':
      long speed;
      speed = Serial.parseInt();
      
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
      else if (speed ==127)
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
  switch(cmd)
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
  switch(cmd)
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
