// Code for Attiny85
// Switchless mod for Megadrive
//
// Code by: Kamencesc
// Board by: Kamencesc
//
// Site: http://www.kamencesc.com/
// 24-07-2019
//--------------------------------------------------------------------
//    * ATtiny25/45/85 pinout
//                 __  __
// LED1     5/A0 -| 1`´8 |- VCC
// LED2     3/A3 -| 2  7 |- 2/A1    BTN
// HERZ     4/A2 -| 3  6 |- 1       RST
//           GND -|_4__5_|- 0       COUNTRY
//
//--------------------------------------------------------------------

#include <EEPROM.h>

//config reset pins
const int btn = 2;
const int rst = 1 ;
//config dual led pins
const int led1 = 5;
const int led2 = 3;
//config mode pins
const int country = 0;
const int herz = 4;

int btnState = 0;
int btnPress = 0;

//config fist region region: 1 for EU, 2 for USA, 3 for JAPAN
int region = 1;

int timepress = 0;

//eeprom
int address = 0;
int value;

void setup() {
  //config pinmodes

  //reset button
  pinMode(rst, OUTPUT);
  pinMode(btn, INPUT);
  //dual led
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  //modes
  pinMode(country, OUTPUT);
  pinMode(herz, OUTPUT);

  value = EEPROM.read(address);
  if ((value > 0) && (value < 4)) {
        region=value;
  } else {
     EEPROM.write(address, region);
  }
  changeRegion(region);
}

void changeRegion (int val) {
  switch (val) {
    case 1:                         //EU
      digitalWrite(country, LOW );
      digitalWrite(herz, HIGH );
      digitalWrite(led1, LOW );
      digitalWrite(led2, HIGH );
      EEPROM.write(address, region);
      region++;
    case 2:                         //USA
      digitalWrite(country, HIGH );
      digitalWrite(herz, HIGH );
      digitalWrite(led1, HIGH );
      digitalWrite(led2, HIGH );
      EEPROM.write(address, region);
      region++;
    case 3:                         //JAPAN
      digitalWrite(country, HIGH );
      digitalWrite(herz, LOW );
      digitalWrite(led1, HIGH );
      digitalWrite(led2, LOW );
      EEPROM.write(address, region);
      region = 1;
  }
}

void loop() {
  btnState = digitalRead(btn);

  if (!btnState) {                  //GND = Reset is pressed
    while (!btnState) {             //while btn is pressed
      delay(200);                   //delay of 200ms
      timepress++;                  //timepress increment
      btnState = digitalRead(btn);  //read btn again
    }
    if (timepress >= 10) {          //if timepress 10*200ms = 2000ms = 2 seconds
      digitalWrite(rst, LOW);       //send LOW to simulate reset press
      delay(10);                    //little delay
      digitalWrite(rst, HIGH);      //back to HIGH
    } else {
      changeRegion(region);
    }
  }
}
