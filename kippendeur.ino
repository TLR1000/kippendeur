//kippendeurNodemcu
// Wiring:
// D1 --> SCL DS3231 
// D2 --> SDA DS3231
// D5 --> Reed Switch boven
// D6 --> Reed Switch onder
// D7 --> Relais 1
// D8 --> Relais 2

#include <Wire.h>
#include "DS3231.h"

//reed schakelaar
const int reedBoven = D5;
const int reedOnder = D6;

//Relais
const int relais1 = D7;
const int relais2 = D8;

RTClib RTC;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(57600);
    //klok
    Wire.begin();

    pinMode(reedBoven, INPUT_PULLUP);
    pinMode(reedOnder, INPUT_PULLUP);  

    pinMode(relais1, OUTPUT);
    pinMode(relais2, OUTPUT);

    relais("uit");
}

void loop() {
    delay(500);
    //Tijd ophalen
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    if(now.hour() >= 9){
      if(now.hour() < 22){
        dagStand();
      } else {
        nachtStand();
      }
    } else {
      nachtStand();
    }
}

String leesSensors(){
    Serial.println("leesSensors functie");
    //reed switches
    // 0 close - 1 open switch
    // dus: boven = 0 --> deur is open/omhoog
    //      beneden = 0 --> deur is gesloten/omlaag
    int boven;
    int onder;
    boven = digitalRead(reedBoven);
    onder = digitalRead(reedOnder);
//    Serial.print("onder:");
//    Serial.println(onder);
//    Serial.print("boven:");
//    Serial.println(boven);
    if (boven == 0){
       //Serial.println("Deur is open");
       return "open";
    } else if (onder == 0){
       //Serial.println("Deur is gesloten");
       return "gesloten";
    } else {
       //Serial.println("Deur is onbekend");
       return "onbekend";
    }
}


void nachtStand() {
    Serial.println("nachtStand functie");
    String stand;
    stand = leesSensors();
    Serial.print("sensors:");
    Serial.println(stand);
    if (stand != "gesloten"){
    relais("sluiten");
    while (stand != "gesloten"){
      stand = leesSensors();
      Serial.println("loopen tot gesloten");
      Serial.print("sensors:");
      Serial.println(stand);
      delay(100); //0,1 seconde
      }
    relais("uit");
    } else {
      //niks 
      }
}    

void dagStand() {
    Serial.println("dagStand functie");
    String stand;
    stand = leesSensors();
    Serial.print("sensors:");
    Serial.println(stand);    
    if (stand != "open"){
    relais("openen");
    while (stand != "open"){
      Serial.println("loopen tot open");
      stand = leesSensors();
      Serial.print("sensors:");
      Serial.println(stand);      
      delay(100); //0,1 seconde
      }
    relais("uit");
    } else {
      //niks 
      }
} 

void relais(String functie){
  if (functie == "openen"){
    Serial.println("ok");
    digitalWrite(relais1, HIGH); 
    digitalWrite(relais2, LOW);       
    }
  if (functie == "sluiten"){
    Serial.println("ok");
    digitalWrite(relais1, LOW); 
    digitalWrite(relais2, HIGH);       
    }
  if (functie == "uit"){
    Serial.println("ok");
    digitalWrite(relais1, LOW); 
    digitalWrite(relais2, LOW);       
    }
}
