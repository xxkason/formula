#include <EEPROM.h>

/* Gamepad msg info
     Left           Right
      A               I
    C   D   G   H   K   L
      B               J

      E               M
      F               N
  A,B,C,D,E,F,M,N continuous send cmd when pressed */

#include "Car_4WD.h"

#define GAMEPAD_BAUD_RATE 115200
#define CAR_MODE_INDICATOR_PIN 2
#define PAD_MODE_INDICATOR_PIN 13

enum PadMode
{
  LEFT_RIGHT_STICK = 0,
  LEFT_PAD_KEY = 1
};

enum CarMode
{
  MANUAL = 0,
  RECORD = 1,
  REPLAY = 2
};

//SoftwareSerial softSerial(10, 11); // RX, TX
Car *btcar;
Car_4WD car4wd(1, 2, 3, 4, 10);
PadMode padMode = LEFT_RIGHT_STICK;
CarMode carMode = MANUAL;
int romAddr = 0;
unsigned long beginTime;

// https://www.arduino.cc/en/Reference/PortManipulation
// PORTD maps to Arduino digital pins 0 to 7
//     DDRD - The Port D Data Direction Register - read/write
//     PORTD - The Port D Data Register - read/write
//     PIND - The Port D Input Pins Register - read only 

// PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable
//     DDRB - The Port B Data Direction Register - read/write
//     PORTB - The Port B Data Register - read/write
//     PINB - The Port B Input Pins Register - read only 

// PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini
//     DDRC - The Port C Data Direction Register - read/write
//     PORTC - The Port C Data Register - read/write
//     PINC - The Port C Input Pins Register - read only 

// http://gammon.com.au/interrupts
// Below is a list of interrupts, in priority order, for the Atmega328:
//  1  Reset 
//  2  External Interrupt Request 0  (pin D2)          (INT0_vect)
//  3  External Interrupt Request 1  (pin D3)          (INT1_vect)
//  4  Pin Change Interrupt Request 0 (pins D8 to D13) (PCINT0_vect)
//  5  Pin Change Interrupt Request 1 (pins A0 to A5)  (PCINT1_vect)
//  6  Pin Change Interrupt Request 2 (pins D0 to D7)  (PCINT2_vect)
//  7  Watchdog Time-out Interrupt                     (WDT_vect)
//  8  Timer/Counter2 Compare Match A                  (TIMER2_COMPA_vect)
//  9  Timer/Counter2 Compare Match B                  (TIMER2_COMPB_vect)
// 10  Timer/Counter2 Overflow                         (TIMER2_OVF_vect)
// 11  Timer/Counter1 Capture Event                    (TIMER1_CAPT_vect)
// 12  Timer/Counter1 Compare Match A                  (TIMER1_COMPA_vect)
// 13  Timer/Counter1 Compare Match B                  (TIMER1_COMPB_vect)
// 14  Timer/Counter1 Overflow                         (TIMER1_OVF_vect)
// 15  Timer/Counter0 Compare Match A                  (TIMER0_COMPA_vect)
// 16  Timer/Counter0 Compare Match B                  (TIMER0_COMPB_vect)
// 17  Timer/Counter0 Overflow                         (TIMER0_OVF_vect)
// 18  SPI Serial Transfer Complete                    (SPI_STC_vect)
// 19  USART Rx Complete                               (USART_RX_vect)
// 20  USART, Data Register Empty                      (USART_UDRE_vect)
// 21  USART, Tx Complete                              (USART_TX_vect)
// 22  ADC Conversion Complete                         (ADC_vect)
// 23  EEPROM Ready                                    (EE_READY_vect)
// 24  Analog Comparator                               (ANALOG_COMP_vect)
// 25  2-wire Serial Interface  (I2C)                  (TWI_vect)
// 26  Store Program Memory Ready                      (SPM_READY_vect)
//volatile boolean leftSensor = false;
//volatile boolean rightSensor = false;
//
//ISR (PCINT1_vect)
//{
//  leftSensor = (PINC & bit(0));
//  rightSensor = (PINC & bit(5));
//  if (leftSensor & rightSensor)
//    car4wd.turn(CENTER_POSITION);
//  else if (~leftSensor & rightSensor)
//    car4wd.turn(RIGHT_POSITION);
//  else if (leftSensor & ~rightSensor)
//    car4wd.turn(LEFT_POSITION);
//  else
//    car4wd.stop();
//}

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
  pinMode(CAR_MODE_INDICATOR_PIN, OUTPUT);
  digitalWrite(CAR_MODE_INDICATOR_PIN, LOW);
  pinMode(PAD_MODE_INDICATOR_PIN, OUTPUT);
  digitalWrite(PAD_MODE_INDICATOR_PIN, HIGH);
