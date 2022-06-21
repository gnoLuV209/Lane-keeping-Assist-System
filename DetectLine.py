import cv2
import numpy as np

import matplotlib.pyplot as plt
'''
import time




'''
#import serial
#arduino = serial.Serial(port='COM6', baudrate=115200, timeout=0.1)
    
#Tim 3 diem bang anh nhi phan
def region_of_interest(image):
 height=image.shape[0]
 #print(height)
 '''
 polygons=np.array([
     [(0,height),(640,height),(300,-200000)]
                  ])
                  '''
 polygons=np.array([
     [(0,height),(800,height),(350,-200000)]
                  ])
 mask=np.zeros_like(image)
 cv2.fillPoly(mask,polygons,255)
 masked_image=cv2.bitwise_and(image,mask)
 return masked_image

#Hien hinh anh
def display_lines(image,lines):
    line_image = np.zeros_like(image)
    if lines is not None:
        for line in lines:
            x1,y1,x2,y2 = line.reshape(4)
            #print(x1+x2,'x1')
            #print(line)
            cv2.line(line_image,(x1,y1),(x2,y2),(255,0,0),10)
    return line_image

cam = cv2.VideoCapture("vdlane.mp4")
window_name = 'Dectect Line'     
while(True):
    Hinh1 = cam.read()[1]
    #Hinh1 = cam.read()
    gray  = cv2.cvtColor(Hinh1, cv2.COLOR_BGR2GRAY)
    blur  = cv2.GaussianBlur(gray,(5,5),0)
    edges = cv2.Canny(blur, 50, 150, apertureSize=3)
    cropped_image = region_of_interest(edges)
    lines = cv2.HoughLinesP(cropped_image,2,np.pi/180,100,np.array([]), minLineLength=70,maxLineGap=70)
    line_control = cv2.HoughLines(edges, rho=1, theta=np.pi/180, threshold=100)
    line_image = display_lines(Hinh1,lines)
    # Chong 2 hinh len nhau
    combo_image= cv2.addWeighted(Hinh1,0.8,line_image,1,1)
    if line_control is not None:
     for line_c in line_control:
        rho,theta = line_c[0]
        #print(theta,'theta')
        #print(line_c[0])
        a = np.cos(theta) 
        b = np.sin(theta) 
        x0 = a*rho 
        y0 = b*rho 
        x1 = int(x0 + 1000*(-b))
        #print(x1,'x1')
        y1 = int(y0 + 1000*(a))
        #print(y1,'y1')
        x2 = int(x0 - 1000*(-b))
        #print(x2,'x2')
        y2 = int(y0 - 1000*(a))
        #print(y2,'y2')
        #cv2.line(Hinh1,(x1,y1),(x2,y2),(255,255,125),2)
        #cv2.line(Hinh1,(0,400),(640,400),(0,255,0),2)
        DieuKhien  = ((300-y1)/(y1-y2))*(x1-x2)+x1
        DieuKhien1 = ((200-y1)/(y1-y2))*(x1-x2)+x1
        DieuKhien2 = ((100-y1)/(y1-y2))*(x1-x2)+x1
        #print(DieuKhien,'DieuKhien')
        #175<DieuKhien<195 or 300<DieuKhien<335
        #(0.3>theta>=0 or 1.5< theta < 2.25) and (175<DieuKhien<195 or 300<DieuKhien<335) 
        if  (150<DieuKhien<220 or 630<DieuKhien<715) and (200<DieuKhien1<260 or 625<DieuKhien1<705) and (250<DieuKhien2<305 or 620<DieuKhien2<695):
            #arduino.write(b'1')
            print("Di thang")
            #time.sleep(0.1)
        elif 2.1<theta<2.4 :
            #arduino.write(b'2')
            #time.sleep(0.1)
            print("Re Trai")
        elif 0.7<theta<0.87 :
            #arduino.write(b'3')
            print("Re Phai")
            #time.sleep(0.1)
            

    #plt.imshow(edges)
    #plt.show()
    
    cv2.imshow(window_name,combo_image)
    #cv2.imshow(window_name,Hinh1)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#arduino.close()
cam.release()
cv2.destroyAllWindows()
