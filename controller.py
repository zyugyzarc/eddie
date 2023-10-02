import RPi.GPIO as GPIO
import numpy as np
GPIO.setmode(GPIO.BOARD)

class Motor:
    def __init__(self, L, R):

        GPIO.setup((L, R), GPIO.OUT)

        self.L = GPIO.PWM(L, 256)
        self.R = GPIO.PWM(R, 256)

        self.L.start(0)
        self.L.start(1)

        self.value = 0

    @property
    def value(self):
        return self.rot

    @value.setter
    def value(self, val):
        self.rot = val

        if val >= 0:
            self.L.ChangeDutyCycle(val)
            self.R.ChangeDutyCycle(0)
        else:
            self.L.ChangeDutyCycle(0)
            self.R.ChangeDutyCycle(-val)

class Driver:
    def __init__(self, LL, LR, RL, RR):
        self.L = Motor(LL, LR)
        self.R = Motor(RL, RR)

    def drive(self, throttle, steer):
        steer *= -1 if throttle < 0 else 1
        self.L.value = throttle + steer
        self.R.value = throttle - steer

"""
class Agent:
    def __init__(self, driver):
        self.driver = driver
        self.position = np.array((0, 0))
"""

# temporary
# Control eddie through http server

driver = Driver(-1, -1, -1, -1)

from HTTPWebSocketsHandler import HTTPWebSocketsHandler
from http.server import HTTPServer
import json

class EddieServer(HTTPWebSocketsHandler):

    def do_get(self):
        with open('client.html', 'rb') as f:
            self.send_response(200)
            self.end_headers()
            self.wfile.write(f.read())

    def on_ws_message(self, message):
        driver.drive(*json.loads(message))

HTTPServer(('0.0.0.0', 8888), EddieServer).serve_forever()
