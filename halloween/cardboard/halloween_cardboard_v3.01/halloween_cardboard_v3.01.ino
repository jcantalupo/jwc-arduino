// Halloween cardboard stuff
// New version written by Dad - Nov 2026.
// Box with 3 arcade buttons controls
// skeleton hand box and swing-up eyeballs.
// Circuit board uses RFnano in case we want
// to bring back radio control.

#include <Servo.h>

Servo skelHand; // pin 3
Servo skelLid;  // pin 2
Servo eyesA;    // pin 4
Servo eyesB;    // pin 5

const int skelHandRest = 12; // Move to 180
const int skelLidRest = 160; // Move to 60
const int eyesDown = 30;
const int eyesUp   = 180;

const int btn_green = 6;
const int btn_yellow = 7;

const int leds[] = {A0, A1, A2, A3, A4, A5};


void setup() {
  Serial.begin(9600);
  Serial.println("Cardboard program initialized.");

  // Arcade buttons
  pinMode(btn_green, INPUT_PULLUP);
  pinMode(btn_yellow, INPUT_PULLUP);

  // Skeleton hand box
  skelHand.attach(3);
  skelLid.attach(2);
  eyesA.attach(4);
  eyesB.attach(5);

  skelHand.write(skelHandRest);
  skelLid.write(skelLidRest);
  eyesA.write(eyesDown);
  eyesB.write(eyesDown);

  for (int i = 0; i < 6; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

}

void loop() {
  //testButtons();
  //testSkel();
  if (digitalRead(btn_green) == LOW) {
    Serial.println("Green is pressed!");
    skelLid.write(60);
    //delay(1000);
    //flashLEDs();
    flashDelay(400);
    skelHand.write(180);
    // Hand is now up
    //delay(1000);
    //flashLEDs();
    flashDelay(600);
    skelHand.write(skelHandRest);
    flashDelay(400);
    skelLid.write(skelLidRest);
  }

  if (digitalRead(btn_yellow) == LOW) {
    Serial.println("Yellow is pressed!");
    eyesA.write(eyesUp);
    delay(500);
    eyesB.write(eyesUp);
    delay(2000);
    eyesA.write(eyesDown);
    delay(500);
    eyesB.write(eyesDown);
  }

}


void flashLEDs () {
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[i], HIGH);
    delay(166);
    digitalWrite(leds[i], LOW);
  }
}

void flash(int chance){
  if (random(0, chance) == 0){
    int randomNum = random(0, 9);  
    digitalWrite(leds[randomNum], HIGH);
    delay(10);
    digitalWrite(leds[randomNum], LOW);
  }
}

void flashDelay(int timee){
  for (int i = 1; i <= (timee / 10); i++) {
    flash(1); delay(10);
  }
}

void testSkel() {
  delay(2000);
  Serial.println("Moving skelHand!");
  skelHand.write(180);
  delay(1000);
  skelHand.write(10);
  //
  delay(2000);
  Serial.println("Moving skelLid!");
  skelLid.write(90);
  delay(1000);
  skelLid.write(20);
}

void testButtons() {
  if (digitalRead(btn_green) == LOW) {
    Serial.println("Green is pressed!");
  }
  if (digitalRead(btn_yellow) == LOW) {
    Serial.println("Yellow is pressed!");
  }
}
