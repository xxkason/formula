#include <SoftwareSerial.h>
#include "Car_2DC.h"
#include "Car_4WD.h"

#define HARD_BAUD_RATE 57600
#define SOFT_BAUD_RATE 9600

SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;

void setup()
{
  Serial.begin(HARD_BAUD_RATE);
  while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
  }
  softSerial.begin(SOFT_BAUD_RATE);
  Serial.println("Hello, monitor");

  Car_2DC car293d(1, 2);
  Car_2DC car298n(2, 4, 6, 7, 8, 9);
  Car_2DC car2hbd(3, 5, 6, 9);
  Car_4WD car4wd(1, 2, 3, 4, 9);
  btcar = & car2hbd;
}

void loop()
{
  char cmd[1];
  if (softSerial.available() > 0)
  {
    softSerial.readBytes(cmd, 1);
    Serial.println(cmd);
    switch(cmd[0])
    {
      case 'f':
      case 'A':
        btcar->run(FOR);
        Serial.println("Run Forward");
        break;
      case 'b':
      case 'B':
        btcar->run(BACK);
        Serial.println("Run Backward");
        break;
      case 'l':
      case 'K':
        btcar->turn(FOR);
        Serial.println("Turn Left");
        break;
      case 'r':
      case 'L':
        btcar->turn(BACK);
        Serial.println("Turn Right");
        break;
      case 's':
        btcar->stop();
        Serial.println("STOPPED");
        break;
    }
  }
}

