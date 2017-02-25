#!/usr/bin/python

from facetrack import *

cascadesRoot = '/usr/local/share/OpenCV/haarcascades'
baselineSamples = 20
sensitivity = 20
token = 'b9f3a9b2b8773b01a69468bd151c98d601bc0c1f'
devId = '20003d001047353138383138'

def contDisp(ft):
    cam = ft.getCam()
    while(True):
        ft.detectFace()
        if ft.detectFace() & 0xFF == ord('q'):
            break
    cam.release()
    destroyAllWindows()

def sendSuggestion(suggestion):
    url = 'https://api.particle.io/v1/devices/{}/suggest'.format(devId)
    payload = {
        'access_token': token,
        'arg': suggestion
    }
    r = requests.post(url, json=payload)

def main():
    ft = FaceTrack(cascadesRoot)
    ft.getBaseline(baselineSamples)
    while True:
        suggestion = ft.suggest(sensitivity)
        sendSuggestion(suggestion)
    
if __name__ == "__main__":
        main()
