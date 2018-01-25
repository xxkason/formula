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
class Direction(Enum):
  CLOCKWISE = 0
  COUNTERCLOCKWISE = 1

class Motor(object):
	def __init__(self, pwm_pin, pwm_enable = True, pwm_frequency = 1000):
		self._pwm_pin = pwm_pin
		self._pwm_en = pwm_pin
		GPIO.setup(self._pwm_pin, GPIO.OUT)
		if self._pwm_en:
			self._pwm_frequency = pwm_frequency
			self._pwm = GPIO.PWM(self._pwm_pin, self._pwm_frequency)
			self._speed = 60

	def __del__(self):
		self.stop()

	def stop(self):
		raise NotImplementedError

	def rotate(self, direction):
		raise NotImplementedError

	def setSpeed(self, speed):
		if not self._pwm_en:
			print ("Unable to change the motor speed with pwm disabled.")
			return
		if speed < 0:
			self._speed = 0
		elif speed > 100:
			self._speed = 100
		else:
			self._speed = speed
		self._pwm.ChangeDutyCycle(self._speed)

	def currentSpeedValue(self):
		return self._speed

class L293D_Motor(Motor):
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

	GPIO.setup(MOTOR_CLK, GPIO.OUT)
	GPIO.setup(MOTOR_DATA, GPIO.OUT)
	GPIO.setup(MOTOR_ENABLE, GPIO.OUT, initial = GPIO.LOW)
	GPIO.setup(MOTOR_LATCH, GPIO.OUT)

	def __init__(self, num, pwm_pin, pwm_enable = True, pwm_frequency  = 1000):
		super(L293D_Motor, self).__init__(pwm_pin, pwm_enable, pwm_frequency)
		self._latch_state = 0
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
		if self._pwm_en:
			self._pwm.stop()
		else:
			GPIO.output(self._pwm_pin, GPIO.LOW)
	
	def rotate(self, direction):
		if direction == Direction.CLOCKWISE:
			self._latch_state |= self.__BV(self._a)
			self._latch_state &= ~self.__BV(self._b)
		elif direction == Direction.COUNTERCLOCKWISE:
			self._latch_state &= ~self.__BV(self._a)
			self._latch_state |= self.__BV(self._b)
		self._latch_tx()
		if self._pwm_en:
			self._pwm.start(self._speed)
		else:
			GPIO.output(self._pwm_pin, GPIO.HIGH)	

	def gear(self, step = 0):
		if (step == 0):
			return
		self.setSpeed(self._speed + step)

class L298N_Motor(Motor):
	def __init__(self, in1_pin, in2_pin, enable_pin):
		super(L298N_Motor, self).__init__(enable_pin)
		self._in1_pin = in1_pin
		self._in2_pin = in2_pin
		GPIO.setup(self._in1_pin, GPIO.OUT, initial = GPIO.LOW)
		GPIO.setup(self._in2_pin, GPIO.OUT, initial = GPIO.LOW)

	def stop(self):
		GPIO.output(self._in1_pin, GPIO.LOW)
		GPIO.output(self._in2_pin, GPIO.LOW)
		self._pwm.stop()
	
	def rotate(self, direction):
		if direction == Direction.CLOCKWISE:
			GPIO.output(self._in1_pin, GPIO.HIGH)
			GPIO.output(self._in2_pin, GPIO.LOW)
		elif direction == Direction.COUNTERCLOCKWISE:
			GPIO.output(self._in1_pin, GPIO.LOW)
			GPIO.output(self._in2_pin, GPIO.HIGH)
		self._pwm.start(self._speed)

	def brake(self):
		GPIO.output(self._in1_pin, GPIO.HIGH)
		GPIO.output(self._in2_pin, GPIO.HIGH)
		self._pwm.stop()