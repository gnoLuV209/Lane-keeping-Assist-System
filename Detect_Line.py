import cv2
import numpy as np
import threading
from time import sleep
import serial
#_________________________________________________________________________________________________________________#
def Xy_Ly_Anh_Mau(image):
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    blur=cv2.GaussianBlur(gray,(5,5),0)
    canny=cv2.Canny(blur,50,150, apertureSize= 3)
    return canny
#################################################################################################################
def Phuong_Trinh_Duong_Thang(image, lines):
    left_fit  = []
    right_fit = []
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 =line[0]
            parameters = np.polyfit((x1, x2), (y1, y2), 1) # Nội suy ra hằng số a và b trong y = ax + b
            slope = parameters[0]
            intercept = parameters[1]
            if slope < 0:
                left_fit.append((slope,intercept))
            elif slope > 0 :
                right_fit.append((slope, intercept))
        left_fit_average = np.average(left_fit, axis=0)
        left_line = Tao_Toa_Do(image, left_fit_average)
        right_fit_average = np.average(right_fit, axis=0)
        right_line = Tao_Toa_Do(image, right_fit_average)
        return np.array([left_line,right_line])
#################################################################################################################
def Tao_Toa_Do(image, line_parameters):
    try:
        slope, intercept = line_parameters
        y1 = int(image.shape[0])
        y2 = int(y1*3/5)
        x1 = int((y1-intercept)/slope)
        x2 = int((y2-intercept)/slope)
    except:
        y1 = 0
        y2 =0
        x1 =0
        x2 =0
    return np.array([x1,y1,x2,y2])
#################################################################################################################
def Hien_Thi_Doan_Thang(image,average_lines):
    line_image = np.zeros_like(image)
    if average_lines is not None:
        for x1, y1, x2, y2 in average_lines:
            try:
                cv2.line(line_image, (x1,y1), (x2,y2), (0,255,0), 5)
            except:
                cv2.putText(line_image,"Detect Unstable", (400,360), cv2.FONT_HERSHEY_COMPLEX, 2, (0,0,255), 3)
    return line_image
#################################################################################################################
def Giao_Diem(average_lines,fixed_line):
    if average_lines is not None:
        left_line_control =  average_lines[0]
        right_line_control = average_lines[1]
        x1_l, y1_l, x2_l, y2_l = left_line_control
        x1_r, y1_r, x2_r, y2_r = right_line_control
        Hoanh_Do_Giao_Diem_Trai = int(((fixed_line-y1_l)/(y1_l-y2_l))*(x1_l-x2_l)+x1_l)
        Hoanh_Do_Giao_Diem_Phai = int(((fixed_line-y1_r)/(y1_r-y2_r))*(x1_r-x2_r)+x1_r)
        Trung_Diem = int((Hoanh_Do_Giao_Diem_Trai + Hoanh_Do_Giao_Diem_Phai)/2)
        return np.array([Trung_Diem, Hoanh_Do_Giao_Diem_Trai, Hoanh_Do_Giao_Diem_Phai])
#################################################################################################################
def Gioi_Han_Khu_Vuc(canny_image, Hoanh_Do_Giao_Diem_Trai, Hoanh_Do_Giao_Diem_Phai ):
    height = canny_image.shape[0]
    if Hoanh_Do_Giao_Diem_Trai == 0:
        triangle = np.array([ 
        [(100,height) , (1250,height) , (400,500)]
        ])
    elif Hoanh_Do_Giao_Diem_Phai == 0:
        triangle = np.array([ 
        [(50,height) , (1250,height) , (900,500)]
        ])
    else:
        triangle = np.array([ 
        [(100,height) , (1200,height) , (600,500)]
        ])
    return triangle
#################################################################################################################
def Tao_Mat_Na(canny_image,left_point,right_point):
    triangle = Gioi_Han_Khu_Vuc(canny_image,left_point,right_point)
    mask = np.zeros_like(canny_image)
    cv2.fillPoly(mask, triangle, 255)
    masked_image = cv2.bitwise_and(canny_image, mask)
    return masked_image
#################################################################################################################
def Dieu_Khien(mid_point, left_point, right_point):
    if 400 < mid_point < 750 and left_point>300 :
        print('Di Thang')
        #arduino.write(b'90')
        sleep(0.1)
    elif 200< left_point <300:
        print('Re Phai',right_point)
        #arduino.write(b'45')
        sleep(0.1)
    else:
        print('Re Trai')
        #arduino.write(b'135')
        sleep(0.1)
#_________________________________________________________________________________________________________________#
#arduino = serial.Serial(port='COM5', baudrate=115200, timeout=0.1)
cap = cv2.VideoCapture("RoadCar.mp4")
while True :
    _, frame = cap.read()
    edges_image = Xy_Ly_Anh_Mau(frame)
    try:
        cropped_image = Tao_Mat_Na(edges_image,left_point,right_point)
    except:
        cropped_image = Tao_Mat_Na(edges_image,0,0)
    lines = cv2.HoughLinesP(cropped_image, 6, np.pi/180, 100, np.array([]), 10, 200)
    average_lines = Phuong_Trinh_Duong_Thang(frame,lines)
    line_image = Hien_Thi_Doan_Thang(frame, average_lines)
    combo_image = cv2.addWeighted(frame,0.8,line_image,1,1)
    try:
        mid_point   = Giao_Diem(average_lines,600)[0]
        left_point  = Giao_Diem(average_lines,600)[1]
        right_point = Giao_Diem(average_lines,600)[2]
        print(left_point, mid_point, right_point)
    except:
        mid_point   = 0
        left_point  = 0
        right_point = 0
        print(left_point, mid_point, right_point)
    thread = threading.Thread(target = Dieu_Khien,args=(mid_point,left_point,right_point,))
    thread.start()
    try:
        cv2.line(combo_image, (left_point,650), (left_point,600), (0,128,255), 3)
        cv2.line(combo_image, (right_point,650), (right_point,600), (102,0,102), 3)
        cv2.line(combo_image, (mid_point,650), (mid_point,600), (0,0,255), 3)
    except:
        cv2.line(frame, (0,0), (0,0), (0,0,0), 5)
    cv2.imshow("Crop Image",cropped_image)
    cv2.imshow("Frame",combo_image )
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#arduino.close()
cap.release()
cv2.destroyAllWindows()