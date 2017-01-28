import numpy as np
import cv2
from matplotlib import pyplot as plt

face_cascade = cv2.CascadeClassifier(r'D:\OpenCV\opencv\sources\data\haarcascades\haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(r'D:\OpenCV\opencv\sources\data\haarcascades\haarcascade_eye.xml')

cap=cv2.VideoCapture(0)

while (1):
	ret, img=cap.read()
	
	gray=cv2. cvtColor(img, cv2.COLOR_BGR2GRAY)
	
	faces=face_cascade.detectMultiScale(gray,1.3,5)
	for(x,y,w,h) in faces:
		img = cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
		roi_gray= gray[y:y+h, x:x+w]
		roi_color = img[y:y+h, x:x+w]
		featrue=img[y:y+h, x:x+w]
		#eyes = eye_cascade.detectMultiScale(roi_gray)
		
		#for (ex,ey,ew,eh) in eyes:
		#	cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
	cv2.imshow('img',img)
	save=cv2.waitKey(30)
	
	k=cv2.waitKey(30)  & 0xff
	if k==27:
		break
	elif (k==ord('s')):
		#str=featrue+str(count)
		cv2.imwrite('feature.png',featrue)
		
cap.release()
cv2.destroyAllWindows()	
	