
#include <SoftwareSerial.h>
 
SoftwareSerial BT = SoftwareSerial(3, 2);
char* buffer = new char[50];
void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("Please enter a bluetooth command: ");
  BT.begin(9600);
  
}
 
void loop() {
  if(Serial.available() > 0) {
    Serial.readBytes(buffer, Serial.available());
    for(int i = 0; i < sizeof(buffer); i++) {
      BT.write(buffer[i]); 
    }
    buffer = new char[50];
  }
}
