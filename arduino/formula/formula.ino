#include <SoftwareSerial.h>
#include "Car_2DC.h"

#define SERVO_PWM_PIN 9
#define BAUD_RATE 9600
#define MAX_POSITION 180
#define MIN_POSITION 0
#define CENTER_POSITION 90

Car_2DC car(1,2);
char cmd[3];
int inputSize = 0;

void setup()
{
  Serial.begin(BAUD_RATE);
}

void loop() {
  if (Serial.available() > 0)
  {
    inputSize = Serial.readBytesUntil('\n', cmd, 3);
    //Serial.readBytes();
    Serial.println(cmd);
    Serial.println(inputSize);
    //cmd.toInt();
    if (inputSize == 1)
    {
      switch (cmd[0]) {
        case 'f':
          car.run(QIAN);
          Serial.println("forward");
          break;
        case 'b':
          car.run(HOU);
          Serial.println("backward");
          break;
        case 'l':
          car.turn(ZUO);
          Serial.println("turn left");
          break;
        case 'r':
          car.turn(YOU);
          Serial.println("turn right");
          break;
        case 's':
          car.stop();
          Serial.println("stop");
          break;
      }
    }
  }
}

//SoftwareSerial mySerial(10, 11); // RX, TX
//Servo wheel;
//AF_DCMotor lfMotor(1);
//AF_DCMotor lbMotor(2);
//AF_DCMotor rfMotor(3);
//AF_DCMotor rbMotor(4);
//
//String data = "";
//uint8_t speed = MAX_SPEED;
//int wheelAngle = CENTER_POSITION;
//
//void setup() {
//  wheel.attach(SERVO_PWM_PIN);
//  lfMotor.setSpeed(MAX_SPEED);
//  lbMotor.setSpeed(MAX_SPEED);
//  rfMotor.setSpeed(MAX_SPEED);
//  rbMotor.setSpeed(MAX_SPEED);
//  wheel.write(wheelAngle);
//  lfMotor.run(RELEASE);
//  lbMotor.run(RELEASE);
//  rfMotor.run(RELEASE);
//  rbMotor.run(RELEASE);
//
//  // Open serial communications and wait for port to open:
//  Serial.begin(57600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//  Serial.println("Goodnight moon!");
//  mySerial.begin(BAUD_RATE);
//}
//
//void loop() {
//  if (mySerial.available() > 0)
//  {
//    data = mySerial.readString();
//    Serial.println(data);
//
//    if (data.startsWith("s:"))
//    {
//      char cmd;
//      cmd = data.charAt(2);
//      switch (cmd) {
//        case 'f':
//          goForward();
//          Serial.println("forward");
//          break;
//        case 'b':
//          goBackward();
//          Serial.println("backward");
//          break;
//        case 'l':
//          turn(5);
//          Serial.println("turn left");
//          break;
//        case 'r':
//          turn(-5);
//          Serial.println("turn right");
//          break;
//        case 's':
//          stop();
//          Serial.println("stop");
//          break;
//        case 'c':
//          wheel.write(CENTER_POSITION);
//          Serial.println("centered");
//          break;
//      }
//    }
//    else if (data.startsWith("i:"))
//    {
//      int cmd;
//      cmd = data.substring(2).toInt();
//      setSpeed(cmd);
//      Serial.print("Set speed to");
//      Serial.println(cmd);
//    }
//  }
//}
//
//void goForward()
//{
//  lfMotor.run(FORWARD);
//  rfMotor.run(FORWARD);
//  rbMotor.run(FORWARD);
//  lbMotor.run(FORWARD);
//}
//
//void goBackward()
//{
//  lfMotor.run(BACKWARD);
//  rfMotor.run(BACKWARD);
//  rbMotor.run(BACKWARD);
//  lbMotor.run(BACKWARD);
//}
//
//void gear(int step)
//{
//  if (step == 0)
//  {
//    return;
//  }
//  speed += step;
//  if (speed > MAX_SPEED)
//  {
//    speed = MAX_SPEED;
//  }
//  else if (speed < MIN_SPEED)
//  {
//    speed = MIN_SPEED;
//  }
//
//  setSpeed(speed);
//}
//
//void stop()
//{
//  lfMotor.run(RELEASE);
//  rfMotor.run(RELEASE);
//  rbMotor.run(RELEASE);
//  lbMotor.run(RELEASE);
//}
//
//void turn(int step)
//{
//  if (step == 0)
//  {
//    return;
//  }
//  wheelAngle += step;
//  if (wheelAngle > MAX_POSITION)
//  {
//    wheelAngle = MAX_POSITION;
//  }
//  else if (speed < MIN_POSITION)
//  {
//    wheelAngle = MIN_POSITION;
//  }
//  wheel.write(wheelAngle);
//}
//
//void setSpeed(int speed)
//{
//  lfMotor.setSpeed(speed);
//  rfMotor.setSpeed(speed);
//  rbMotor.setSpeed(speed);
//  lbMotor.setSpeed(speed);
//}
