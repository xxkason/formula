import bluetooth
import RPi.GPIO as GPIO
from car import Car_2DC, Car_4WD, Run_Direction, Turn_Direction

BT_SERVER_PORT = 1
WHEEL_PWM = 16 # GPIO23
PWM_2 = 18 # GPIO24
FAKE_PWM_1 = 22 # GPIO25
FAKE_PWM_2 = 32 # GPIO12
FAKE_PWM_3 = 36 # GPIO16
FAKE_PWM_4 = 38 # GPIO20

def main():
    # L293D solution
    #btcar = Car_2DC(1, WHEEL_PWM, 2, PWM_2)
    # L298N solution
    btcar = Car_2DC(32, 18, 37, 16, 12, 11)
    # 4wd car demo
    #btcar = Car_4WD(WHEEL_PWM, 1, FAKE_PWM_1, 2, FAKE_PWM_2, 3, FAKE_PWM_3, 4, FAKE_PWM_4)

    server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    server_socket.bind(("", BT_SERVER_PORT))
    server_socket.listen(1) # Server listens to accept 1 connection at a time

    print "Waiting for a connection..."
    client_socket, address = server_socket.accept()
    print "Welcome ", address
    try:
        while True:
            cmd = client_socket.recv(128)
            print "Received command: %s" % cmd
            if cmd == 'f':
                print "Yes, my lord. Go! Go! Go!"
                btcar.run(Run_Direction.FORWARD)
            elif cmd == 'b':
                print "Yes, my lord. Go backward"
                btcar.run(Run_Direction.BACKWARD)
            elif cmd == 'l':
                print "Turning left"
                btcar.turn(Turn_Direction.LEFT)
            elif cmd == 'r':
                print "Turning right"
                btcar.turn(Turn_Direction.RIGHT)
            elif cmd == 's':
                print "Stop now!"
                btcar.stop()
            elif cmd == 'a':
                print "Speed Up"
                btcar.gear(5)
                print "Current speed is %d" % btcar.currentSpeed()
            elif cmd == 'd':
                print "Slow down..."
                btcar.gear(-5)
                print "Current speed is %d" % btcar.currentSpeed()
            elif cmd == 'q':
                print "Quit"
                btcar.stop()
                break
            else:
                try:
                    btcar.changeSpeed(int(cmd))
                    print "Current speed is %d" % btcar.currentSpeed()
                except:
                    print "%s is an invalid command" % cmd
    except:
        pass
       
    btcar.stop()
    GPIO.cleanup()
    client_socket.close()
    server_socket.close()
    print("Exiting...")

if __name__ == "__main__":
    main()