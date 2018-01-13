# motor.py
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)

# since raspberry pi 3 only have 2 PWM output
# so rpi3 can only control the speed of 2 motors

# DC or Step motor control pins
# MOTOR_CLK = 7     # pin 4 on the shield
# MOTOR_ENABLE = 11 # pin 7 on the shield
# MOTOR_DATA = 13   # pin 8 on the shield
# MOTOR_LATCH = 15  # pin 12 on the shield

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

# PWM_1 = 16
# PWM_2 = 18
# PWM_FREQUENCY = 1000

class Motor(object):
	# DC or Step motor control pins
	MOTOR_CLK = 7     # pin 4 on the shield
	MOTOR_ENABLE = 11 # pin 7 on the shield
	MOTOR_DATA = 13   # pin 8 on the shield
	MOTOR_LATCH = 15  # pin 12 on the shield

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

	def __init__(self, num, pwm_pin = 16, pwm_frequency  = 1000):
		self.__latch_state = 0
		GPIO.setup(pwm_pin, GPIO.OUT)
		self.__pwm = GPIO.PWM(pwm_pin, pwm_frequency)
		self.__speed = 50
		if num == 1:
			self.__a = Motor.MOTOR1_A
			self.__b = Motor.MOTOR1_B
		elif num == 2:
			self.__a = Motor.MOTOR2_A
			self.__b = Motor.MOTOR2_B
		elif num == 3:
			self.__a = Motor.MOTOR3_A
			self.__b = Motor.MOTOR3_B
		elif num == 4:
			self.__a = Motor.MOTOR4_A
			self.__b = Motor.MOTOR4_B
		else:
			raise Exception('Unsupported Motor Number')
		self.number = num
		self.__latch_state &= ~self.__BV(self.__a) & ~self.__BV(self.__b)
		self.__latch_tx()

	def __del__(self):
		self.__pwm.stop()
		GPIO.cleanup()

	def __BV(self, bit):
		return (1 << (bit))

	def __latch_tx(self):
		GPIO.output(Motor.MOTOR_LATCH, GPIO.LOW)
		GPIO.output(Motor.MOTOR_DATA, GPIO.LOW)

		for i in range(8):
			GPIO.output(Motor.MOTOR_CLK, GPIO.LOW)
			if (self.__latch_state & self.__BV(7-i)):
				GPIO.output(Motor.MOTOR_DATA, GPIO.HIGH)
			else:
				GPIO.output(Motor.MOTOR_DATA, GPIO.LOW)
			GPIO.output(Motor.MOTOR_CLK, GPIO.HIGH)

		GPIO.output(Motor.MOTOR_LATCH, GPIO.HIGH)

	def stop(self):
		self.__latch_state &= ~self.__BV(self.__a)
		self.__latch_state &= ~self.__BV(self.__b)
		self.__latch_tx()
		self.__pwm.stop()
	
	def run(self, direction):
		if direction == Motor.FORWARD:
			self.__latch_state |= self.__BV(self.__a)
			self.__latch_state &= ~self.__BV(self.__b)
		elif direction == Motor.BACKWARD:
			self.__latch_state &= ~self.__BV(self.__a)
			self.__latch_state |= self.__BV(self.__b)
		self.__latch_tx()
		self.__pwm.start(self.__speed)

	def setSpeed(self, speed):
		if speed < 0:
			self.__speed = 0
		elif speed > 100:
			self.__speed = 100
		else:
			self.__speed = speed
		self.__pwm.ChangeDutyCycle(self.__speed)