#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 115200

Car *btcar;
Car_4WD car4wd(1, 2, 3, 4, 10);

void setup()
{
  car4wd.attachWheel();
  btcar = &car4wd;
  Serial.begin(GAMEPAD_BAUD_RATE);
}

void loop()
{
  processMessage();
  //messageHandler();
}

//void messageHandler()
//{
//  char msg[1];
//  if (Serial.readBytes(msg, 1))
//  {
//    char cmd = msg[0];
//    switch (cmd)
//    {
//      case 'W':
//        byte speed;
//        speed = Serial.parseInt();
//        if (speed < 127)
//        {
//          btcar->changeSpeed(255 - 2 * speed);
//          btcar->run(FOR);
//        }
//        else if (speed > 127)
//        {
//          btcar->changeSpeed(2 * speed - 255);
//          btcar->run(BACK);
//        }
//        else if (speed == 127)
//        {
//          btcar->stop();
//        }
//
//        // clear the followed message "Pxxx\n"
//        Serial.readStringUntil('\n');
//        // clear end
//        break;
//      case 'Q':
//        // clear the initial message "Qxxx"
//        Serial.readStringUntil('S');
//        // clear end
//
//        byte angle;
//        angle = Serial.parseInt();
//        btcar->turn(angle * 180 / 256);
//        break;
//    }
//  }
//}

void processMessage()
{
  String msg = Serial.readStringUntil('\n');
  
  if (msg.startsWith("Q"))
  {
    byte index = msg.indexOf("S");
    String sub = msg.substring(++index);
    byte angle = sub.toInt();
    btcar->turn(angle * 180 / 256);
    return;
  }

  if (msg.startsWith("W"))
  {
    byte index = msg.indexOf("P");
    String sub = msg.substring(1, index);
    byte speed = sub.toInt();

    if (speed == 127)
    {
      btcar->stop();
      return;
    }
    btcar->changeSpeed(abs(255 - 2 * speed));
    if (speed < 127)
    {
      btcar->run(FOR);
      return;
    }
    if (speed > 127)
    {
      btcar->run(BACK);
      return;
    }
  }
}
