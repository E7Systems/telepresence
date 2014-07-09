
#include <SoftwareSerial.h>
 
SoftwareSerial BT = SoftwareSerial(3, 2);
 
char vals[4] = {'0', '0', '0', '0'};
int xidx = 1;
int combosTested = 0;
void attemptCombo() {
  if(xidx >= 4)
    xidx = 1;
  vals[xidx-1]++;
  if(vals[xidx-1] > '9') {
   vals[xidx]++;
   vals[xidx-1] = '0'; 
  } 
  if(vals[xidx] >= '1') {
    //vals[xidx] = 0;
    xidx++;
  }
  for (int i = 0; i < 4; i++) {
    Serial.write(vals[i]);
    BT.write(vals[i]);
    delay(10);
  }
  BT.write('\n');
  //delay(1000);
  combosTested++;
  if(combosTested % 5 == 0 && combosTested != 0) {
     analogWrite(13, 255); //too lazy for digitalWrite
     delay(1000); 
     BT.write('\n');
     delay(5000); 
     BT.write('\n');
     delay(5000); 
     BT.write('\n');
     delay(5000); 
     BT.write('\n');
     delay(5000); 
     BT.write('\n');
     delay(5000); 
     BT.write('\n');
     delay(10000); 
     
     analogWrite(13, 0);
  } else {
    delay(100);
  }
  //BT.write('\n');

}
void setup() {
  Serial.begin(9600);
  BT.begin(9600);
 
  delay(5000);
  Serial.println("Softserial/BT test!\n\rPress any key+return to start");
  //altkeystest();
  //nonprinting();
  //iphonekeyboard();
  //rawkeytest();
 
  //Supported in v1.1
  //rawmousetest();
}
 
void loop() {
  attemptCombo();
  
}
