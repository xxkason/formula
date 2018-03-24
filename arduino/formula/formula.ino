#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 115200

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
Car_4WD car4wd(1, 2, 3, 4, 10);

void setup()
{
  Serial.begin(GAMEPAD_BAUD_RATE);
  car4wd.attachWheel();
  btcar = &car4wd;
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
    switch (cmd)
  {
    case 'W':
      byte speed;
      speed = Serial.parseInt();

      // clear the followed message "Pxxx\n"
      Serial.readStringUntil('\n');
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
      Serial.readStringUntil('S');
      // clear end

      byte angle;
      angle = Serial.parseInt();
      btcar->turn(angle * 180 / 256);
      break;
  }
  }
}