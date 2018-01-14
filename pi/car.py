# encoding:utf8  

import bluetooth
from motor import Motor
from time import sleep
import RPi.GPIO as GPIO

BT_SERVER_PORT = 1
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

    def currentSpeed(self):
        return self.__drive_motor.getSpeed()

    def goForward(self):
        self.__drive_motor.stop()
        self.__drive_motor.run(Motor.FORWARD)

    def goBackward(self):
        self.__drive_motor.stop()
        self.__drive_motor.run(Motor.BACKWARD)

    def stop(self):
        self.__drive_motor.stop()
        self.__wheel.stop()

    def changeSpeed(self, speed):
        self.__drive_motor.setSpeed(speed)

    def gear(self, step):
        self.__drive_motor.gear(step)

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
    def __init__(self, wheel_pwm_pin, 
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

def main():
    car = ToyCar(1, WHEEL_PWM, 2, PWM_2)

    server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    server_socket.bind(("", BT_SERVER_PORT))
    server_socket.listen(1) # Server listens to accept 1 connection at a time

    print "Waiting for a connection..."
    client_socket, address = server_socket.accept()
    print "Welcome ", address
    try:
        while True:
            cmd = client_socket.recv(128)
            #print "Received command: %s" % cmd
            if cmd == 'f':
                print "Yes, my lord. Go! Go! Go!"
                car.goBackward()
            elif cmd == 'b':
                print "Yes, my lord. Go backward"
                car.goBackward()
            elif cmd == 'l':
                print "Turning left"
                car.turnLeft()
            elif cmd == 'r':
                print "Turning right"
                car.turnRight()
            elif cmd == 's':
                print "Stop now!"
                car.stop()
            elif cmd == 'acc':
                print "Speed Up"
                car.gear(5)
                print "Current speed is ",car.currentSpeed()
            elif cmd == 'deacc':
                print "Slow down..."
                car.gear(-5)
                print "Current speed is ",car.currentSpeed()
            elif cmd == 'q':
                print "Quit"
                car.stop()
                break
    except KeyboardInterrupt:
        car.stop()
        GPIO.cleanup()
        print("Exiting...")
    
    GPIO.cleanup()
    client_socket.close()
    server_socket.close()

# toyCar = ToyCar(1, WHEEL_PWM, 2, PWM_2)
# raceCar = RaceCar(WHEEL_PWM, 1, FAKE_PWM_1, 2, FAKE_PWM_2, 
#                   3, FAKE_PWM_3, 4, FAKE_PWM_4, True, True, True, True)

if __name__ == "__main__":
    main()