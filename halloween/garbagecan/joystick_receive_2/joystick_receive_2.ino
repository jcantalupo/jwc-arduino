#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(2, 10); // CE, CSN - Usually 9, 10
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;

// Motor A connections
const int rightEnable = 9;
const int rightA = 5;
const int rightB = 4;
// Motor B connections
const int leftEnable = 3;
const int leftA = 8;
const int leftB = 6;

const int lid = A0;
const int thing = A1;
const int signalLight = 7;

// Variables to track received data
int currentDirection = 99;
unsigned long int lastSignal = 0;
const int stopInterval = 500;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MAX);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
  Serial.println("starting");

  pinMode(rightEnable, OUTPUT);
  pinMode(leftEnable, OUTPUT);
  pinMode(rightA, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(leftA, OUTPUT);
  pinMode(leftB, OUTPUT);

  stopMotors();

  pinMode(lid, OUTPUT);
  pinMode(thing, OUTPUT);
  pinMode(signalLight, OUTPUT);

  digitalWrite(lid, HIGH);
  digitalWrite(thing, HIGH);
  digitalWrite(signalLight, LOW);
}

void loop()
{
 unsigned long int currentTime = millis();
 if (currentTime - lastSignal > stopInterval) {
   stopMotors();
   currentDirection = 99;
   Serial.println("No signal!");
   digitalWrite(signalLight, HIGH);
 }
  
 if (radio.available())              //Looking for the data.
 {
   //Serial.println("message received");
   char text[10] = "";                 //Saving the incoming data
   radio.read(&text, sizeof(text));    //Reading the data
   lastSignal = millis();
   digitalWrite(signalLight, LOW);

   switch(text[0]) {

     case 'S':
       if (currentDirection != 9) {
         Serial.println("S - STOP");
         stopMotors();
         currentDirection = 9;
       }
       break;
       
     case 'U':
       Serial.println("All Up");
        digitalWrite(lid, LOW);
        delay(500);
        digitalWrite(thing, LOW);
       break;
       
     case 'L':
       Serial.println("Lid Up");
       digitalWrite(lid, LOW);
       break;

     case 'D':
       Serial.println("Both Down");
       digitalWrite(thing, HIGH);
        delay(500);
        digitalWrite(lid, HIGH);
       break;

       case '0':
       if (currentDirection != 0) {
         Serial.println("0 - Forward");
         forward();
         currentDirection = 0;
       }
       break;
       
       case '1':
       if (currentDirection != 1) {
         Serial.println("1 - Right Forward");
         stopMotors();
         rightForward();
         currentDirection = 1;
       }
       break;

       case '2':
       if (currentDirection != 2) {
         Serial.println("2 - Right");
         leftForward();
         rightBackward();
         currentDirection = 2;
       }
       break;

       case '3':
       if (currentDirection != 3) {
         Serial.println("3 - Right Backward");
         stopMotors();
         rightBackward();
         currentDirection = 3;
       }
       break;

       case '4':
       if (currentDirection != 4) {
         Serial.println("4 - Backward");
         backward();
         currentDirection = 4;
       }
       break;

       case '5':
       if (currentDirection != 5) {
         Serial.println("5 - Left Backward");
         stopMotors();
         leftBackward();
         currentDirection = 5;
       }
       break;

       case '6':
       if (currentDirection != 6) {
         Serial.println("6 - Left");
         leftBackward();
         rightForward();
         currentDirection = 6;
       }
       break;

       case '7':
       if (currentDirection != 7) {
         Serial.println("7 - Left Forward");
         stopMotors();
         leftForward();
         currentDirection = 7;
       }
       break;
   } // switch
   } // if radio

 delay(5);
}

void stopMotors() {
  // Turn off motors
  digitalWrite(rightEnable, HIGH);
  digitalWrite(leftEnable, HIGH);

  }

void rightForward() {
  digitalWrite(rightEnable, HIGH);
  digitalWrite(rightA, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(rightEnable, LOW);
}

void rightBackward() {
  digitalWrite(rightEnable, HIGH);
  digitalWrite(rightA, HIGH);
  digitalWrite(rightB, HIGH);
  digitalWrite(rightEnable, LOW);
}

void leftForward() {
  digitalWrite(leftEnable, HIGH);
  digitalWrite(leftA, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(leftEnable, LOW);
}

void leftBackward() {
  digitalWrite(leftEnable, HIGH);
  digitalWrite(leftA, HIGH);
  digitalWrite(leftB, HIGH);
  digitalWrite(leftEnable, LOW);
}

void forward() {
  rightForward();
  leftForward();
}

void backward() {
  rightBackward();
  leftBackward();
}
