#include <SoftwareSerial.h>

#define GAMEPAD_BAUD_RATE 115200
#define BLUETOOTH_BAUD_RATE 9600

SoftwareSerial softSerial(10, 11); // RX, TX
bool autoMode = false;

void setup() {
  Serial.begin(BLUETOOTH_BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  softSerial.begin(GAMEPAD_BAUD_RATE);
  Serial.println("Hello, monitor");
}

void loop() {
  processMessage()
}

void processMessage()
{
  char cmd[1];
  if (softSerial.readBytes(cmd, 1))
  {
    Serial.println(cmd);
    switch(cmd[0])
    {
      case 'f':
      case 'A':
        Serial.println("Run Forward");
        break;
      case 'b':
      case 'B':
        Serial.println("Run Backward");
        break;
      case 'l':
      case 'C':
        Serial.println("Turn Left");
        break;
      case 'r':
      case 'D':
        Serial.println("Turn Right");
        break;
      case 's':
      case 'E':
        Serial.println("STOPPED");
        break;
      case 'F':
      case 'G':
      case 'H':
      case 'I':
      case 'J':
        Serial.println("Unkown command");
        break;
      case 'K':
        Serial.println("U-Turn Left");
        break;
      case 'L':
        Serial.println("U-Turn Right");
        break;
      case 'M':
      case 'N':
        Serial.println("Reserved command for auto mode switch");
        autoMode = !autoMode;
        break;
      case 'W':
        long speed;
        speed = softSerial.parseInt();
        
        if (speed > 127)
        {
          Serial.println("go forward with speed %d", speed);
        }
        else if (speed < 127)
        {
          Serial.println("go backward with speed %d", speed);
        }
        else if (speed ==127)
          Serial.println("got %d, stoped", speed);
        break;
      case 'P':
        long angle;
        angle = softSerial.parseInt();
        Serial.println("Turn to angle %d", angle);
        break;
    }
  }
}
