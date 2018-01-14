# encoding:utf8  

from motor import Motor
from time import sleep
import RPi.GPIO as GPIO

WHEEL_PWM = 16 # GPIO23
PWM_2 = 18 # GPIO24
FAKE_PWM_1 = 22 # GPIO25
FAKE_PWM_2 = 32 # GPIO12
FAKE_PWM_3 = 36 # GPIO16
FAKE_PWM_4 = 38 # GPIO20

# ToyCar class
# 两个直流电机
# 一个当舵机，负责转向，但不能调节角度
# 一个驱动电机
class ToyCar(object):
    TURNING_TIME = 0.5

    def __init__(self, wheel_motor_number, wheel_pwm_pin, 
    driver_motor_num, driver_pwm_pin, default_wheel_speed = 60):
        self.__wheel = Motor(wheel_motor_number, wheel_pwm_pin)
        self.__drive_motor = Motor(driver_motor_num, driver_pwm_pin)
        self.__wheel.setSpeed(default_wheel_speed)

    def __del__(self):
        self.stop()

    def goForward(self):
        self.__drive_motor.stop()
        self.__drive_motor.run(Motor.FORWARD)

    def goBackward(self):
        self.__drive_motor.stop()
        self.__drive_motor.run(Motor.BACKWARD)

    def stop(self):
        self.__drive_motor.stop()
        self.__wheel.stop()

    def gear(self, speed):
        self.__drive_motor.setSpeed(speed)

    def turnLeft(self):
        self.__wheel.stop()
        self.__wheel.run(Motor.BACKWARD)
        sleep(ToyCar.TURNING_TIME)
        self.__wheel.stop()

    def turnRight(self):
        self.__wheel.stop()
        self.__wheel.run(Motor.FORWARD)
        sleep(ToyCar.TURNING_TIME)
        self.__wheel.stop()

# RaceCar class
# 1个舵机，5V输出，PWM控制转向角度
# 4个驱动电机，驱动不能调速，全速转动
class RaceCar(object):
    def __init__(self，wheel_pwm_pin, 
    lfm_num, lfm_pwm_pin, 
    lbm_num, lbm_pwm_pin,
    rfm_num, rfm_pwm_pin,
    rbm_num, rbm_pwm_pin,
    lfm_fake_pwm = False, lbm_fake_pwm = False,
    rfm_fake_pwm = False, rbm_fake_pwm = False
    ):
        self.__leftFront = Motor(lfm_num, lfm_pwm_pin, lfm_fake_pwm)
        self.__leftBehind = Motor(lbm_num, lbm_pwm_pin, lbm_fake_pwm)
        self.__rightFront = Motor(rfm_num, rfm_pwm_pin, rfm_fake_pwm)
        self.__rightBehind = Motor(rbm_num, rbm_pwm_pin, rbm_fake_pwm)
        GPIO.setup(wheel_pwm_pin, GPIO.OUT)
        self.__wheelPWM = GPIO.PWM(wheel_pwm_pin, 1000)

    def goForward(self):
        self.__leftFront.stop()
        self.__rightFront.stop()
        self.__leftBehind.stop()
        self.__rightBehind.stop()
        self.__leftFront.run(Motor.FORWARD)
        self.__rightFront.run(Motor.FORWARD)
        self.__rightBehind.run(Motor.FORWARD)
        self.__leftBehind.run(Motor.FORWARD)

    def goBackward(self):
        self.__leftFront.stop()
        self.__rightFront.stop()
        self.__leftBehind.stop()
        self.__rightBehind.stop()
        self.__leftFront.run(Motor.BACKWARD)
        self.__rightFront.run(Motor.BACKWARD)
        self.__rightBehind.run(Motor.BACKWARD)
        self.__leftBehind.run(Motor.BACKWARD)

    def stop(self):
        self.__leftFront.stop()
        self.__rightFront.stop()
        self.__leftBehind.stop()
        self.__rightBehind.stop()
        self.__wheelPWM.stop()

    def gear(self, speed):
        pass

    def wheelControl(self, angle):
        self.__wheelPWM.start(angle)

toyCar = ToyCar(1, WHEEL_PWM, 2, PWM_2)
raceCar = RaceCar(WHEEL_PWM, 1, FAKE_PWM_1, 2, FAKE_PWM_2, 
                  3, FAKE_PWM_3, 4, FAKE_PWM_4, True, True, True, True)

# try:
#     while(True):
#         cmd = raw_input('f: go forward; b: go backward; s: stop; l: turn left; r: turn right\n')
#         if cmd == 'f':
#             go_forward()
#         elif cmd == 'b':
#             go_backward()
#         elif cmd == 'l':
#             turn_left()
#         elif cmd == 'r':
#             turn_right()
#         elif cmd == 's':
#             stop()
#         elif cmd == 'q':
#             stop()
#             GPIO.cleanup()
#         else:
#             print "Unsupported command"
# except KeyboardInterrupt:
#     stop()
#     GPIO.cleanup()
#     print("Exiting...")