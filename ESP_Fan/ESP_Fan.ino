void setup() {
  Serial.begin(115200);

  pinMode(15, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

 /*
  analogWrite(15, 50); 
  delay(2000);
  Serial.println("50");
   
  analogWrite(15, 100);
  delay(2000);
  Serial.println("100");

  analogWrite(15, 150);
  delay(2000);
  Serial.println("150");
  */

  for(int i=50; i <255; i=i+50){

   Serial.println(i);
   analogWrite(15, i);
   delay(1500);
    
  }

  
 
}
