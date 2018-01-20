import RPi.GPIO as GPIO
import time

sensor_input = 36
led = 40
ledstate = False
#set numbering mode for the program
# GPIO.setmode(GPIO.BOARD)

# GPIO.setup(sensor_input, GPIO.IN,initial=0)
# GPIO.setup(led, GPIO.OUT,initial=0)

#this method will be invoked when the event occurs
def switchledstate(channel):
    global ledstate
    global led
    ledstate = not(ledstate)
    GPIO.output(led,ledstate)
 
GPIO.setmode(GPIO.BOARD)
GPIO.setup(sensor_input ,GPIO.IN, GPIO.PUD_UP)
GPIO.setup(led, GPIO.OUT, initial=ledstate)
# adding event detect to the switch pin
GPIO.add_event_detect(sensor_input, GPIO.BOTH, switchledstate, 600)
try:
    while(True):
        #to avoid 100% CPU usage
        time.sleep(1)
except KeyboardInterrupt:
    #cleanup GPIO settings before exiting
    GPIO.cleanup()