from motor import Motor
import RPi.GPIO as GPIO

PWM_1 = 16
PWM_2 = 18

wheel = Motor(1, PWM_1)
driver = Motor(2, PWM_2)
wheel.setSpeed(15)
init.car()

try:
    while(True):
        cmd = raw_input('f: go forward; b: go backward; s: stop; l: turn left; r: turn right')
        if cmd == 'f':
            go_forward()
        elif cmd == 'b':
            go_backward()
        elif cmd == 'l':
            turn_left()
        elif cmd == 'r':
            turn_right()
        else:
            print "Unsupported command"
except KeyboardInterrupt:
    driver.stop()
    wheel.stop()
	GPIO.cleanup()
	print("Exiting...")

def init_car():
    GPIO.setup(Motor.MOTOR_CLK, GPIO.OUT)
    GPIO.setup(Motor.MOTOR_DATA, GPIO.OUT)
    GPIO.setup(Motor.MOTOR_ENABLE, GPIO.OUT, initial = GPIO.LOW)
    GPIO.setup(Motor.MOTOR_LATCH, GPIO.OUT)
    GPIO.setup(PWM_1, GPIO.OUT)
    GPIO.setup(PWM_2, GPIO.OUT)

def go_forward():
    driver.stop()
    driver.run(Motor.FORWARD)

def go_backward():
    driver.stop()
    driver.run(Motor.BACKWARD)

def stop():
    driver.stop()

def changeSpeed(speed):
    driver.setSpeed(speed)

def turn_left():
    wheel.run(Motor.FORWARD)

def turn_right():
    wheel.run(Motor.BACKWARD)
