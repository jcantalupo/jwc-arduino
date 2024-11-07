//  Nilheim Mechatronics Simplified Eye Mechanism Code
//  Make sure you have the Adafruit servo driver library installed >>>>> https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//  X-axis joystick pin: A1
//  Y-axis joystick pin: A0
//  Trim potentiometer pin: A2
//  Button pin: 2

// John Cantalupo
// This program has been modied to take ultrasonc sensor input  and do some basic automatic 
// Move ments based on that.


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 140  // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 520  // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0;

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

// ultrasonic sensors
const int trigPin = 10;
const int echoPin = 9;
float duration, distance;

const int trigPin2 = 5;
const int echoPin2 = 6;
float duration2, distance2;


void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pinMode(analogInPin, INPUT);
  pinMode(2, INPUT);

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  delay(10);
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;  // 1,000,000 us per second
  pulselength /= 60;      // 60 Hz
  Serial.print(pulselength);
  Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength);
  Serial.println(" us per bit");
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
}

void loop() {

  // Check ultrasonic sensors
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  //Serial.print("Distance: ");
  //Serial.println(distance);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2*.0343)/2;
  Serial.println(distance2);

  if (distance < 10.0) {
    //Serial.println(distance);
    lookLeft(500);
  }

    if (distance2 < 10.0) {
    Serial.println(distance2);
    lookRight(500);
  }

  // Map A0 from joystick to left-right eye position
  xval = analogRead(A0);
  lexpulse = map(xval, 0, 1023, 220, 440); // original
  //lexpulse = map(xval, 0, 1023, 195, 415);  
  rexpulse = lexpulse;  // never used

  // Check blink button
  switchval = digitalRead(2);

  // Map A1 from joystick to up-down eye position
  yval = 1023 - analogRead(A1);
  leypulse = map(yval, 0, 1023, 250, 500); // original
  //leypulse = map(yval, 0, 1023, 280, 530);
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

  delay(5);
}

// Function to blink at a desired rate
void blink(int blinkDuration) {
    pwm.setPWM(2, 0, 400);
    pwm.setPWM(3, 0, 240);
    pwm.setPWM(4, 0, 240);
    pwm.setPWM(5, 0, 400);
    delay(blinkDuration);
    pwm.setPWM(2, 0, uplidpulse);
    pwm.setPWM(3, 0, lolidpulse);
    pwm.setPWM(4, 0, altuplidpulse);
    pwm.setPWM(5, 0, altlolidpulse);
}

// Function to 
void lookLeft(int lookDuration) {
  pwm.setPWM(0, 0, 439);
  pwm.setPWM(1, 0, 499);
  delay(lookDuration);
  //pwm.setPWM(0, 0, 330);
  //pwm.setPWM(1, 0, 371);
}

void lookRight(int lookDuration) {
  pwm.setPWM(0, 0, 220);
  pwm.setPWM(1, 0, 499);
  delay(lookDuration);
  //pwm.setPWM(0, 0, 330);
  //pwm.setPWM(1, 0, 371);
}

void eyeRoll(int rollDelay) {
  pwm.setPWM(0, 0, 330); pwm.setPWM(1, 0, 499); delay(rollDelay);
  pwm.setPWM(0, 0, 439); pwm.setPWM(1, 0, 499); delay(rollDelay);
  pwm.setPWM(0, 0, 439); pwm.setPWM(1, 0, 371); delay(rollDelay);
  pwm.setPWM(0, 0, 439); pwm.setPWM(1, 0, 250); delay(rollDelay);
  pwm.setPWM(0, 0, 341); pwm.setPWM(1, 0, 250); delay(rollDelay);
  pwm.setPWM(0, 0, 221); pwm.setPWM(1, 0, 250); delay(rollDelay);
  pwm.setPWM(0, 0, 220); pwm.setPWM(1, 0, 372); delay(rollDelay);
  pwm.setPWM(0, 0, 220); pwm.setPWM(1, 0, 499); delay(rollDelay);
  //pwm.setPWM(0, 0, 330); pwm.setPWM(1, 0, 371); delay(rollDelay);
}

void home() {
  pwm.setPWM(0, 0, 330); 
  pwm.setPWM(1, 0, 371);
}

void testMotion() {
  delay(3000);
  eyeRoll(80);
  eyeRoll(80);
  home();
  delay(1000);
  blink(400);
  delay(200);
  blink(400);
  delay(1000);
  lookLeft(1500);
  delay(1000);
  lookRight(1500);
  delay(1000);
  blink(100);
  delay(200);
  blink(100);
}