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

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
// Car_2DC_L293D car293d(1, 2);
// Car_2DC_L298N car298n(2, 4, 6, 7, 8, 9);
// Car_2DC_L2HBd car2hbd(3, 5, 6, 9);
Car_4WD car4wd(1, 2, 3, 4, 10);

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
  processMessage();
}

void processMessage()
{
  char cmd[1];
  if (Serial.readBytes(cmd, 1))
  {
    //Serial.println(cmd);
    switch(cmd[0])
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
      case 's':
      case 'E':
        btcar->stop();
        //Serial.println("STOPPED");
        break;
      case 'F':
      case 'G':
      case 'H':
      case 'I':
      case 'J':
        //Serial.println("Unkown command");
        break;
      case 'K':
        car4wd.uturn(BACK);
        //Serial.println("U-Turn Left");
        break;
      case 'L':
        car4wd.uturn(FOR);
        //Serial.println("U-Turn Right");
        break;
      case 'M':
      case 'N':
        //Serial.println("Reserved command for auto mode switch");
        //autoMode = !autoMode;
        break;
      case 'W':
        long speed;
        speed = Serial.parseInt();
        
        if (speed < 127)
        {
          btcar->changeSpeed(255 - 2 * speed); //127 is the center position value, min value is 0, max value is 255, 128 = 255 -127, full is (127-speed)/(127-0)*255
          btcar->run(FOR);
        }
        else if (speed > 127)
        {
          btcar->changeSpeed(2 * speed); //127 is the center position value, min value is 0, max value is 255, 128 = 255 -127, full is speed/(255-127)*255
          btcar->run(BACK);
          // Serial.print("go backward with speed: ");
          // Serial.println(speed);
        }
        else if (speed ==127)
        {
          btcar->stop();
          // Serial.print("got speed value: ");
          // Serial.print(speed);
          // Serial.println(";, stoped");
        }
        break;
      case 'P':
        long angle;
        angle = Serial.parseInt();
        car4wd.analog_turn(angle * 180 / 256);
        // Serial.print("Turn to angle: ");
        // Serial.println(angle);
        break;
    }
  }
  else
  {
    btcar->stop();
  }
}
