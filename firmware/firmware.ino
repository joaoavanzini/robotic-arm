//{"j1":[40, 10, 1], "j2":[60, 10, 1], "j3":[80, 10, 1]}



#include <ArduinoJson.h>
#include <VarSpeedServo.h>

VarSpeedServo j1;
VarSpeedServo j2;
VarSpeedServo j3;

DynamicJsonDocument data(1024);

String input;

void setup() {
  j1.attach(2, 1, 180);
  j2.attach(4, 1, 180);
  j3.attach(6, 1, 180);

  Serial.begin(115200);

}

void loop() {
//  if(Serial.available() > 0){
//    input = Serial.readString();
//    
//    deserializeJson(data, input);
//
//    if(data["j1"][2].as<String>() == "1"){
//      j1.write(data["j1"][0].as<int>(), data["j1"][1].as<int>(), true);
//    }
//    else{
//      j1.write(data["j1"][0].as<int>(), data["j1"][1].as<int>(), false);
//    }
//
//    Serial.print("pos-j1: ");
//    Serial.println(j1.read());
//
//    if(data["j2"][2].as<String>() == "1"){
//      j2.write(data["j1"][0].as<int>(), data["j2"][1].as<int>(), true);
//    }
//    else{
//      j2.write(data["j2"][0].as<int>(), data["j2"][1].as<int>(), false);
//    }
//
//    Serial.print("pos-j2: ");
//    Serial.println(j2.read());
//
//    if(data["j3"][2].as<String>() == "1"){
//      j3.write(data["j3"][0].as<int>(), data["j3"][1].as<int>(), true);
//    }
//    else{
//      j3.write(data["j3"][0].as<int>(), data["j3"][1].as<int>(), false);
//    }
//
//    Serial.print("pos-j3: ");
//    Serial.println(j3.read());
//  }

    for (int i = 0; i <= 180; i++){
      j1.write(i, 100, false);
      j2.write(180-i, 100, false);
      Serial.print("pos-j1: ");
      Serial.println(j1.read());
      Serial.print("pos-j2: ");
      Serial.println(j2.read());
      delay(25);
     }
    for (int x = 180; x >= 0; x--){
      j1.write(x, 100, false);
      j2.write(180-x, 100, false);
      Serial.print("pos-j1: ");
      Serial.println(j1.read());
      Serial.print("pos-j2: ");
      Serial.println(j2.read());
      delay(25);    
     }    
    
}
