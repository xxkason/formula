# encoding:utf8  
# motor.py
import RPi.GPIO as GPIO
from enum import Enum, unique

GPIO.setmode(GPIO.BOARD)

# since raspberry pi 3 have only 2 PWM outputs
# so rpi3 can only control the speed of 2 motors

# DC or Step motor PWM signal pins
# DC Motor#1 / Step Motor#1 PWM: pin 11 on the shield
# DC Motor#2 / Step Motor#1 PWM: pin 3 on the shield
# DC Motor#3 / Step Motor#2 PWM: pin 5 on the shield
# DC Motor#4 / Step Motor#2 PWM: pin 6 on the shield

# 1KHz is used by default.
# Frequencies for motor 1 & 2 are:
# 	MOTOR12_64KHZ
# 	MOTOR12_8KHZ
# 	MOTOR12_2KHZ
# 	MOTOR12_1KHZ

# Frequencies for motor 3 & 4 are:
# 	MOTOR34_64KHZ
# 	MOTOR34_8KHZ
# 	MOTOR34_1KHZ

@unique
class MotorState(Enum):
  Running = 0
  Stopped = 1

class Motor(object):
	# DC or Step motor control pins
	MOTOR_CLK = 7     # pi GPIO4 --> shield pin 4
	MOTOR_ENABLE = 11 # pi GPIO17 --> shield pin 7
	MOTOR_DATA = 13   # pi GPIO27 --> shield pin 8
	MOTOR_LATCH = 15  # pi GPIO22 --> shield pin 12

	# Bit positions in the 74HCT595 shift register output
	MOTOR1_A = 2
	MOTOR1_B = 3
	MOTOR2_A = 1
	MOTOR2_B = 4
	MOTOR3_A = 5
	MOTOR3_B = 7
	MOTOR4_A = 0
	MOTOR4_B = 6

	# Command
	FORWARD = 1
	BACKWARD = 2

	GPIO.setup(MOTOR_CLK, GPIO.OUT)
	GPIO.setup(MOTOR_DATA, GPIO.OUT)
	GPIO.setup(MOTOR_ENABLE, GPIO.OUT, initial = GPIO.LOW)
	GPIO.setup(MOTOR_LATCH, GPIO.OUT)

	def __init__(self, num, pwm_pin, fake_pwm = False, pwm_frequency  = 1000):
		self._latch_state = 0
		self._state = MotorState.Stopped
		self._pwm_pin = pwm_pin
		GPIO.setup(self._pwm_pin, GPIO.OUT)
		self._fake_pwm = fake_pwm
		if not self._fake_pwm:
			self._pwm = GPIO.PWM(self._pwm_pin, pwm_frequency)
			self._speed = 50
		if num == 1:
			self._a = Motor.MOTOR1_A
			self._b = Motor.MOTOR1_B
		elif num == 2:
			self._a = Motor.MOTOR2_A
			self._b = Motor.MOTOR2_B
		elif num == 3:
			self._a = Motor.MOTOR3_A
			self._b = Motor.MOTOR3_B
		elif num == 4:
			self._a = Motor.MOTOR4_A
			self._b = Motor.MOTOR4_B
		else:
			raise Exception('Unsupported Motor Number')
		self.number = num
		self._latch_state &= ~self.__BV(self._a) & ~self.__BV(self._b)
		self._latch_tx()

	def __del__(self):
		self.stop()

	def __BV(self, bit):
		return (1 << (bit))

	def _latch_tx(self):
		GPIO.output(Motor.MOTOR_LATCH, GPIO.LOW)
		GPIO.output(Motor.MOTOR_DATA, GPIO.LOW)

		for i in range(8):
			GPIO.output(Motor.MOTOR_CLK, GPIO.LOW)
			if (self._latch_state & self.__BV(7-i)):
				GPIO.output(Motor.MOTOR_DATA, GPIO.HIGH)
			else:
				GPIO.output(Motor.MOTOR_DATA, GPIO.LOW)
			GPIO.output(Motor.MOTOR_CLK, GPIO.HIGH)

		GPIO.output(Motor.MOTOR_LATCH, GPIO.HIGH)

	def stop(self):
		self._latch_state &= ~self.__BV(self._a)
		self._latch_state &= ~self.__BV(self._b)
		self._latch_tx()
		if self._fake_pwm:
			GPIO.output(self._pwm_pin, GPIO.LOW)
		else:
			self._pwm.stop()
		self._state = MotorState.Stopped
	
	def run(self, direction):
		if direction == Motor.FORWARD:
			self._latch_state |= self.__BV(self._a)
			self._latch_state &= ~self.__BV(self._b)
		elif direction == Motor.BACKWARD:
			self._latch_state &= ~self.__BV(self._a)
			self._latch_state |= self.__BV(self._b)
		self._latch_tx()
		if self._fake_pwm:
			GPIO.output(self._pwm_pin, GPIO.HIGH)
		else:
			self._pwm.start(self._speed)
		self._state = MotorState.Running

	def getSpeed(self):
		if self._state:
			return 0
		else:
			return self._speed

	def setSpeed(self, speed):
		if self._state:
			print "Unable to set motor speed when it is stopped"
			return
		if self._fake_pwm:
			print "Unable to change the motor speed with fake pwm signal"
			return
		if speed < 0:
			self._speed = 0
		elif speed > 100:
			self._speed = 100
		else:
			self._speed = speed
		self._pwm.ChangeDutyCycle(self._speed)

	def gear(self, step = 0):
		if (step == 0):
			return
		self.setSpeed(self._speed + step)
