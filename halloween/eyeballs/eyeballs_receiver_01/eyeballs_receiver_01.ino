//  Nilheim Mechatronics Simplified Eye Mechanism Code
//  Make sure you have the Adafruit servo driver library installed >>>>> https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
//  X-axis joystick pin: A1
//  Y-axis joystick pin: A0
//  Trim potentiometer pin: A2
//  Button pin: 2

// John Cantalupo
// This is a receiver program that takes commands from eueballs_transmitter_xx.ino
// via nrf24 radio signal.
// However, if pin 3 is LOW, then radio commands are ignored and it will run in
// AUTO mode, doing random motion and/or responding to signals from two ultrasonic
// sensors.
// 10/28/24 Barely started radio mods.

// For servos
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// For nrf24
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int boxLight = 2;
const float senseDistance = 100.0;

RF24 radio(7, 8);  // CE, CSN - Usually 9, 10
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

Data_Package data;  // Create a variable with the above structure

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
const int trigPin = 5;
const int echoPin = 6;
float duration, distance;

const int trigPin2 = 10;
const int echoPin2 = 9;
float duration2, distance2;

bool autoModeLocal = false;

unsigned long int lastUpDown = 0;
bool localPlatformUp = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pinMode(analogInPin, INPUT);
  //pinMode(2, INPUT);  // Was blink button, repurposed for light.

  // DIP switch 1
  // ON = eyballs in automatic mode - all motion commands are ignored
  // OFF = respond to tranmitter motion commands
  pinMode(3, INPUT_PULLUP);

  pinMode(boxLight, OUTPUT);

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  radio.begin();
  radio.setChannel(70);
  radio.openReadingPipe(0, address);  //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MAX);      //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();             //This sets the module as receiver

  delay(10);
  pwm.setPWM(12, 0, 520);
}


void loop() {
  /* big servo test. 120 seems to be close to lower limit. 120-600 is just over 180 degrees.
  pwm.setPWM(12, 0, 520); // down
  delay(3000);
  pwm.setPWM(12, 0, 320);
  blink(2000);
  delay(3000);
*/

  int autoSwitch = digitalRead(3);
  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed.
  if (autoSwitch == HIGH) {
    autoModeLocal = false;
  }
  else {
    autoModeLocal = true;
    //Serial.println("in autoMode");
  }

  if ( (autoModeLocal) || (data.autoMode) ) {

    if ( millis() - lastUpDown > random(20000,35000) ) {
      localPlatformUp = 1-localPlatformUp;
      lastUpDown = millis();
    }

    if (localPlatformUp) {
        pwm.setPWM(12, 0, 320);
        digitalWrite(boxLight, HIGH);
    }
    else {
        pwm.setPWM(12, 0, 520);
        digitalWrite(boxLight, LOW);
    }

    if (localPlatformUp) {

        // Check ultrasonic sensors
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration * .0343) / 2;
        //Serial.print("Distance: ");
        //Serial.println(distance);
        digitalWrite(trigPin2, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin2, LOW);
        duration2 = pulseIn(echoPin2, HIGH);
        distance2 = (duration2 * .0343) / 2;
        //Serial.println(distance2);

        if (distance < senseDistance) {
          //Serial.println(distance);
          lookLeft(random(300,1400)); // 500
          delay(random(0,1000));
          blink2(random(50,200));
          if (random(0,2) == 1) {
            delay(random(40,600));
            blink2(random(50,600));
          }
        }

        if (distance2 < senseDistance) {
          //Serial.println(distance);
          lookRight(random(300,1400)); // 500
          delay(random(0,1000));
          blink2(random(50,200));
          if (random(0,2) == 1) {
            delay(random(40,600));
            blink2(random(50,600));
          }
        }

    } // localPlatformUp

  } // end local

  // Read packet from radio
  if (radio.available()) {
    //Serial.println("radio is available");
    radio.read(&data, sizeof(Data_Package));  // Read the whole data and store it into the 'data' structure
    //Serial.print(data.lexpulse); Serial.print(' '); Serial.println(data.leypulse);

    if ( (!autoModeLocal) && (!data.autoMode) ) {
        // Set eye positon
        pwm.setPWM(0, 0, data.lexpulse);
        pwm.setPWM(1, 0, data.leypulse);

        // eyelids
        pwm.setPWM(2, 0, data.servo2);
        pwm.setPWM(3, 0, data.servo3);
        pwm.setPWM(4, 0, data.servo4);
        pwm.setPWM(5, 0, data.servo5);

        if (data.platformUp) {
            pwm.setPWM(12, 0, 320);
            digitalWrite(boxLight, HIGH);
        }
        else {
            pwm.setPWM(12, 0, 520);
            digitalWrite(boxLight, LOW);
        }

    }

  }  // end radio loop

  delay(5);
}

// Function to blink at a desired rate
void blink2(int blinkDuration) {
  pwm.setPWM(2, 0, 400);
  pwm.setPWM(3, 0, 240);
  pwm.setPWM(4, 0, 240);
  pwm.setPWM(5, 0, 400);
  delay(blinkDuration);
  pwm.setPWM(2, 0, 280);
  pwm.setPWM(3, 0, 400);
  pwm.setPWM(4, 0, 400);
  pwm.setPWM(5, 0, 280);
}

// Function to
void lookLeft(int lookDuration) {
  pwm.setPWM(0, 0, 439);
  pwm.setPWM(1, 0, 499);
  delay(lookDuration);
  pwm.setPWM(0, 0, 330);
  pwm.setPWM(1, 0, 371);
}

void lookRight(int lookDuration) {
  pwm.setPWM(0, 0, 220);
  pwm.setPWM(1, 0, 499);
  delay(lookDuration);
  pwm.setPWM(0, 0, 330);
  pwm.setPWM(1, 0, 371);
}

void eyeRoll(int rollDelay) {
  pwm.setPWM(0, 0, 330);
  pwm.setPWM(1, 0, 499);
  delay(rollDelay);
  pwm.setPWM(0, 0, 439);
  pwm.setPWM(1, 0, 499);
  delay(rollDelay);
  pwm.setPWM(0, 0, 439);
  pwm.setPWM(1, 0, 371);
  delay(rollDelay);
  pwm.setPWM(0, 0, 439);
  pwm.setPWM(1, 0, 250);
  delay(rollDelay);
  pwm.setPWM(0, 0, 341);
  pwm.setPWM(1, 0, 250);
  delay(rollDelay);
  pwm.setPWM(0, 0, 221);
  pwm.setPWM(1, 0, 250);
  delay(rollDelay);
  pwm.setPWM(0, 0, 220);
  pwm.setPWM(1, 0, 372);
  delay(rollDelay);
  pwm.setPWM(0, 0, 220);
  pwm.setPWM(1, 0, 499);
  delay(rollDelay);
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
  blink2(400);
  delay(200);
  blink2(400);
  delay(1000);
  lookLeft(1500);
  delay(1000);
  lookRight(1500);
  delay(1000);
  blink2(100);
  delay(200);
  blink2(100);
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
