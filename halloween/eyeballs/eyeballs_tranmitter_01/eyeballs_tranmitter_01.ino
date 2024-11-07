//  Nilheim Mechatronics Simplified Eye Mechanism Code
//  Make sure you have the Adafruit servo driver library installed >>>>> https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//  X-axis joystick pin: A1
//  Y-axis joystick pin: A0
//  Trim potentiometer pin: A2
//  Button pin: 2

// John Cantalupo
// This is a tranmitter program that takes sensor input and uses nrf20
// radios to send it to a receiver.
// Servo code is stripped out.
// 10/28/24 Code completed - needs testing.

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int camPin = A3;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  boolean autoMode = false;
  boolean platformUp = false;
  int lexpulse = 0;
  int leypulse = 0;
  int servo2 = 0;
  int servo3 = 0;
  int servo4 = 0;
  int servo5 = 0;
};

Data_Package data; // Create a variable with the above structure

int xval;
int yval;

int lexpulse;
int rexpulse;

int oldlex;
int oldley;

int leypulse;
int reypulse;

int uplidpulse;
int lolidpulse;
int altuplidpulse;
int altlolidpulse;

int trimval;

const int analogInPin = A0;
int sensorValue = 0;
int outputValue = 0;
int switchval = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Eyeball control tranmitter!");
  pinMode(analogInPin, INPUT);
  pinMode(2, INPUT); // blink button

  // DIP switch 1 
  // ON = eyballs in automatic mode - all motion commands are ignored
  // OFF = respond to tranmitter motion commands
  pinMode(3,INPUT_PULLUP);
  // Toggle switch to turn cam to raise/lowe platform
  pinMode(camPin,INPUT_PULLUP);

  // radio transmit setup
  radio.begin();
  radio.setChannel(70);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  delay(10);
}


void loop() {

  int autoSwitch = digitalRead(3);
  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. 
  if (autoSwitch == HIGH) {
    data.autoMode = false;
  } else {
    data.autoMode = true;
    //Serial.println("autoMode is ON");
  }

  int camSwitch = digitalRead(camPin);
  if (camSwitch == HIGH) {
    data.platformUp = false;
    Serial.println("platform down");
  } else {
    data.platformUp = true;
    Serial.println("platform up");
  }

  // Map A0 from joystick to left-right eye position
  xval = analogRead(A0);
  lexpulse = map(xval, 0, 1023, 220, 440); // original
  rexpulse = lexpulse;  // never used

  // Check blink button
  switchval = digitalRead(2);

  // Map A1 from joystick to up-down eye position
  yval = 1023 - analogRead(A1);
  leypulse = map(yval, 0, 1023, 250, 500); // original
  reypulse = map(yval, 0, 1023, 400, 280); // never used

  // Check pot for to set eyelid rest position
  trimval = analogRead(A2);
  trimval = map(trimval, 320, 580, -40, 40);
  uplidpulse = map(yval, 0, 1023, 400, 280);
  uplidpulse -= (trimval - 40);
  uplidpulse = constrain(uplidpulse, 280, 400);
  altuplidpulse = 680 - uplidpulse;

  lolidpulse = map(yval, 0, 1023, 410, 280);
  lolidpulse += (trimval / 2);
  lolidpulse = constrain(lolidpulse, 280, 400);
  altlolidpulse = 680 - lolidpulse;

  // Save old eye postion values solely so for output convenience
  if ( (abs(oldlex-lexpulse) > 1) || (abs(oldley-leypulse) > 1) ) {
      //Serial.print(xval); Serial.print(","); Serial.print(yval); Serial.print("   ");
      //Serial.print(lexpulse); Serial.print(","); Serial.println(leypulse);
      oldlex = lexpulse;
      oldley = leypulse;
  }

  // Set up struct
  // data.autoMode is set at beginning of loop.
  data.lexpulse = lexpulse;
  data.leypulse = leypulse;
  if (switchval == HIGH) {
    data.servo2 = 400;
    data.servo3 = 240;
    data.servo4 = 240;
    data.servo5 = 400;
  } else if (switchval == LOW) {
    data.servo2 = uplidpulse;
    data.servo3 = lolidpulse;
    data.servo4 = altuplidpulse;
    data.servo5 = altlolidpulse;
  }

  //Serial.print("Sending:  ");
  //Serial.print(data.lexpulse); Serial.print(' '); Serial.println(data.leypulse);
  radio.write(&data, sizeof(Data_Package));

/*
  // Set eye positon
  pwm.setPWM(0, 0, lexpulse);
  pwm.setPWM(1, 0, leypulse);

  // blink
  if (switchval == HIGH) {
    pwm.setPWM(2, 0, 400);
    pwm.setPWM(3, 0, 240);
    pwm.setPWM(4, 0, 240);
    pwm.setPWM(5, 0, 400);
  } else if (switchval == LOW) {
    pwm.setPWM(2, 0, uplidpulse);
    pwm.setPWM(3, 0, lolidpulse);
    pwm.setPWM(4, 0, altuplidpulse);
    pwm.setPWM(5, 0, altlolidpulse);
  }
*/

  delay(5);
}
