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

    def __init__(self, wheel_motor_number, wheel_pwm_pin, \
    driver_motor_num, driver_pwm_pin, default_wheel_speed = 60):
        # 转向电机
        self._wheel = Motor(wheel_motor_number, wheel_pwm_pin)
        # 驱动电机
        self._drive_motor = Motor(driver_motor_num, driver_pwm_pin)
        # 设置转向电机电压，通过PWM来改变驱动电压
        self._wheel.setSpeed(default_wheel_speed)

    def __del__(self):
        self.stop()

    def currentSpeed(self):
        return self._drive_motor.getSpeed()

    def goForward(self):
        self._drive_motor.stop()
        self._drive_motor.run(Motor.FORWARD)

    def goBackward(self):
        self._drive_motor.stop()
        self._drive_motor.run(Motor.BACKWARD)

    def stop(self):
        self._drive_motor.stop()
        self._wheel.stop()

    def changeSpeed(self, speed):
        self._drive_motor.setSpeed(speed)

    def gear(self, step):
        self._drive_motor.gear(step)

    def turnLeft(self):
        self._wheel.stop()
        self._wheel.run(Motor.BACKWARD)
        sleep(ToyCar.TURNING_TIME)
        self._wheel.stop()

    def turnRight(self):
        self._wheel.stop()
        self._wheel.run(Motor.FORWARD)
        sleep(ToyCar.TURNING_TIME)
        self._wheel.stop()

# RaceCar class
# 1个舵机，5V输出，PWM控制转向角度
# 4个驱动电机，驱动不能调速，全速转动
class RaceCar(object):
    def __init__(self, wheel_pwm_pin, 
    lfm_num, lfm_pwm_pin, 
    lbm_num, lbm_pwm_pin,
    rfm_num, rfm_pwm_pin,
    rbm_num, rbm_pwm_pin,
    lfm_fake_pwm = True, lbm_fake_pwm = True,
    rfm_fake_pwm = True, rbm_fake_pwm = True):
        self._leftFront = Motor(lfm_num, lfm_pwm_pin, lfm_fake_pwm)
        self._leftBehind = Motor(lbm_num, lbm_pwm_pin, lbm_fake_pwm)
        self._rightFront = Motor(rfm_num, rfm_pwm_pin, rfm_fake_pwm)
        self._rightBehind = Motor(rbm_num, rbm_pwm_pin, rbm_fake_pwm)
        GPIO.setup(wheel_pwm_pin, GPIO.OUT)
        self._wheelPWM = GPIO.PWM(wheel_pwm_pin, 1000)

    def goForward(self):
        self._leftFront.stop()
        self._rightFront.stop()
        self._leftBehind.stop()
        self._rightBehind.stop()
        self._leftFront.run(Motor.FORWARD)
        self._rightFront.run(Motor.FORWARD)
        self._rightBehind.run(Motor.FORWARD)
        self._leftBehind.run(Motor.FORWARD)

    def goBackward(self):
        self._leftFront.stop()
        self._rightFront.stop()
        self._leftBehind.stop()
        self._rightBehind.stop()
        self._leftFront.run(Motor.BACKWARD)
        self._rightFront.run(Motor.BACKWARD)
        self._rightBehind.run(Motor.BACKWARD)
        self._leftBehind.run(Motor.BACKWARD)

    def stop(self):
        self._leftFront.stop()
        self._rightFront.stop()
        self._leftBehind.stop()
        self._rightBehind.stop()
        self._wheelPWM.stop()

    def gear(self, speed):
        pass

    def wheelControl(self, angle):
        self._wheelPWM.start(angle)

def main():
    toycar = ToyCar(1, WHEEL_PWM, 2, PWM_2)
    # raceCar = RaceCar(WHEEL_PWM, 1, FAKE_PWM_1, 2, FAKE_PWM_2, 3, FAKE_PWM_3, 4, FAKE_PWM_4)

    server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    server_socket.bind(("", BT_SERVER_PORT))
    server_socket.listen(1) # Server listens to accept 1 connection at a time

    print "Waiting for a connection..."
    client_socket, address = server_socket.accept()
    print "Welcome ", address
    try:
        while True:
            data = client_socket.recv(128)
            #print "Received command: %s" % cmd
            if data.startwith('s:'):
                cmd = data[2:]
                if cmd == 'f':
                    print "Yes, my lord. Go! Go! Go!"
                    toycar.goForward()
                elif cmd == 'b':
                    print "Yes, my lord. Go backward"
                    toycar.goBackward()
                elif cmd == 'l':
                    print "Turning left"
                    toycar.turnLeft()
                elif cmd == 'r':
                    print "Turning right"
                    toycar.turnRight()
                elif cmd == 's':
                    print "Stop now!"
                    toycar.stop()
                elif cmd == 'a':
                    print "Speed Up"
                    toycar.gear(5)
                    print "Current speed is ",toycar.currentSpeed()
                elif cmd == 'd':
                    print "Slow down..."
                    toycar.gear(-5)
                    print "Current speed is ",toycar.currentSpeed()
                elif cmd == 'q':
                    print "Quit"
                    toycar.stop()
                    break
            elif data.startwith('i:'):
                try:
                    cmd = int(data[2:])
                except:
                    print "invalid command"
                toycar.changeSpeed(cmd)
    except KeyboardInterrupt:
        toycar.stop()
        GPIO.cleanup()
        client_socket.close()
        server_socket.close()
        print("Exiting...")
    
    GPIO.cleanup()
    client_socket.close()
    server_socket.close()

if __name__ == "__main__":
    main()