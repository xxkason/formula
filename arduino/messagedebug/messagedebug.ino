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
  LEARN = 1,
  AUTO = 2
};

SoftwareSerial softSerial(10, 11); // RX, TX
PadMode manual_mode = LEFT_ANALOG_STICK;
CarMode control_mode = MANUAL;
int rom_addr = 0;
unsigned long begin_time;
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
  processMessage();
}

void processMessage()
{
  char msg[1];
  if (softSerial.readBytes(msg, 1))
  {
    char cmd = msg[0];
    Serial.println(cmd);
    if (cmd == 'G')
    {
      control_mode = CarMode((control_mode + 1) % 3);
      switch (control_mode)
      {
        case MANUAL:
          digitalWrite(INDICATOR_PIN, LOW);
          Serial.println("switch to manual mode");
          break;
        case LEARN:
          clear_eeprom();
          rom_addr = 0;
          begin_time = 0;
          digitalWrite(INDICATOR_PIN, HIGH);
          Serial.println("switch to learning mode");
          break;
        case AUTO:
          digitalWrite(INDICATOR_PIN, HIGH);
          Serial.println("switch to auto mode");
          break;
      }
    }
    else
    {
      switch (control_mode)
      {
        case MANUAL:
          manualControl(cmd);
          break;
        case AUTO:
          if (cmd == 'H')
          {
            Serial.println("start auto run");
            autoControl();
          }
          break;
        case LEARN:
          Serial.println("start learning the route...");
          learning(cmd);
          break;
      }
    }
  }
  else if (control_mode == MANUAL && manual_mode == LEFT_PAD_KEY)
  {
    Serial.println("Stop car for serial timeout");
  }
  else if (control_mode == LEARN)
  {
    //Serial.println("Blink the led to indicate this is learn mode now");
    digitalWrite(INDICATOR_PIN, LOW);
    delay(500);
    digitalWrite(INDICATOR_PIN, HIGH);
  }
}

void manualControl(char cmd)
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
        Serial.println("use left analog stick to control the car");
        switch (cmd)
        {
          case 'W':
            long speed;
            speed = Serial.parseInt();
            if (speed < 127)
              Serial.println("run forward with full speed");
            else if (speed > 127)
              Serial.println("run backward with full speed");
            else if (speed == 127)
              Serial.println("car stopped");
            break;
          case 'P':
            long angle;
            angle = Serial.parseInt();
            Serial.print("wheel the car to angle ");
            Serial.println(angle);
            break;
        }
        break;
      case LEFT_RIGHT_STICK:
        Serial.println("use left analog stick to control the car speed");
        Serial.println("use right analog stick to control the car angle");
        switch (cmd)
        {
          case 'W':
            long speed;
            speed = Serial.parseInt();

            if (speed < 127)
            {
              Serial.print("go forward with speed ");
              Serial.println(255 - 2 * speed);
            }
            else if (speed > 127)
            {
              Serial.print("go backward with speed ");
              Serial.println(2 * speed);
            }
            else if (speed == 127)
            {
              Serial.println("car stopped");
            }
            break;
          case 'S':
            long angle;
            angle = Serial.parseInt();
            Serial.print("wheel the car to angle ");
            Serial.println(angle);
            break;
        }
        break;
      case LEFT_PAD_KEY:
        Serial.println("use left pad key to control the car");
        switch (cmd)
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
        }
        break;
      case RIGHT_PAD_KEY:
        Serial.println("use right pad key to control the car");
        switch (cmd)
        {
          case 'I':
            Serial.println("Run Forward");
            break;
          case 'J':
            Serial.println("Run Backward");
            break;
          case 'K':
            Serial.print("turn to angle ");
            Serial.println(currentAngle - 90);
            break;
          case 'L':
            Serial.print("turn to angle ");
            Serial.println(currentAngle + 90);
            break;
          case 'E':
            Serial.println("car stopped");
            break;
        }
        break;
    }
  }
}

void autoControl()
{
  for (int index = 0; index < EEPROM.length() ; index++) {
    if (EEPROM[index] == 0) {
      Serial.print("EEPROM[index] = ");
      Serial.println(EEPROM[index]);
      break;
    }
    Serial.println(char(EEPROM[index]));
    learn_control(EEPROM[index]);
    byte high = EEPROM.read(++index);
    byte low = EEPROM.read(++index);
    int duration = word(high, low);
    Serial.println(duration);
    delay(duration);
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
        if (begin_time != 0) {
          int duration = millis() - begin_time;
          EEPROM.write(rom_addr++, highByte(duration));
          EEPROM.write(rom_addr++, lowByte(duration));
          Serial.println(millis() - begin_time);
        }
        //        Serial.print("EEPROM[");
        //        Serial.print(rom_addr-1);
        //        Serial.print("]=");
        //        Serial.println(EEPROM[rom_addr-1]);
        EEPROM.write(rom_addr++, cmd);
        //        Serial.print("EEPROM[");
        //        Serial.print(rom_addr-1);
        //        Serial.print("]=");
        //        Serial.println(EEPROM[rom_addr-1]);
        learn_control(cmd);
        begin_time = millis();
      }
      break;
  }
}

void learn_control(char cmd)
{
  switch (cmd)
  {
    case 'I':
      Serial.println("Run Forward with full speed in auto mode");
      break;
    case 'J':
      Serial.println("Run Backward with full speed in auto mode");
      break;
    case 'K':
      Serial.print("[AUTO MODE]turn to angle ");
      Serial.println(currentAngle - 90);
      break;
    case 'L':
      Serial.print("[AUTO MODE]turn to angle ");
      Serial.println(currentAngle + 90);
      break;
    case 'H':
      Serial.print("[AUTO MODE]Stop the car");
      break;
  }
}

void clear_eeprom()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}
