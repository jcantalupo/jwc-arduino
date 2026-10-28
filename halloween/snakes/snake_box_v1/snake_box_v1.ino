#include <Stepper.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int relay = 2;
bool popLid = false;

//RF24 radio(7, 8); // CE, CSN
RF24 radio(7, 8); // CE, CSN. RFnano
//const byte addresses[][6] = {"00001", "00002"};
const byte addresses[][6] = {"Prime", "2Node", "3xxxx", "4xxxx"};

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte fogPos = 0;    // tranmit to fog machine
  byte byte02 = 0;    // open lid
  byte byte03 = 0;
  byte byte04 = 0;
  byte byte05 = 0;
  byte byte06 = 0;
  byte byte07 = 0;
  byte byte08 = 0;
  byte byte09 = 0;
  byte byte10 = 0;
  byte byte11 = 0;
  byte lidSignal = 0;
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


void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
}

void loop() {
  // test
   //delay(2000);
   //openLid();

  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    /*
    Serial.print("fogPos: ");
    Serial.print(data.fogPos);
    Serial.print("  Received: ");
    Serial.println(data.fogPos, DEC);
    */
    Serial.print("lidSignal: ");
    Serial.print(data.lidSignal);
    Serial.print("  Received: ");
    Serial.println(data.lidSignal, DEC);
    //Serial.print("byte20: ");
    //Serial.println(data.byte20);
    //Serial.print("byte21: ");
    //Serial.println(data.byte21);
    // Set popLid
    // No need to debounce because of 500ms delay in openLid,
    // and controller automatically resets value to 0 on any other button press.
    // Note that controller only sends signal if switch A4 is also on (up).
    if (data.lidSignal == 1) {
      popLid = true;
    }
  }

  if (popLid) { 
    openLid(); 
    popLid = false;
  }

}


void openLid() {
  // LOW activates relay
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(relay, LOW);
  Serial.println("Lid open");
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(relay, HIGH);
  Serial.println("Lid closed");
}