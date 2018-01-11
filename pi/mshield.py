#######
# Author: Pablo J. Rogina
# Date: 26 May 2016
# pablojr@gmail.com
#
# mshield.py
# Python rewrite of C code sample [1]
# to drive Adafruit motor shield v1 from Raspberry
# [1] https://www.raspberrypi.org/forums/viewtopic.php?f=45&t=16118
#######

import RPi.GPIO as GPIO
from time import sleep

TURNING_TIME = .59

GPIO.setmode(GPIO.BOARD)

MOTORLATCH = 11		#(GPIO_GEN0 - Pin 11)
MOTORCLK = 12		#(GPIO_GEN1 - Pin 12)
MOTORENABLE = 13	#(GPIO_GEN2 - Pin 13)
MOTORDATA = 15		#(GPIO_GEN3 - Pin 15)
MOTOR_1_PWM = 16	#(GPIO_GEN4 - Pin 16)
MOTOR_2_PWM = 18	#(GPIO_GEN5 - Pin 18)

MOTOR1_A = 2
MOTOR1_B = 3
MOTOR2_A = 1
MOTOR2_B = 4
MOTOR4_A = 0
MOTOR4_B = 6
MOTOR3_A = 5
MOTOR3_B = 7

FORWARD = 1
BACKWARD = 2
BRAKE = 3
RELEASE = 4

latch_state = 0

def _BV(bit):
    return (1 << (bit))



def disable():
    GPIO.cleanup()



def latch_tx():
    GPIO.output(MOTORLATCH, GPIO.LOW)
    GPIO.output(MOTORDATA, GPIO.LOW)
    for i in range(8):
        GPIO.output(MOTORCLK, GPIO.LOW)
        if (latch_state & _BV(7-i)):
            GPIO.output(MOTORDATA, GPIO.HIGH)
        else:
            GPIO.output(MOTORDATA, GPIO.LOW)
        GPIO.output(MOTORCLK, GPIO.HIGH)
    else:
        GPIO.output(MOTORLATCH, GPIO.HIGH)



def enable():
    #Set up all pins as outputs
    GPIO.setup(MOTORLATCH,  GPIO.OUT)
    GPIO.setup(MOTORENABLE, GPIO.OUT)
    GPIO.setup(MOTORDATA,   GPIO.OUT)
    GPIO.setup(MOTORCLK,    GPIO.OUT)
    GPIO.setup(MOTOR_1_PWM, GPIO.OUT)
    GPIO.setup(MOTOR_2_PWM, GPIO.OUT)
    latch_state = 0
    latch_tx()
    GPIO.output(MOTORENABLE, GPIO.LOW)
    GPIO.output(MOTOR_1_PWM, GPIO.LOW)
    GPIO.output(MOTOR_2_PWM, GPIO.LOW)
    DCMotorInit(1)
    DCMotorInit(2)




def DCMotorInit( num ):
    global latch_state
    if num == 1:
        latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B)
        latch_tx()
    elif num == 2:
        latch_state &= ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B)
        latch_tx()
    elif num == 3:
        latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B)
        latch_tx()
    elif num == 4:
        latch_state &= ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B)
        latch_tx()
    else:
        print 'Unsupported motor number'


def DCMotorRun(num, cmd):
    a = 0
    b = 0
    if num == 1:
        a = MOTOR1_A
        b = MOTOR1_B
    elif num == 2:
        a = MOTOR2_A
        b = MOTOR2_B
    elif num == 3:
        a = MOTOR3_A
        b = MOTOR3_B
    elif num == 4:
        a = MOTOR4_A
        b = MOTOR4_B
    else:
        print 'Unsupported motor number'
    
    global latch_state 
    if cmd == FORWARD:
        latch_state |= _BV(a)
        latch_state &= ~_BV(b)
        latch_tx()
    elif cmd == BACKWARD:
        latch_state &= ~_BV(a)
        latch_state |= _BV(b)
        latch_tx()
    elif cmd == RELEASE:
        latch_state &= ~_BV(a)
        latch_state &= ~_BV(b)
        latch_tx()
    else:
        print 'Unsupported command'



def forward():
    stop()
    DCMotorRun(1, FORWARD)
    GPIO.output(MOTOR_1_PWM, GPIO.HIGH)
    DCMotorRun(2, FORWARD)
    GPIO.output(MOTOR_2_PWM, GPIO.HIGH)



def backward():
    stop()
    DCMotorRun(1, BACKWARD)
    GPIO.output(MOTOR_1_PWM, GPIO.HIGH)
    DCMotorRun(2, BACKWARD)
    GPIO.output(MOTOR_2_PWM, GPIO.HIGH)



def stop():
    DCMotorRun(1, RELEASE)
    DCMotorRun(2, RELEASE)



def turnLeft():
    DCMotorRun(2, RELEASE)
    DCMotorRun(1, FORWARD)
    sleep(TURNING_TIME)
    DCMotorRun(1, RELEASE)



def turnRight():
    DCMotorRun(1, RELEASE)
    DCMotorRun(2, FORWARD)
    sleep(TURNING_TIME)
    DCMotorRun(2, RELEASE)


