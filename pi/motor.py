# motor.py
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)

# since raspberry pi 3 only have 2 PWM output
# so rpi3 can only control the speed of 2 motors

# DC or Step motor control pins
MOTOR_CLK = 7     # pin 4 on the shield
MOTOR_ENABLE = 11 # pin 7 on the shield
MOTOR_DATA = 13   # pin 8 on the shield
MOTOR_LATCH = 15  # pin 12 on the shield

# DC or Step motor PWM signal pins
# DC Motor#1 / Step Motor#1 PWM: pin 11 on the shield
# DC Motor#2 / Step Motor#1 PWM: pin 3 on the shield
# DC Motor#3 / Step Motor#2 PWM: pin 5 on the shield
# DC Motor#4 / Step Motor#2 PWM: pin 6 on the shield
PWM_1 = 16
PWM_2 = 18

# Not sure what these number for, need further investigation
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
RELEASE = 4

class motor:
	def __init__(self, num):
		self.number = num
		if num == 1:
			self.A = MOTOR1_A
			self.B = MOTOR1_B
		elif num == 2:
			self.A = MOTOR2_A
			self.B = MOTOR2_B
		elif num == 3:
			self.A = MOTOR3_A
			self.B = MOTOR3_B
		elif num == 4:
			self.A = MOTOR4_A
			self.B = MOTOR4_B
		else:
			raise Exception('Unsupported Motor Number')

	def __BV(bit):
    return (1 << (bit))