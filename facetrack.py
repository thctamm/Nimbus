from cv2 import *
import numpy as np
import time

class FaceTrack(object):

    def __init__(self, cascadesRoot, eyeSens = (5,15)):
        self.eyeSens = eyeSens
        self.cascadesRoot = cascadesRoot
        self.face_cascade = CascadeClassifier(self.__getCascade__('haarcascade_frontalface_default.xml'))
        self.eye_cascade = CascadeClassifier(self.__getCascade__('haarcascade_eye.xml'))
        self.eyes_cascade = CascadeClassifier(self.__getCascade__('haarcascade_eyes.xml'))
        self.mouth_cascade = CascadeClassifier(self.__getCascade__('haarcascade_mouth.xml'))
        self.eyes = []
        self.faces = []
        self.img = None
        self.debug = False
        self.cam = VideoCapture(0)
        self.cam.set(3,640)
        self.cam.set(4,640)
        self.averageStraight = (0,0,0,0)

    def setDebug(self, val):
        self.debug = val
        if val:
            namedWindow('debug')
        

    def __getCascade__(self, cascade):
        return '{}/{}'.format(self.cascadesRoot, cascade)
               
    def captureImage(self):
        s, img = self.cam.read()
        if s:
            self.img = img


    def getCam(self):
        return self.cam

    def detectFace(self):
        self.captureImage()
        gray = cvtColor(self.img, COLOR_BGR2GRAY)
        self.faces = self.face_cascade.detectMultiScale(
            self.img,
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30)
        )
        for (x,y,w,h) in self.faces:
            rectangle(self.img,(x,y),(x+w,y+h),(255,0,0),2)
            if self.averageStraight != (0,0,0,0):
                (ex,ey,ew,eh) = self.averageStraight
                rectangle(self.img,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
            roi_gray = gray[y:y+h, x:x+w]
            roi_color = self.img[y:y+h, x:x+w]
            if self.debug:
                imshow('debug',self.img)
                return waitKey(200)
        return 0
                
    def filterMouth(self):
        lowestX = 0
        lowestMouth = None
        for mouth in self.mouths:
            if mouth[0] > lowestX:
                lowestX = mouth[0]
                lowestMouth = mouth
        if lowestMouth == None:
            self.mouths = []
            return
        self.mouths = [lowestMouth]

    def classifyEyes(self):
        if len(self.eyes) < 2:
            self.eyes = []
            return
        for i, sourceEye in enumerate(self.eyes):
            for j in range(i+1, len(self.eyes)):
                targetEye = self.eyes[j]
                if abs(sourceEye[3]-targetEye[3]) <= self.eyeSens[0]:
                    if sourceEye[0] < targetEye[0]:
                        left = sourceEye
                        right = targetEye
                    else:
                        right = sourceEye
                        left = targetEye
                    if right[0] - (left[0]+left[2]) > self.eyeSens[1]:
                        self.eyes = [(left[0],left[1],right[0]-left[0]+right[2],left[3])]
                        return
        self.eyes = []


    def getBaseline(self, numSamples):
        oldDebug = self.debug
        self.setDebug(False)
        raw_input("keep looking straight at the middle of the screen and press enter")
        self.averageStraight = self.averageFeatures(numSamples)
        self.avgMid = self.averageStraight[1]+ 0.5*self.averageStraight[3]
        print("Done!")
        self.setDebug(oldDebug)


    def suggest(self, sens):
        self.detectFace()
        while len(self.faces) != 1:
            self.detectFace()
        mid = self.faces[0][1]+0.5*self.faces[0][3]
        if abs(self.avgMid - mid) >= sens:
            if self.avgMid > mid:
                return 1
            else:
                return -1
        return 0

    def averageFeatures(self, numSamples):
        faceArr = []
        while True:
            self.detectFace()
            faceArr.extend(self.faces)
            if len(faceArr) >= numSamples:
                break
        
        return self.getAverageFeature(faceArr)


    def getAverageFeature(self, featArr):
        x = 0
        y = 0
        w = 0
        h = 0
        for feat in featArr:
            x += feat[0]
            y += feat[1]
            w += feat[2]
            h += feat[3]
        featLen = len(featArr)
        return (x/featLen, y/featLen, w/featLen, h/featLen)


    def printFaces(self):
        print('faces:')
        for face in self.faces:
            print('\t{}'.format(face))
        print('eyes:')
        for eye in self.eyes:
            print('\t{}'.format(eye))

    def printAverages(self):
        print('straight:')
        print('\t{}'.format(self.averageStraight))


