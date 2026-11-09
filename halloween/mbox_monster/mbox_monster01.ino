#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
// DY1703A Voice Sound Module

Servo myservo;

RF24 radio(7, 8); // CE, CSN
// Also uses 7, 8, 11, 12, 13, 

const byte addresses[][6] = {"Prime", "2Node", "3xxxx", "4xxxx"};
const int relay1a = A0;
const int leds = A1;
const int motor = 10;
int sounds[5] = {6,5,4,3,2};
int busy = 9;
unsigned long int endIgnore = 0;

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
  byte byte12 = 0;
  byte byte13 = 0;
  byte byte14 = 0;
  byte byte15 = 0;
  byte byte16 = 0;
  byte byte17 = 0;
  byte byte18 = 0;
  byte byte19 = 0;
  byte byte20 = 0;
  byte byte21 = 0;
  byte byte22 = 0;
  byte byte23 = 0;
  byte byte24 = 0;
  byte byte25 = 0;
  byte byte26 = 0;
  byte byte27 = 0;
  byte byte28 = 0;
  byte byte29 = 0;
  byte byte30 = 0;
  byte byte31 = 0;
  byte byte32 = 0;
};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);

  pinMode(relay1a, OUTPUT);
  digitalWrite(relay1a,HIGH);
  pinMode(leds, OUTPUT);
  digitalWrite(leds,HIGH);
  //pinMode(motor, OUTPUT);
  //digitalWrite(motor, LOW);
  //myservo.attach(motor);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  for (int i=0; i<=4; i++) {
    //Serial.print("Initializing ");  Serial.println(i);
    pinMode(sounds[i], OUTPUT);
    digitalWrite(sounds[i], HIGH);
  }
  pinMode(busy, INPUT);

  Serial.println("Starting");
  //test_mbox3();
 /*
  delay(5000);
  Serial.print("playing... ");
  digitalWrite(5, LOW);
  delay(50);
  digitalWrite(5, HIGH);
  */
}

void loop() {

  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    Serial.print("size: ");
    Serial.println(sizeof(Data_Package));
    Serial.print("mbox: ");
    Serial.println(data.mbox);
    if (data.mbox == 99 && millis() < endIgnore) {
      data.mbox = 0;
    }
    Serial.println(digitalRead(busy));
  }

  if ((data.mbox >= 1) && (data.mbox <=7) && (digitalRead(busy))) {
    // Need to have 0 as off, but shift input value for zero-ordered array.
    digitalWrite(sounds[data.mbox-1], LOW);
    delay(50);
    digitalWrite(sounds[data.mbox-1], HIGH);
    digitalWrite(leds,HIGH);
    delay(1000);
    digitalWrite(leds,LOW);
    while (!digitalRead(busy)) {
      //Serial.println("waiting");
      Serial.print("Playing ");  Serial.print(data.mbox-1); Serial.print(sounds[data.mbox-1]);
      //Serial.print("Busy: "); Serial.println(digitalRead(busy));
      delay(1000);
    }
    data.mbox = 0;
  } 

/*
  // Eyes and motor
  if (data.mbox == 99 && millis() > endIgnore) {
    Serial.println("set endIgnore");
    endIgnore = millis()+5000;
    digitalWrite(leds,HIGH);
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    delay(1000);
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    data.mbox = 0;
  }
  */

  // Turn off LEDs after configured duration.
  if (millis() > endIgnore) {
    digitalWrite(leds,LOW);
    //digitalWrite(motor, LOW);
  }



}

void test_mbox() {
  for (int i=0; i<=4; i++) {
    Serial.print("Play ");  Serial.print(i); Serial.println(sounds[i]);
    digitalWrite(sounds[i], LOW);
    delay(50);
    digitalWrite(sounds[i], HIGH);
    delay(10000);
  }
}

void test_mbox2() {
  for (int i=0; i<=4; i++) {
    Serial.print("Busy: "); Serial.println(digitalRead(busy));
    Serial.print("Play ");  Serial.print(i); Serial.print(" "); Serial.println(sounds[i]);
    digitalWrite(sounds[i], LOW);
    delay(50);
    Serial.print("Busy: "); Serial.println(digitalRead(busy));
    digitalWrite(sounds[i], HIGH);
    delay(10000);
  }
}

void test_mbox3() {
  for (int i=0; i<=4; i++) {
    Serial.print("Play ");  Serial.print(i); Serial.print(" "); Serial.println(sounds[i]);
    digitalWrite(sounds[i], LOW);
    delay(1000);
    digitalWrite(sounds[i], HIGH);
    while (!digitalRead(busy)) {
      Serial.println("waiting");
      //Serial.print("Busy: "); Serial.println(digitalRead(busy));
      delay(1000);
    }
    //Serial.print("Busy: "); Serial.println(digitalRead(busy));
    Serial.println("Wait 2 seconds for next song");
    delay(2000);
  }
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
  data.byte12 = 0;
  data.byte13 = 0;
  data.byte14 = 0;
  data.byte15 = 0;
  data.byte16 = 0;
  data.byte17 = 0;
  data.byte18 = 0;
  data.byte19 = 0;
  data.byte20 = 0;
  data.byte21 = 0;
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
