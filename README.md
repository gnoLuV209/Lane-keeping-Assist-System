# Lane-keeping-Assist-System
Design and Simulate The Lane Keeping Assist System on Automobile
This is a personal project. Now i'll guide you how to create the model and run the code

STEP 1 : You need to prepare some electronic components
+ One Microcontroller Arduino Uno for receiving data from Small single-board computers Raspberry Pi or your personal computer and controlling L298 DC Motor Driver
+ One L298 DC Motor Driver for control 4 motors at 2 wheels 
+ One 4 Wheel Drive Chassis Kit with Speed Encoder for the move
+ One Small single-board computers Raspberry Pi with Module Camera Mini Raspberry "or" your personal computer with computer's webcam for road marking recognition
+ One Motor Servo for steering wheel simulation

STEP 2 : Download some sofware for this project
+ Download the Arduino IDE sofware for uploading source code to the microcontroller
+ Download the Python IDE sofware for image processing by computer vision
+ Open Command Prompt to install some package for python: type command: py -m pip install pyserial, type command: py -m pip install Pillow, type command: py -m pip install opencv-python, type command: py -m pip install pyserial

STEP 3 : Design model with all components prepared at STEP 1, you can see file "Circuit Diagram with L298.png" and "DesignModel.png" for designing models

STEP 4 : Run the source code and watch the result  
+ Open Arduino IDE software and copy code in file "Control_steering wheel_and_motorDC.ino" to your Arduino IDE software
+ Connect to your computer and select board, here you'll chose Arduino Uno and click "upload" for loading into the microcontroller
+ Download video "RoadCar.mp4" and put in the same folder as the file "Detect_Line.py"
+ Open file "Detect_Line.py" run and see the result

