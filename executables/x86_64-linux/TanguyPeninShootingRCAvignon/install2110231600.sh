#!/bin/bash
cat <<- EOF
+----------------------------------------------------------------------+
| Note : Script d'installation des dépendance nécessaires au fonction- |
| -nement des fonctionnalités de vision assistée par ordinateur pour le|
| JEU #Arducible pétanque game.                                        |
| Pour distribution Debian: installation python3 python3-pip           |
| python3-venv                                                         |
| Création d'un environement virtuel python3 et installation des       |
| dépendence mediapipe opencv-python numpy pynput                      |
| création des scripts auxilières                                      |
+----------------------------------------------------------------------+
Auteur : Jean-Baptiste GUIRAUD
EOF


sudo apt-get update
sudo apt-get install python3 python3-pip python3-venv
python3 -mvenv ./Scripts/pythonvenv
source ./Scripts/pythonvenv/bin/activate
pip3 install opencv-python numpy mediapipe pynput
cat <<- EOF >./Scripts/runcam.sh
#!/bin/bash
pwd
source ./Scripts/pythonvenv/bin/activate
python3 ./Scripts/opencv.py
EOF
chmod 775 ./Scripts/runcam.sh

cat <<- EOF >./Scripts/TestCam.sh
#!/bin/bash
pwd
source ./pythonvenv/bin/activate
python3 ./TestOpencv.py
read a
EOF
chmod 775 ./Scripts/TestCam.sh

cat <<- EOF > ./Scripts/opencv.py
import cv2
import mediapipe as mp
from pynput.keyboard import Key, Controller
font = cv2.FONT_HERSHEY_PLAIN
keyboard = Controller()
zoneinterdite=False
PourcentageLargeurCamera=40
# initialize Pose estimator
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

pose = mp_pose.Pose(
    min_detection_confidence=0.8,
    min_tracking_confidence=0.9)
#Largeur=320
#Hauteur=240
cap = cv2.VideoCapture(0)
#cap.set(3, Largeur)
#cap.set(4, Hauteur)
Largeur  = round(cap.get(3))  # float `width`
Hauteur = round(cap.get(4))  # float `height`
LimiteRestrictedArea=round(Hauteur*0.67)
LargeurChampCamera = round((PourcentageLargeurCamera * Largeur)/100)
LimiteGaucheCamera = round((Largeur-LargeurChampCamera)/2)
LimiteDroiteCamera = round(Largeur-(Largeur-LargeurChampCamera)/2)
ret, image = cap.read()
while True :
    photo = cap.read()[1]           # Storing the frame in a variable photo
    photo = cv2.flip(photo,1)       # Fliping the photo for mirror view
    frame = photo[0:Hauteur,LimiteGaucheCamera:LimiteDroiteCamera]      # Cut part of the photo
    RGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    # process the RGB frame to get the result
    results = pose.process(RGB)
    if results.pose_landmarks: # test des resultats
        for i in range(1, 32):
            posY=results.pose_landmarks.landmark[i].y*Hauteur
            if posY < LimiteRestrictedArea:
                zoneinterdite=False
            else:
                zoneinterdite=True
    else:
        zoneinterdite=True
    
    if zoneinterdite==True:
    #    cv2.rectangle(frame,(0,LimiteRestrictedArea),(LargeurChampCamera-1,Hauteur-1),(255,0,0),-1)#draw restricted area
        keyboard.press('y')
    else:
        keyboard.release('y')
    
    #mp_drawing.draw_landmarks(
    #        frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
    # show the final output
    #cv2.rectangle(frame,(0,LimiteRestrictedArea),(LargeurChampCamera-1,Hauteur-1),(0,255,0),1)
    #cv2.putText(frame,'Restricted Area',(0,LimiteRestrictedArea), font, 1,(0,0,255),1,cv2.LINE_AA)
    #cv2.imshow("frame",frame)
    if cv2.waitKey(50) == 13 :
        break
cv2.destroyAllWindows()

EOF


cat <<- EOF > ./Scripts/TestOpencv.py
import cv2
import mediapipe as mp
from pynput.keyboard import Key, Controller
font = cv2.FONT_HERSHEY_PLAIN
keyboard = Controller()
zoneinterdite=False
PourcentageLargeurCamera=40
# initialize Pose estimator
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

pose = mp_pose.Pose(
    min_detection_confidence=0.8,
    min_tracking_confidence=0.9)
#Largeur=320
#Hauteur=240
cap = cv2.VideoCapture(0)
#cap.set(3, Largeur)
#cap.set(4, Hauteur)
Largeur  = round(cap.get(3))  # float `width`
Hauteur = round(cap.get(4))  # float `height`
LimiteRestrictedArea=round(Hauteur*0.67)
LargeurChampCamera = round((PourcentageLargeurCamera * Largeur)/100)
LimiteGaucheCamera = round((Largeur-LargeurChampCamera)/2)
LimiteDroiteCamera = round(Largeur-(Largeur-LargeurChampCamera)/2)
ret, image = cap.read()
while True :
    photo = cap.read()[1]           # Storing the frame in a variable photo
    photo = cv2.flip(photo,1)       # Fliping the photo for mirror view
    frame = photo[0:Hauteur,LimiteGaucheCamera:LimiteDroiteCamera]      # Cut part of the photo
    RGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    # process the RGB frame to get the result
    results = pose.process(RGB)
    if results.pose_landmarks: # test des resultats
        for i in range(1, 32):
            posY=results.pose_landmarks.landmark[i].y*Hauteur
            if posY < LimiteRestrictedArea:
                zoneinterdite=False
            else:
                zoneinterdite=True
    else:
        zoneinterdite=True
    
    if zoneinterdite==True:
        cv2.rectangle(frame,(0,LimiteRestrictedArea),(LargeurChampCamera-1,Hauteur-1),(255,0,0),-1)#draw restricted area
        keyboard.press('y')
    else:
        keyboard.release('y')
    
    mp_drawing.draw_landmarks(
            frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
    # show the final output
    cv2.rectangle(frame,(0,LimiteRestrictedArea),(LargeurChampCamera-1,Hauteur-1),(0,255,0),1)
    cv2.putText(frame,'Restricted Area',(0,LimiteRestrictedArea), font, 1,(0,0,255),1,cv2.LINE_AA)
    cv2.imshow("frame",frame)
    if cv2.waitKey(50) == 13 :
        break
cv2.destroyAllWindows()

EOF
