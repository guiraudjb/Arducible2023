# TechVidvan Human pose estimator
# import necessary packages

import cv2
cap=cv2.VideoCapture(0)
cap.set(3, 320)
cap.set(4, 240)
out = cv2.VideoWriter('test.avi',cv2.VideoWriter_fourcc('M','J','P','G'), 10, (320,240))

while cap.isOpened():
    # read frame from capture object
    _, frame = cap.read()
    cv2.imshow('Output', frame)
    out.write(frame)

