import time
from pynput.keyboard import Key, Controller

from flask import *
 
app = Flask(__name__)
keyboard = Controller()
 
def presstwo(x, y):
    keyboard.press(x)
    keyboard.press(y)
    time.sleep(0.25)
    keyboard.release(x)
    keyboard.release(y)
 
 
def press(x):
    keyboard.press(x)
    time.sleep(0.25)
    keyboard.release(x)
 
@app.route("/")
def play():
    # ?posy=0&posx=0
    posy = int(request.args.get('posy'))
    posx = int(request.args.get('posx'))
    posz = int(request.args.get('posz'))
    touch = int(request.args.get('touch'))
    #sw = int(request.args.get('sw'))
    encoder = int(request.args.get('encoder'))
    if posx == -1:
        if posy == -1: presstwo('w', 'a')
        elif posy == 0: press('w')
        elif posy == 1: presstwo('w', 'd')
    elif posx == -0:
        if posy == -1: press('a')
        elif posy == 1: press('d')
    elif posx == 1:
        if posy == -1: presstwo('s', 'a')
        elif posy == 0: press('s')
        elif posy == 1: presstwo('s','d')
    if touch == 1 : press('f')
    if encoder == 1 or encoder == -1 : press(Key.esc)
    return 'Playing key {}'.format(posx)
# start the server at the port 5000
app.run(host = '0.0.0.0')