//   pinMode(A0, INPUT);
//   pinMode(A1, INPUT);
//  DDRD |= (1<<DDD2); // Set digital pin 2 to output mode
//  DDRB |= (1<<DDB5); // Set digital pin 13 to output mode
//  DDRC &= ~(1<<DDC0); // Set analog pin A0 to input mode
//  DDRC &= ~(1<<DDC5); // Set analog pin A5 to input mode
  // PORTD &= ~(1<<PORTD2); // make digital pin 2 low
  // PORTB &= ~(1<<PORTB5); // make digital pin 13 low
  
  // pin change interrupt
  // PCMSK1 |= bit (PCINT8); // want pin A0
  // PCMSK1 |= bit (PCINT13); // want pin A5
  // PCIFR  |= bit (PCIF1);   // clear any outstanding interrupts
  // PCICR  |= bit (PCIE1);   // enable pin change interrupts for A0 to A5
  Serial.setTimeout(300);
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
    if (cmd == 'G') // switch car mode
    {
      carMode = CarMode((carMode + 1) % 3);
      switch (carMode)
      {
        case MANUAL:
          PORTD &= ~(1<<PORTD2); // make digital pin 2 low
          break;
        case RECORD:
          //PORTD |= (1<<PORTD2); // make digital pin 2 high
          PORTB &= ~(1<<PORTB5); // make digital pin 13 low
          break;
        case REPLAY:
          PORTD |= (1<<PORTD2); // make digital pin 2 high
          break;
      }
    }
    else
    {
      switch (carMode)
      {
        case MANUAL:
          manualControl(cmd);
          break;
        case REPLAY:
          if (cmd == 'H') // 'H' is the command to start replay in REPLAY mode
            replay();
          break;
        case RECORD:
          if (cmd == 'H') // 'H' is the command to start record in RECORD mode
            record();
          break;
      }
    }
  }
  else if (carMode == MANUAL && padMode == LEFT_PAD_KEY)
  {
    btcar->stop();
    btcar->turn(CENTER_POSITION);
  }
  else if (carMode == RECORD)
    ledBlink(CAR_MODE_INDICATOR_PIN, 1000);
}

void replay()
{
  for (int index = 0; index < EEPROM.length() ; index++) {
    if (EEPROM[index] == 0) {
      btcar->stop();
      btcar->turn(CENTER_POSITION);
      break;
    }
    rightPadKey(EEPROM[index]);
    byte high = EEPROM.read(++index);
    byte low = EEPROM.read(++index);
    unsigned int duration = word(high, low);
    delay(duration);
  }
}

void record()
{
  PORTD &= ~(1<<PORTD2); // make digital pin 2 low, turn off the led to indicate user eeprom clear is started
  clearEEPROM();
  digitalWrite(CAR_MODE_INDICATOR_PIN, HIGH); // turn on the led to indicate user eeprom clear is done
  beginTime = 0;
  romAddr = 0;

  while (true)
  {
    char learnCmd[1];
    if (Serial.readBytes(learnCmd, 1))
    {
      char cmdValue = learnCmd[0];
      if (cmdValue == 'G')
      {
        // stop learning
        btcar->stop();
        btcar->turn(CENTER_POSITION);
        PORTD &= ~(1<<PORTD2); // make digital pin 2 low
        break;
      }
      switch (cmdValue)
      {
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'H':
          if (romAddr < EEPROM.length())
          {
            if (beginTime != 0)
            {
              unsigned int duration = millis() - beginTime;
              EEPROM.write(romAddr++, highByte(duration));
              EEPROM.write(romAddr++, lowByte(duration));
            }
            EEPROM.write(romAddr++, cmdValue);
            rightPadKey(cmdValue);
            beginTime = millis();
          }
          break;
      }
    }
    else
      ledBlink(CAR_MODE_INDICATOR_PIN, 500);
  }
}

void manualControl(char cmd)
{
  if (cmd == 'H') // switch gamepad control mode
  {
    padMode = PadMode((padMode + 1) % 2);
    switch (padMode)
    {
      case LEFT_RIGHT_STICK:
        digitalWrite(PAD_MODE_INDICATOR_PIN, HIGH);
        break;
      case LEFT_PAD_KEY:
        digitalWrite(PAD_MODE_INDICATOR_PIN, LOW);
        break;
    }
    btcar->changeSpeed(255);
  }
  else
  {
    switch (padMode)
    {
      case LEFT_RIGHT_STICK:
        leftRightAnalogStick(cmd);
        break;
      case LEFT_PAD_KEY:
        leftPadKey(cmd);
        break;
    }
  }
}

void leftRightAnalogStick(char cmd)
{
  switch (cmd)
  {
    case 'W':
      byte speed;
      speed = Serial.parseInt();

      // clear the followed message "Pxxx\n"
      Serial.readStringUntil('\n');
      // clear end

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

      /*
        128 is the center position value,
        left position is min value = 0,
        right position is max value = 255,
        analog speed change:
        0: map to servo 0
        128: map to servo 90
        255: map to servo 180
      */
      byte angle;
      angle = Serial.parseInt();
      btcar->turn(angle * 180 / 256);
      break;
  }
}

void leftPadKey(char cmd)
{
  switch (cmd)
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
      btcar->turn(LEFT_POSITION);
      //Serial.println("Turn Left");
      break;
    case 'r':
    case 'D':
      btcar->turn(RIGHT_POSITION);
      //Serial.println("Turn Right");
      break;
    case 'F':
      car4wd.uturn(FOR);
      break;
    case 'N':
      car4wd.uturn(BACK);
      break;
  }
}

void rightPadKey(char cmd)
{
  switch (cmd)
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
      btcar->turn(btcar->currentAngle() - 90);
      //Serial.println("Turn Left");
      break;
    case 'L':
      btcar->turn(btcar->currentAngle() + 90);
      //Serial.println("Turn Right");
      break;
    case 'E':
    case 'H':
      btcar->stop();
      btcar->turn(CENTER_POSITION);
      break;
  }
}

void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void ledBlink(byte ledPin, unsigned int period)
{
  digitalWrite(ledPin, LOW);
  delay(period / 2);
  digitalWrite(ledPin, HIGH);
  delay(period / 2);
}
