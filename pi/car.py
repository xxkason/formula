# encoding:utf8  

from time import sleep
import RPi.GPIO as GPIO
from enum import Enum, unique

GPIO.setmode(GPIO.BOARD)

@unique
class Run_Direction(Enum):
  FORWARD = 0
  BACKWARD = 1

@unique
class Turn_Direction(Enum):
  LEFT = 0
  RIGHT = 1

class Car(object):
    """
    this is an absctract class for all types of cars
    """

    def stop(self):
        raise NotImplementedError

    def run(self, direction):
        raise NotImplementedError

    def turn(self, direction):
        raise NotImplementedError

    def changeSpeed(self, speed):
        raise NotImplementedError

    def __del__(self):
        self.stop()

class Car_2DC(Car):
    """
    Car with 2 DC Motor, one for wheel, the other one for drive,
    Use L293D/L298N/SimpleBoard motion board solution
    
    Use L293D solution, create instance with 4 parameters:
    e.g. Car_2DC(wheel_motor_number, wheel_pwm_pin, drive_motor_number, drive_pwm_pin)
    
    use L298N solution, create instance with 6 parameters:
    e.g. Car_2DC(in1_pin, in2_pin, pwm1_pin, in3_pin, in4_pin, pwm2_pin)

    use Simple Board solution, create instance with 5 parameters:
    e.g. Car_2DC(in1_pin, in2_pin, in3_pin, in4_pin, pwm_enable)
    """
    from motor import L293D_Motor, L298N_Motor, SimpleBoard_Motor, Direction
    TURNING_TIME = 0.5

    def __init__(self, *args):
        if len(args) == 6:
            self._wheel = self.L298N_Motor(args[0], args[1], args[2])
            self._drive_motor = self.L298N_Motor(args[3],args[4], args[5])
        elif len(args) == 4:
            # create wheel motor
            self._wheel = self.L293D_Motor(args[0], args[1])
            # create drive motor
            self._drive_motor = self.L293D_Motor(args[2], args[3])
        elif len(args) == 5:
            self._wheel = self.SimpleBoard_Motor(args[0], args[1])
            self._drive_motor = self.SimpleBoard_Motor(args[2], args[3], False)
        else:
            raise ValueError
        # set pwm to limit the wheel motor voltage
        self._wheel.setSpeed(60)

    def stop(self):
        self._drive_motor.stop()
        self._wheel.stop()

    def run(self, direction):
        self._drive_motor.stop()
        if direction == Run_Direction.FORWARD:
            cmd = self.Direction.CLOCKWISE
        elif direction == Run_Direction.BACKWARD:
            cmd = self.Direction.COUNTERCLOCKWISE
        self._drive_motor.rotate(cmd)

    def turn(self, direction):
        self._wheel.stop()
        if direction == Turn_Direction.LEFT:
            self._wheel.rotate(self.Direction.CLOCKWISE)
        elif direction == Turn_Direction.RIGHT:
            self._wheel.rotate(self.Direction.COUNTERCLOCKWISE)
        sleep(Car_2DC.TURNING_TIME)
        self._wheel.stop()

    def changeSpeed(self, speed):
        self._drive_motor.setSpeed(speed)

    def currentSpeed(self):
        return self._drive_motor.currentSpeedValue()

    def gear(self, step):
        self._drive_motor.gear(step)

class Car_4WD(Car):
    """
    Car with 1 servo motor for wheel, and 4 DC motor for drive
    """
    from motor import L293D_Motor, Direction

    def __init__(self, wheel_pwm_pin, lfm_num, lfm_pwm_pin, 
    lbm_num, lbm_pwm_pin, rfm_num, rfm_pwm_pin, rbm_num, rbm_pwm_pin,
    lfm_pwm_en = False, lbm_pwm_en = False,
    rfm_pwm_en = False, rbm_pwm_en = False):
        self._leftFront = self.L293D_Motor(lfm_num, lfm_pwm_pin, lfm_pwm_en)
        self._leftBehind = self.L293D_Motor(lbm_num, lbm_pwm_pin, lbm_pwm_en)
        self._rightFront = self.L293D_Motor(rfm_num, rfm_pwm_pin, rfm_pwm_en)
        self._rightBehind = self.L293D_Motor(rbm_num, rbm_pwm_pin, rbm_pwm_en)
        GPIO.setup(wheel_pwm_pin, GPIO.OUT)
        self._wheel_pwm = GPIO.PWM(wheel_pwm_pin, 1000)

    def stop(self):
        self._leftFront.stop()
        self._rightFront.stop()
        self._leftBehind.stop()
        self._rightBehind.stop()
        self._wheel_pwm.stop()

    def run(self, direction):
        self._leftFront.stop()
        self._rightFront.stop()
        self._leftBehind.stop()
        self._rightBehind.stop()
        if direction == Run_Direction.FORWARD:
            cmd = self.Direction.CLOCKWISE
        elif direction == Run_Direction.BACKWARD:
            cmd = self.Direction.COUNTERCLOCKWISE
        self._leftFront.rotate(cmd)
        self._rightFront.rotate(cmd)
        self._rightBehind.rotate(cmd)
        self._leftBehind.rotate(cmd)

    def turn(self, angle):
        self._wheel_pwm.start(angle)

    def changeSpeed(self, speed):
        pass