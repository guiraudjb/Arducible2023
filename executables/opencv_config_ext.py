import cv2
import mediapipe as mp
from pynput.keyboard import Key, Controller
import json
# import sys
font = cv2.FONT_HERSHEY_PLAIN
keyboard = Controller()
zoneinterdite=False
# config.json
try:
  with open("config.json","r") as f:
    data = json.load(f)
    showVideo = data['showVideo']
    saveVideo = data['saveVideo']
    # debug
    # print("showVideo:",data['showVideo'],"saveVideo:",data['saveVideo'])
except IOError:
  showVideo=True
  saveVideo=True  
  print("error config.json")  

# debug
# print("showVideo:",showVideo,"saveVideo:",saveVideo)
# print("debug stop")
# sys.exit 

# config.json - end
showVideo=True
saveVideo=True
# Define the codec and create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', fourcc, 20.0, (320,  240))

PourcentageLargeurCamera=30
PourcentageHauteurCamera=35
# initialize Pose estimator
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

pose = mp_pose.Pose(
    min_detection_confidence=0.8,
    min_tracking_confidence=0.9)
Largeur=320
Hauteur=240
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
cap.set(3, Largeur)
cap.set(4, Hauteur)
#Largeur  = round(cap.get(3))  # float 
#Hauteur = round(cap.get(4))  # float 
LimiteRestrictedArea=round(Hauteur*0.67)
LargeurChampCamera = round((PourcentageLargeurCamera * Largeur)/100)
LimiteGaucheCamera = round((Largeur-LargeurChampCamera)/2)
LimiteDroiteCamera = round(Largeur-(Largeur-LargeurChampCamera)/2)

HauteurChampCamera = round((PourcentageHauteurCamera * Largeur)/100)
LimiteBasseCamera = round((Hauteur-HauteurChampCamera)/2)
LimiteHauteCamera = round(Hauteur-(Hauteur-HauteurChampCamera)/2)

# Define the codec and create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'MJPG')
out = cv2.VideoWriter('output.avi', fourcc, 20.0, (LargeurChampCamera,  HauteurChampCamera))

ret, image = cap.read()
while True :
    photo = cap.read()[1]           # Storing the frame in a variable photo
    photo = cv2.flip(photo,1)       # Fliping the photo for mirror view
    frame = photo[LimiteBasseCamera:LimiteHauteCamera,LimiteGaucheCamera:LimiteDroiteCamera]      # Cut part of the photo
    RGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    # process the RGB frame to get the result
    results = pose.process(RGB)
    if results.pose_landmarks: # test des resultats
        for i in range(1, 32):
            posY=results.pose_landmarks.landmark[i].y*HauteurChampCamera
            #print(posY)
            if posY <HauteurChampCamera :
                zoneinterdite=False
            else:
                zoneinterdite=True
    else:
        zoneinterdite=True
    if zoneinterdite==True:
        keyboard.press('y')
        cv2.rectangle(frame,(0,0),(30,30),(0,0,255),-1)
        cv2.putText(frame,'Go to shooting area!',(0,50), font, 1,(0,0,255),1,cv2.LINE_AA)
    else:
        keyboard.release('y')
        cv2.rectangle(frame,(0,0),(30,30),(0,255,0),-1)
        cv2.line(frame, (0,0), (30,30), (0,255,0), 1)
        cv2.line(frame,(round(results.pose_landmarks.landmark[12].x*LargeurChampCamera),round(results.pose_landmarks.landmark[12].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[11].x*LargeurChampCamera),round(results.pose_landmarks.landmark[11].y*HauteurChampCamera)),(0,255,0),3)

        cv2.line(frame,(round(results.pose_landmarks.landmark[12].x*LargeurChampCamera),round(results.pose_landmarks.landmark[12].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[14].x*LargeurChampCamera),round(results.pose_landmarks.landmark[14].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[14].x*LargeurChampCamera),round(results.pose_landmarks.landmark[14].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[16].x*LargeurChampCamera),round(results.pose_landmarks.landmark[16].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[11].x*LargeurChampCamera),round(results.pose_landmarks.landmark[11].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[13].x*LargeurChampCamera),round(results.pose_landmarks.landmark[13].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[13].x*LargeurChampCamera),round(results.pose_landmarks.landmark[13].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[15].x*LargeurChampCamera),round(results.pose_landmarks.landmark[15].y*HauteurChampCamera)),(0,255,0),3)

        cv2.line(frame,(round(results.pose_landmarks.landmark[12].x*LargeurChampCamera),round(results.pose_landmarks.landmark[12].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[24].x*LargeurChampCamera),round(results.pose_landmarks.landmark[24].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[24].x*LargeurChampCamera),round(results.pose_landmarks.landmark[24].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[26].x*LargeurChampCamera),round(results.pose_landmarks.landmark[26].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[26].x*LargeurChampCamera),round(results.pose_landmarks.landmark[26].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[28].x*LargeurChampCamera),round(results.pose_landmarks.landmark[28].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[28].x*LargeurChampCamera),round(results.pose_landmarks.landmark[28].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[32].x*LargeurChampCamera),round(results.pose_landmarks.landmark[32].y*HauteurChampCamera)),(0,255,0),3)

        cv2.line(frame,(round(results.pose_landmarks.landmark[11].x*LargeurChampCamera),round(results.pose_landmarks.landmark[11].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[23].x*LargeurChampCamera),round(results.pose_landmarks.landmark[23].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[23].x*LargeurChampCamera),round(results.pose_landmarks.landmark[23].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[25].x*LargeurChampCamera),round(results.pose_landmarks.landmark[25].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[25].x*LargeurChampCamera),round(results.pose_landmarks.landmark[25].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[27].x*LargeurChampCamera),round(results.pose_landmarks.landmark[27].y*HauteurChampCamera)),(0,255,0),3)
        cv2.line(frame,(round(results.pose_landmarks.landmark[27].x*LargeurChampCamera),round(results.pose_landmarks.landmark[27].y*HauteurChampCamera)),(round(results.pose_landmarks.landmark[31].x*LargeurChampCamera),round(results.pose_landmarks.landmark[31].y*HauteurChampCamera)),(0,255,0),3)
    if showVideo ==True:
        cv2.imshow("frame",frame)
    if saveVideo==True:
        out.write(frame)
    if cv2.waitKey(50) == 13 :
        break
cv2.destroyAllWindows()

