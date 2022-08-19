// Khai báo thư viện
#include <Servo.h>
/*____________________________________________________________________*/
// Định nghĩa các chân quan trọng
#define tien_motor_1 11               // chân IN - 1 của Module L298
#define lui_motor_1  10              // chân IN - 2 của Module L298
#define tien_motor_2 6              // chân IN - 3 của Module L298
#define lui_motor_2  5             // chân IN - 4 của Module L298
#define servo 3       
/*____________________________________________________________________*/                 
// Khai báo biến
int data_from_arduino;
int control_motor;
Servo myServo;
/*____________________________________________________________________*/
// Thiết lập ban đầu cho các chân Arduino và khởi tạo giao tiếp Serial với máy tính
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  myServo.attach(servo);
  pinMode(tien_motor_1,OUTPUT); 
  pinMode(lui_motor_1,OUTPUT); 
  pinMode(tien_motor_2,OUTPUT); 
  pinMode(lui_motor_2,OUTPUT); 
  digitalWrite(tien_motor_1,LOW);
  digitalWrite(lui_motor_1,LOW);
  digitalWrite(tien_motor_2,LOW);
  digitalWrite(lui_motor_2,LOW);
}
/*____________________________________________________________________*/
void loop() {
  data_from_arduino = Serial.readString().toInt();
  control_motor = steering_angle(data_from_arduino);
  if( control_motor == 90 ){
    dithang();
  }
  else if( control_motor == 45 ){
    disangphai();
  }
  else if( control_motor == 135 ){
    disangtrai();
  } 
}
/*____________________________________________________________________*/
// Điều khiển vô lăng đánh lái tự động mô phỏng bằng motor Servo
int steering_angle(int data_from_arduino){
  int steering_angle;
  if( data_from_arduino==90 ){
    steering_angle = 90;
    myServo.write(steering_angle);
    delay(500);
  }
  else if( data_from_arduino==45 ){
    steering_angle = 45;
    myServo.write(steering_angle);
    delay(500);
  }
  else if( data_from_arduino==135 ){
    steering_angle = 135;
    myServo.write(steering_angle);
    delay(500);
  }
  return steering_angle;
}
/*____________________________________________________________________*/
// Điều khiển trạng thái của xe ( Đi thẳng, Rẽ Phải, Rẽ Trái, Dừng lại )
void dithang(){ 
  analogWrite(tien_motor_1,50);   
  analogWrite(lui_motor_1,0);
  analogWrite(tien_motor_2,50);
  analogWrite(lui_motor_2,0);
  delay(500);      
}
void disangtrai(){
  resetdongco();
  analogWrite(tien_motor_1,50);
  analogWrite(lui_motor_1,0);
  analogWrite(tien_motor_2,0);
  analogWrite(lui_motor_2,0);
  delay(500);  
}
void disangphai(){
  resetdongco();
  analogWrite(tien_motor_1,0);
  analogWrite(lui_motor_1,0);
  analogWrite(tien_motor_2,50);
  analogWrite(lui_motor_2,0);
  delay(500);
}
void resetdongco(){
  analogWrite(tien_motor_1,0);
  analogWrite(lui_motor_1,0);
  analogWrite(tien_motor_2,0);
  analogWrite(lui_motor_2,0);
  delay(500);
}
