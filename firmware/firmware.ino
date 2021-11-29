#include <VarSpeedServo.h>

VarSpeedServo j1;
VarSpeedServo j2;
VarSpeedServo j3;

String pos;
int pos_x;

void setup() {
  j1.attach(2, 1, 180);
  j2.attach(4, 1, 180);
  j3.attach(6, 1, 180);

  Serial.begin(115200);

}

void loop() {
//  j1.write(100, 50, true);
//  j2.write(100, 50, true);
//  j3.write(100, 50, true);
//  j1.write(75, 50, true);
//  j2.write(75, 50, true);
//  j3.write(75, 50, true);
//  j1.write(50, 50, true);
//  j2.write(50, 50, true);
//  j3.write(50, 50, true);
//  j1.write(25, 50, true);
//  j2.write(25, 50, true);
//  j3.write(25, 50, true);

//  j1.slowmove(100, 50);
//  j2.slowmove(100, 50);
//  j3.slowmove(100, 50);
//  j1.slowmove(75, 50);
//  j2.slowmove(75, 50);
//  j3.slowmove(75, 50);
//  j1.slowmove(50, 50);
//  j2.slowmove(50, 50);
//  j3.slowmove(50, 50);
//  j1.slowmove(25, 50);
//  j2.slowmove(25, 50);
//  j3.slowmove(25, 50);

//    j1.sequencePlay(0, 100, true, 0);

//  for (int x = 0; x <= 180; x++){
//    j1.write(x, 50, true);
//    Serial.println(j1.read());
//  }

  if(Serial.available() > 0){
    pos = Serial.readString();
    j1.write(pos.toInt(), 50, true);
    Serial.println(j1.read());
  }


}
