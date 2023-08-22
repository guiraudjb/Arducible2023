# TechVidvan Human pose estimator
# import necessary packages

import cv2
import mediapipe as mp
zoneinterdite=False
# initialize Pose estimator
mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

pose = mp_pose.Pose(
    min_detection_confidence=0.6,
    min_tracking_confidence=0.5)

# create capture object
#cap = cv2.VideoCapture('./echvid/01.webm')
cap=cv2.VideoCapture(0)
cap.set(3, 320)
cap.set(4, 240)
out = cv2.VideoWriter('outpy.avi',cv2.VideoWriter_fourcc('M','J','P','G'), 10, (320,240))

font = cv2.FONT_HERSHEY_SIMPLEX

while cap.isOpened():
    # read frame from capture object
    _, frame = cap.read()

    try:
        # convert the frame to RGB format
        RGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # process the RGB frame to get the result
        results = pose.process(RGB)
        
        if results.pose_landmarks: # test des resultats
            zoneinterdite=False
            for i in range(1, 32):
                #print(i) # prints: 1, 2, 3, 4
                posY=results.pose_landmarks.landmark[i].y*240
                print(posY)
                if posY > 161:
                    zoneinterdite=True


        #print(results.pose_landmarks)
        # draw detected skeleton on the frame
        if zoneinterdite==True:
            cv2.rectangle(frame,(0,160),(319,239),(255,0,0),-1)#draw restricted area

        mp_drawing.draw_landmarks(
            frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

        # show the final output
        cv2.rectangle(frame,(0,160),(319,239),(0,255,0),1)#draw restricted area
        cv2.putText(frame,'Restricted Area',(10,190), font, 1,(255,255,255),2,cv2.LINE_AA)
        cv2.imshow('Output', frame)
        out.write(frame)

    except:
        break
    if cv2.waitKey(1) == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
