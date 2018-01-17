#include <AFMotor.h>
#include <Servo.h>

#define SERVO_PWM_PIN 9
#define MAX_SPEED 255
#define MIN_SPEED 50
#define BAUD_RATE 9600
#define MAX_POSITION 180
#define MIN_POSITION 0
#define CENTER_POSITION 90

Servo wheel;
AF_DCMotor lfMotor(1);
AF_DCMotor lbMotor(2);
AF_DCMotor rfMotor(3);
AF_DCMotor rbMotor(4);
char cmd[1];
uint8_t speed = MAX_SPEED;
int wheelAngle = CENTER_POSITION;

void setup() {
  wheel.attach(SERVO_PWM_PIN);
  lfMotor.setSpeed(MAX_SPEED);
  lbMotor.setSpeed(MAX_SPEED);
  rfMotor.setSpeed(MAX_SPEED);
  rbMotor.setSpeed(MAX_SPEED);
  wheel.write(wheelAngle);
  lfMotor.run(RELEASE);
  lbMotor.run(RELEASE);
  rfMotor.run(RELEASE);
  rbMotor.run(RELEASE);
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(200);
}

void loop() {
  if (Serial.readBytes(cmd, 1) > 0)
  {
    switch (cmd[0]) {
      case 'f':
        goForward();
        break;
      case 'b':
        goForward();
        break;
      case 'l':
        turn(-5);
        break;
      case 'r':
        turn(5);
        break;
      case 's':
        stop();
        break;
      case 'a':
        gear(5);
        break;
      case 'd':
        gear(-5);
        break;
    }
  }
  else
  {
    wheel.write(CENTER_POSITION);
  }
  
//  if (Serial.available() > 0)
//  {
//    cmd = Serial.read();
//    switch (cmd) {
//      case 'f':
//        goForward();
//        break;
//      case 'b':
//        goForward();
//        break;
//      case 'l':
//        turnLeft();
//        break;
//      case 'r':
//        turnRight();
//        break;
//      case 's':
//        stop();
//        break;
//      case 'a':
//        gear(5);
//        break;
//      case 'd':
//        gear(-5);
//        break;
//    }
//  }
}

void goForward()
{
  lfMotor.run(FORWARD);
  rfMotor.run(FORWARD);
  rbMotor.run(FORWARD);
  lbMotor.run(FORWARD);
}

void goBackward()
{
  lfMotor.run(BACKWARD);
  rfMotor.run(BACKWARD);
  rbMotor.run(BACKWARD);
  lbMotor.run(BACKWARD);
}

void gear(int step)
{
  if (step == 0)
  {
    return;
  }
  speed += step;
  if (speed > MAX_SPEED)
  {
    speed = MAX_SPEED;
  }
  else if (speed < MIN_SPEED)
  {
    speed = MIN_SPEED;
  }

  lfMotor.setSpeed(speed);
  rfMotor.setSpeed(speed);
  rbMotor.setSpeed(speed);
  lbMotor.setSpeed(speed);
}

void stop()
{
  lfMotor.run(RELEASE);
  rfMotor.run(RELEASE);
  rbMotor.run(RELEASE);
  lbMotor.run(RELEASE);
}

void turn(int step)
{
  if (step == 0)
  {
    return;
  }
  wheelAngle += step;
  if (wheelAngle > MAX_POSITION)
  {
    wheelAngle = MAX_POSITION;
  }
  else if (speed < MIN_POSITION)
  {
    wheelAngle = MIN_POSITION;
  }
  wheel.write(wheelAngle);
}