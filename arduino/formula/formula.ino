//#include <SoftwareSerial.h>
#include "Car_2DC.h"
//#include "Car_4WD.h"

#define HARD_BAUD_RATE 115200
//#define SOFT_BAUD_RATE 115200

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
Car_2DC_L293D car293d(1, 2);
// Car_2DC_L298N car298n(2, 4, 6, 7, 8, 9);
// Car_2DC_L2HBd car2hbd(6,9,3,5);
// Car_4WD car4wd(1, 2, 3, 4, 9);

void setup()
{
  Serial.begin(HARD_BAUD_RATE);
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }
  // softSerial.begin(SOFT_BAUD_RATE);
  // Serial.println("Hello, monitor");

  btcar = & car293d;
  Serial.setTimeout(1000);
}

void loop()
{
  msgHandler();
}

void msgHandler()
{
  char msg[32];

  if (Serial.readBytes(msg, 1) > 0)
  {
    switch (msg[0])
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
        btcar->turn(FOR);
        //Serial.println("Turn Left");
        break;
      case 'r':
      case 'D':
        btcar->turn(BACK);
        //Serial.println("Turn Right");
        break;
      case 's':
      case 'E':
        btcar->stop();
        //Serial.println("STOPPED");
        break;
    }
  }
  else
  {
    btcar->stop();
  }
}