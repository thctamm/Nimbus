#!/usr/bin/python

from facetrack import *
import sys
import requests
from time import sleep

cascadesRoot = '/usr/local/share/OpenCV/haarcascades'
baselineSamples = 10
sensitivity = 15
token = 'b9f3a9b2b8773b01a69468bd151c98d601bc0c1f'
devId = '20003d001047353138383138'

def contDisp(ft):
    cam = ft.getCam()
    ft.getBaseline(40)
    while(True):
        ft.printFaces()
        if ft.detectFace() & 0xFF == ord('q'):
            break
    cam.release()
    destroyAllWindows()

def sendSuggestion(suggestion):
    url = 'https://api.particle.io/v1/devices/20003d001047353138383138/suggest?access_token=b9f3a9b2b8773b01a69468bd151c98d601bc0c1f'
    payload = {
        'arg': suggestion
    }
    r = requests.post(url, data=payload)
    print(r.json())

def main():
    if len(sys.argv) == 2:
        while True:
            direction = raw_input('enter u for up, d for down, n for nothing: ')
            if direction == 'u':
                sendSuggestion(1)
            elif direction == 'd':
                sendSuggestion(-1)
            elif direction == 'n':
                sendSuggestion(0)
            else:
                print('Try again')
    elif len(sys.argv) > 2:
        ft = FaceTrack(cascadesRoot)
        ft.setDebug(True)
        contDisp(ft)
    else:
        ft = FaceTrack(cascadesRoot)
        #ft.setDebug(True)
        ft.getBaseline(baselineSamples)
        while True:
            suggestion = ft.suggest(sensitivity)
            ft.printFaces()
            sendSuggestion(suggestion)
            
    
if __name__ == "__main__":
        main()
