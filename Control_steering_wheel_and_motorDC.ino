// Khai báo thư viện
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
/*____________________________________________________________________*/
// Định nghĩa các chân quan trọng
#define tien_motor_1 11               // chân IN - 1 của Module L298
#define lui_motor_1  10              // chân IN - 2 của Module L298
#define tien_motor_2 6              // chân IN - 3 của Module L298
#define lui_motor_2  5             // chân IN - 4 của Module L298
#define enA  7
#define enB  2
#define servo 3 
#define congtac 13      
/*____________________________________________________________________*/                 
// Khai báo biến
uint8_t data_from_arduino;
uint8_t control_motor;
uint8_t driveModeSwitchtruoc,driveModeSwitch;
uint8_t trangThaiNutNhan;
uint8_t VTA,ESP;
uint16_t kqADC0,kqADC1;
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
/*____________________________________________________________________*/
// Thiết lập ban đầu cho các chân Arduino và khởi tạo giao tiếp Serial với máy tính
void setup() {
  Serial.begin(57600);
  Serial.setTimeout(1);
  myServo.attach(servo);
  lcd.begin();
  lcd.backlight();
  pinMode(congtac,INPUT_PULLUP);
  pinMode(tien_motor_1,OUTPUT); 
  pinMode(lui_motor_1,OUTPUT); 
  pinMode(tien_motor_2,OUTPUT); 
  pinMode(lui_motor_2,OUTPUT);
  pinMode(enA,OUTPUT); 
  pinMode(enB,OUTPUT);
  digitalWrite(tien_motor_1,LOW);
  digitalWrite(lui_motor_1,LOW);
  digitalWrite(tien_motor_2,LOW);
  digitalWrite(lui_motor_2,LOW);
}
/*____________________________________________________________________*/
void loop() {
  driveModeSwitchtruoc = driveModeSwitch; driveModeSwitch = digitalRead(13);
  if(driveModeSwitchtruoc==1 && driveModeSwitch==0){
    trangThaiNutNhan = 1-trangThaiNutNhan;
  }
  if(trangThaiNutNhan == 0){
    lcd.setCursor(0,0);
    lcd.print("Self-Driving:OFF");
    kqADC0 = analogRead(A0);
    VTA = (uint32_t)kqADC0*100/1023;
    dithang(kqADC0/4);
    kqADC1 = analogRead(A1);
    ESP = map(kqADC1,0,1023,0,180);
    myServo.write(ESP);
    lcd.setCursor(0,1);
    lcd.print("VTA:");lcd.print(VTA);lcd.print("%");
    lcd.print(" ESP:");hienThi1Byte(ESP);
  }
  if(trangThaiNutNhan == 1){
    lcd.setCursor(0,0);
    lcd.print("Self-Driving:ON ");
    data_from_arduino = Serial.readString().toInt();
    control_motor = steering_angle(data_from_arduino);
    if( control_motor == 90 ){
      dithang(255);
      lcd.setCursor(0,1);
      lcd.print("Go straight     "); 
    }
    else if( control_motor == 45 ){
      disangphai(150);
      lcd.setCursor(0,1);
      lcd.print("Turn right      ");
    }
    else if( control_motor == 135 ){
      disangtrai(150);
      lcd.setCursor(0,1);
      lcd.print("Turn left       ");
    }  
  }
  delay(10);
}
/*____________________________________________________________________*/
// Điều khiển vô lăng đánh lái tự động mô phỏng bằng motor Servo
uint8_t steering_angle(uint8_t data_from_arduino){
  uint8_t steering_angle;
  if( data_from_arduino == 90 ){
    steering_angle = 90;
    myServo.write(steering_angle);
  }
  else if( data_from_arduino == 45 ){
    steering_angle = 45;
    myServo.write(steering_angle);
  }
  else if( data_from_arduino == 135 ){
    steering_angle = 135;
    myServo.write(steering_angle);
  }
  return steering_angle;
}
/*____________________________________________________________________*/
// Điều khiển trạng thái của xe ( Đi thẳng, Rẽ Phải, Rẽ Trái, Dừng lại )
void dithang(uint8_t VTA){ 
  analogWrite(enA,VTA);
  analogWrite(enB,VTA);
  digitalWrite(tien_motor_1,HIGH);   
  digitalWrite(lui_motor_1,LOW);
  digitalWrite(tien_motor_2,LOW);
  digitalWrite(lui_motor_2,HIGH);      
}
void disangphai(uint8_t VTA){
  resetdongco();
  analogWrite(enA,VTA);
  digitalWrite(tien_motor_1,HIGH);
  digitalWrite(lui_motor_1,LOW);
  digitalWrite(tien_motor_2,LOW);
  digitalWrite(lui_motor_2,LOW);  
}
void disangtrai(uint8_t VTA){
  resetdongco();
  analogWrite(enB,VTA);
  digitalWrite(tien_motor_1,LOW);
  digitalWrite(lui_motor_1,LOW);
  digitalWrite(tien_motor_2,LOW);
  digitalWrite(lui_motor_2,HIGH);
}
void resetdongco(){
  digitalWrite(tien_motor_1,0);
  digitalWrite(lui_motor_1,0);
  digitalWrite(tien_motor_2,0);
  digitalWrite(lui_motor_2,0);
}
void hienThi1Byte(uint8_t dem){
  if(dem<10){lcd.print(dem);lcd.print("*  ");}
  else if(10<dem<100){lcd.print(dem);lcd.print("* ");}
  else {lcd.print(dem);lcd.print("*");}
}
