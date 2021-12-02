#include "version.h"
#include "greetings.h"
#include <ArduinoJson.h>
#include <VarSpeedServo.h>

VarSpeedServo j1;
VarSpeedServo j2;
VarSpeedServo j3;
VarSpeedServo j4;
VarSpeedServo j5;
VarSpeedServo j6;
VarSpeedServo j7;

DynamicJsonDocument data(1024);

String input;

void setup() {
//  j1.attach(0);
//  j2.attach(1);
//  j3.attach(2);
//  j4.attach(3);
  j5.attach(4);
  j6.attach(5);
  j7.attach(6);

  Serial.begin(115200);
  Serial.print(GREETINGS);
  Serial.print(VERSION_NB);

}

void loop() {
  if(Serial.available() > 0){    
//    Serial.println("");
//    Serial.println("Calibration of servo-motors");
//    Serial.print("pos-j1: ");
//    Serial.println(j1.read());
//    Serial.print("pos-j2: ");
//    Serial.println(j2.read());
//    Serial.print("pos-j3: ");
//    Serial.println(j3.read());
//    Serial.print("pos-j4: ");
//    Serial.println(j4.read());
//    Serial.print("pos-j5: ");
//    Serial.println(j5.read());
//    Serial.print("pos-j6: ");
//    Serial.println(j6.read());
//    Serial.print("pos-j7: ");
//    Serial.println(j7.read());
    
    input = Serial.readString();
    deserializeJson(data, input);
//    Serial.println("\nStarting procedures");

//    if(data["j1"][2].as<String>() == "1"){
//      j1.write(data["j1"][0].as<int>(), data["j1"][1].as<int>(), true);
//    }
//    else{
//      j1.write(data["j1"][0].as<int>(), data["j1"][1].as<int>(), false);
//    }
//
//    Serial.print("pos-j1: ");
//    Serial.println(j1.read());

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

//    if(data["j4"][2].as<String>() == "1"){
//      j4.write(data["j4"][0].as<int>(), data["j4"][1].as<int>(), true);
//    }
//    else{
//      j4.write(data["j4"][0].as<int>(), data["j4"][1].as<int>(), false);
//    }

//    Serial.print("pos-j4: ");
//    Serial.println(j4.read());
    
    if(data["j5"][2].as<String>() == "1"){
      j5.write(data["j5"][0].as<int>(), data["j5"][1].as<int>(), true);
    }
    else{
      j5.write(data["j5"][0].as<int>(), data["j5"][1].as<int>(), false);
    }

    Serial.print("pos-j5: ");
    Serial.println(j5.read()); 
      
    if(data["j6"][2].as<String>() == "1"){
      j6.write(data["j6"][0].as<int>(), data["j6"][1].as<int>(), true);
    }
    else{
      j6.write(data["j6"][0].as<int>(), data["j6"][1].as<int>(), false);
    }

    Serial.print("pos-j6: ");
    Serial.println(j6.read()); 
    
    if(data["j7"][2].as<String>() == "1"){
      j7.write(data["j7"][0].as<int>(), data["j7"][1].as<int>(), true);
    }
    else{
      j7.write(data["j7"][0].as<int>(), data["j7"][1].as<int>(), false);
    }

    Serial.print("pos-j7: ");
    Serial.println(j7.read());

    Serial.println("End of execution");
  }         
}
