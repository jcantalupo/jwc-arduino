// Full working version.
// Uses RF-Nano 5V
// Black box with 8 buttons, 2 switched, two knobs.
// Updated 10/2026 for snake box.
// Updated 10/22/26 for zipline ghost.
// Updated 10/25 to add mbox growl control.

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>

RF24 radio(10,9); // CE, CSN
const byte addresses[][6] = {"Prime", "2Node", "3xxxx", "4xxxx"};

#define PIN            8
#define NUMPIXELS      2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// buttons
// fog control
const int btn1 = 2;
const int btn2 = 3;
const int btn3 = 4;
const int btn4 = 5;
const int btn5 = 6;

const int btn6 = A0;  // ghost zipline move
const int btn7 = A1;  // growl
const int btn8 = A2;  // snake box open
bool btn1val;
bool btn2val;
bool btn3val;
bool btn4val;
bool btn5val;
bool btn6val;
bool btn7val;
bool btn8val;

const int pot1 = A6;  // ghost zip line speed
const int pot2 = A7;
int pot1val;
int pot2val;

const int sw1 = A3;  // ghost zipline direction
const int sw2 = A4;  // snake box armed
bool sw1val;
bool sw2val;


bool buttonPress = false;
bool motorOn = false;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte fogPos = 0;    // tranmit to fog machine
  byte controller = 0;
  byte blue = 0;
  byte device1a = 0;
  byte plasmaTV = 0;
  byte mbox = 0;
  byte byte07 = 0;
  byte byte08 = 0;
  byte byte09 = 0;
  byte byte10 = 0;
  byte byte11 = 0;
  byte lidSignal = 0;  // byte12 A2 btn8
  byte byte13 = 0;
  byte lidEnable = 0;  // byte14 A4 sw2
  byte byte15 = 0;
  byte byte16 = 0;
  byte byte17 = 0;
  byte byte18 = 0;
  byte byte19 = 0;
  byte motorDirection = 0;  // byte20
  byte motorSpeed = 0;  // byte21
  byte byte22 = 0;
  byte byte23 = 0;
  byte byte24 = 0;
  byte byte25 = 0;  //byte25
  byte byte26 = 0;
  byte byte27 = 0;
  byte byte28 = 0;
  byte byte29 = 0;
  byte byte30 = 0;
  byte byte31 = 0;
  byte byte32 = 0;
};

Data_Package data; // Create a variable with the above structure

void setup() {
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btn5, INPUT_PULLUP);
  pinMode(btn6, INPUT_PULLUP);
  pinMode(btn7, INPUT_PULLUP);
  pinMode(btn8, INPUT_PULLUP);

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);

  pixels.begin();

  Serial.begin(9600);
  Serial.println("Test tranmitter!");

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  delay(10);
  pixels.setPixelColor(0, pixels.Color(0,0,0)); // This pixel is currently unused.
}


void loop() {

  btn1val = digitalRead(btn1);
  btn2val = digitalRead(btn2);
  btn3val = digitalRead(btn3);
  btn4val = digitalRead(btn4);
  btn5val = digitalRead(btn5);

  btn6val = digitalRead(btn6);
  sw1val = digitalRead(sw1);
  pot1val = analogRead(pot1);

  btn8val = digitalRead(btn8);
  sw2val = digitalRead(sw2);

  // Unused at this time
  btn7val = digitalRead(btn7);
  pot2val = analogRead(pot2);

  /*
  Serial.print("btn1: "); Serial.println(btn1val);
  Serial.print("btn2: "); Serial.println(btn2val);
  Serial.print("btn3: "); Serial.println(btn3val);
  Serial.print("btn4: "); Serial.println(btn4val);
  Serial.print("btn5: "); Serial.println(btn5val);
  Serial.print("btn6: "); Serial.println(btn6val);
  Serial.print("btn7: "); Serial.println(btn7val);
  Serial.print("btn8: "); Serial.println(btn8val);
  
  Serial.print("pot1: ");
  Serial.println(pot1val);
  Serial.print("pot2: ");
  Serial.println(pot2val);
  
  Serial.print("sw1: ");
  Serial.println(sw1val);
  Serial.print("sw2: ");
  Serial.println(sw2val);
  */


  // Check fog controller buttons
  if (!digitalRead(btn1)) {
      buttonPress = true;
      clearData();
      data.fogPos = 49;
      pixels.setPixelColor(1, pixels.Color(0,0,20)); // RGB
      pixels.show();
  }
  if (!digitalRead(btn2)) {
      buttonPress = true;
      clearData();
      data.fogPos = 50;
      pixels.setPixelColor(1, pixels.Color(10,10,0)); // RGB
      pixels.show();
  }
  if (!digitalRead(btn3)) {
      buttonPress = true;
      clearData();
      data.fogPos = 51;
      pixels.setPixelColor(1, pixels.Color(10,10,10)); // RGB
      pixels.show();
  }
  if (!digitalRead(btn4)) {
      buttonPress = true;
      clearData();
      data.fogPos = 52;
      pixels.setPixelColor(1, pixels.Color(20,0,0)); // RGB
      pixels.show();
  }
  if (!digitalRead(btn5)) {
      buttonPress = true;
      clearData();
      data.fogPos = 53;
      pixels.setPixelColor(1, pixels.Color(0,20,0)); // RGB
      pixels.show();
  }

  if (!digitalRead(btn7)) {
      buttonPress = true;
      clearData();
      data.mbox = random(1,5);
  } else {
    data.mbox = 0;
  }

  if (!digitalRead(btn8) && !digitalRead(sw2)) {
      buttonPress = true;
      clearData();
      data.lidSignal = 1;
  } else {
    data.lidSignal = 0;
  }

  // ghost zipline
  if (!digitalRead(btn6)) {
      buttonPress = true;
      motorOn = true;
      clearData();
      data.motorSpeed = map(pot1val, 0, 1023, 0, 255);
      if (!digitalRead(sw1)) {
        data.motorDirection = 1;
        Serial.print("forward ");
      } else {
        data.motorDirection = 2;
        Serial.print("backward ");
      }
      Serial.println(data.motorSpeed);
  } else {
    // Need to send a signal to turn motor off
    if (motorOn) {
      motorOn = false;
      data.motorDirection = 0;
      buttonPress = true;
      Serial.println("Send motor off");
    }
  }

  

  if (buttonPress) {
      Serial.print("Sending fog position: ");
      Serial.println(data.fogPos);
      radio.stopListening();
      radio.write(&data, sizeof(Data_Package));
      radio.startListening();
      buttonPress = false;
  }
  
  delay(50);
}

void clearData() {
  data.fogPos = 0;
  data.blue = 0;
  data.controller = 0;
  data.device1a = 0;
  data.plasmaTV = 0;
  data.mbox = 0;
  data.byte07 = 0;
  data.byte08 = 0;
  data.byte09 = 0;
  data.byte10 = 0;
  data.byte11 = 0;
  data.lidSignal = 0;
  data.byte13 = 0;
  data.lidEnable = 0;
  data.byte15 = 0;
  data.byte16 = 0;
  data.byte17 = 0;
  data.byte18 = 0;
  data.byte19 = 0;
  data.motorDirection = 0;
  //data.motorSpeed = 0;
  data.byte22 = 0;
  data.byte23 = 0;
  data.byte24 = 0;
  data.byte25 = 0;
  data.byte26 = 0;
  data.byte27 = 0;
  data.byte28 = 0;
  data.byte29 = 0;
  data.byte30 = 0;
  data.byte31 = 0;
  data.byte32 = 0;
}
