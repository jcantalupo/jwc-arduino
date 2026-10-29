#include <Stepper.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
New version for Fall 2026 with gear-driven turntable.
*/

RF24 radio(7, 8); // CE, CSN
//const byte addresses[][6] = {"00001", "00002"};
const byte addresses[][6] = {"Prime", "2Node", "3xxxx", "4xxxx"};

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte fogPos = 0;    // tranmit to fog machine
  byte byte02 = 0;
  byte byte03 = 0;
  byte byte04 = 0;
  byte byte05 = 0;
  byte byte06 = 0;
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

Data_Package data; 

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int inc = 20;
int dir = 1;  // -1=cw, 1=ccw, 0=stopped
const int cwStop = 2;
const int ccwStop = 3;
byte fogPosition = 49;
byte prevPosition = 49;
byte incomingByte;
int steps = 0;

const int pos5 = -1920;
const int pos4 = -1440;
const int pos3 = -960;
const int pos2 = -480;

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper myStepper(stepsPerRevolution, A0, A2, A1, A3);


void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(10);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(cwStop, INPUT_PULLUP);
  pinMode(ccwStop, INPUT_PULLUP);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  Serial.println("Initializing postion...");
}

void loop() {

  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    Serial.print("fogPos: ");
    Serial.println(data.fogPos);
    Serial.print("Received: ");
    Serial.println(data.fogPos, DEC);
    prevPosition = fogPosition;
    fogPosition = data.fogPos;
  }

  /* test back and forth
  // step one increment in current direction:
  Serial.println(dir);
  myStepper.step(inc * dir);
  delay(5);

  // if endstop is hit, change direction
  if ( dir == -1 and not digitalRead(cwStop) ) {
    Serial.println("go counterclockwise");
    dir = 1;
  }
  else if ( dir == 1 and not digitalRead(ccwStop) ) {
    Serial.println("go clockwise");
    dir = -1;
  }
  */

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte != 10) {
      prevPosition = fogPosition;
      fogPosition = incomingByte;
      Serial.print("Entered: ");
      Serial.println(fogPosition, DEC);
    }
  }

  switch(fogPosition) {
    case 49:
      //Serial.println("1");
      dir = 1;
      while(digitalRead(ccwStop)){
        Serial.println("moving 1");
        myStepper.step(inc * dir);
        //delay(5);
      }
      stepperOff();
      steps = 0;
      break;
    case 50:
      //Serial.println("2 - no action yet");
      if (prevPosition < fogPosition) {
        dir = -1;
      }
      else {
        dir = 1;
      }
      while(steps != pos2 and digitalRead(mapDir(dir)) ) {
        myStepper.step(inc * dir);
        steps += inc * dir;
        Serial.println(steps);
      }
      stepperOff();
      break;
    case 51:
      //Serial.println("3 - no action yet");
      if (prevPosition < fogPosition) {
        dir = -1;
      }
      else {
        dir = 1;
      }
      while(steps != pos3 and digitalRead(mapDir(dir)) ) {
        myStepper.step(inc * dir);
        steps += inc * dir;
        Serial.println(steps);
      }
      stepperOff();
      break;
    case 52:
      //Serial.println("4 - no action yet");
      if (prevPosition < fogPosition) {
        dir = -1;
      }
      else {
        dir = 1;
      }
      while(steps != pos4 and digitalRead(mapDir(dir)) ) {
        myStepper.step(inc * dir);
        steps += inc * dir;
        Serial.println(steps);
      }
      stepperOff();
      break;
    case 53:
      //Serial.println("5");
      dir = -1;
      while(digitalRead(cwStop)){
        myStepper.step(inc * dir);
        //steps += inc * dir;
        //Serial.println(steps);
        //delay(5);
      }
      steps = pos5;
      stepperOff();
      break;
  }

}

void stepperOff() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

bool eStop() {
  if ( !digitalRead(cwStop) or !digitalRead(ccwStop) ) {
    return true;
  }
  else {
    return false;
  }
}

int mapDir(int inDir) {
  if (inDir == -1) return 2;
  else return 3;
}
