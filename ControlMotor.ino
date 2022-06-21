int tien1=10;            // chân IN - 1 của Module L298.
int tien2=11;           // chân IN - 3 của Module L298.
int lui1=6;            // chân IN - 4 của Module L298.
int lui2=9;           // chân IN - 2 của Module L298.
int x;          
void dithang();
void disangtrai();
void disangphai();
void setup()
 {
pinMode(tien1,OUTPUT); 
pinMode(tien2,OUTPUT); 
pinMode(lui1,OUTPUT); 
pinMode(lui2,OUTPUT); 
digitalWrite(tien1,LOW);
digitalWrite(tien2,LOW);
digitalWrite(lui1,LOW);
digitalWrite(lui1,LOW);
Serial.begin(115200);  
Serial.setTimeout(1); 
}

  

void loop()
 {
x = Serial.readString().toInt();  // python đưa về dạng chuỗi sau đó chuyển chuỗi đó thành số nguyên
 if (x==1){
    dithang();
   }
 if (x==2) {
    disangtrai();
   }
 if (x==3){
    disangphai();
    }
  }
void dithang()
{ 
        analogWrite(tien1,50);   
        analogWrite(lui1,0);
        analogWrite(tien2,50);
        analogWrite(lui2,0);
          
}

void disangtrai()
{
        resetdongco();
        analogWrite(tien1,50);
        analogWrite(lui1,0);
        analogWrite(tien2,0);
        analogWrite(lui2,0);
        delay(910);
    
}
void disangphai()
{
        resetdongco();
        analogWrite(tien1,0);
        analogWrite(lui1,0);
        analogWrite(tien2,35);
        analogWrite(lui2,0);
        delay(910);
  
}


void resetdongco()
{

        analogWrite(tien1,0);
        analogWrite(lui1,0);
        analogWrite(tien2,0);
        analogWrite(lui2,0);
        delay(2000);
}