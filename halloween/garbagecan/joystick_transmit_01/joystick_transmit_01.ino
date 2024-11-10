#include <stdio.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio setup
RF24 radio(9,10); // CE, CSN
const byte address[6] = "00001";

unsigned long x;

const int btn_up = 2;
const int btn_right = 3;
const int btn_down = 4;
const int btn_left = 5;
const int btn_joy = 8;
const int ulimit = 400;
const int llimit = 280;

void setup() {
  Serial.begin(9600);

  pinMode(btn_up, INPUT_PULLUP);
  pinMode(btn_right, INPUT_PULLUP);
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_left, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(btn_joy, INPUT_PULLUP);

  // radio transmit setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  
//  Serial.print(digitalRead(2));
//  Serial.print("  ");
//  Serial.print(digitalRead(3));
//  Serial.print("  ");
//  Serial.print(digitalRead(4));
//  Serial.print("  ");
//  Serial.print(digitalRead(5));
//  Serial.print("  ");
//  Serial.print(digitalRead(6));
//  Serial.print("  ");
//  Serial.print(digitalRead(7));

  char tbs[6];
  //sprintf(tbs, "%4d%4d%3d", analogRead(A0), analogRead(A1), digitalRead(8));
  //Serial.println(tbs);

  int x = analogRead(A0);
  int y = analogRead(A1);
  
  // transmit joystick values
  if ( (middle(y)) && (middle(x)) ) { 
    Serial.println("STOP");
    const char text[] = "S";
    radio.write(&text, sizeof(text));   
  }
  
  if ( (y > ulimit) && (middle(x)) ) { 
    Serial.println("joystick up");
    const char text[] = "0";
    radio.write(&text, sizeof(text));   
  }
  if ( (y > ulimit) && (x > ulimit) ) { 
    Serial.println("joystick up-right");
    const char text[] = "1";
    radio.write(&text, sizeof(text));   
  }
  if ( (middle(y)) && (x > ulimit) ) { 
    Serial.println("joystick right");
    const char text[] = "2";
    radio.write(&text, sizeof(text));   
  }
  if ( (y < llimit) && (x > ulimit) ) { 
    Serial.println("joystick down-right");
    const char text[] = "3";
    radio.write(&text, sizeof(text));   
  }
  if ( (y < llimit) && (middle(x)) ) { 
    Serial.println("joystick down");
    const char text[] = "4";
    radio.write(&text, sizeof(text));   
  }
  if ( (y < llimit) && (x < llimit) ) { 
    Serial.println("joystick down-left");
    const char text[] = "5";
    radio.write(&text, sizeof(text));   
  }
  if ( (middle(y)) && (x < llimit) ) { 
    Serial.println("joystick left");
    const char text[] = "6";
    radio.write(&text, sizeof(text));   
  }
  if ( (y > ulimit) && (x < llimit) ) { 
    Serial.println("joystick up-left");
    const char text[] = "7";
    radio.write(&text, sizeof(text));   
  }

  unsigned int i;
  if ( !digitalRead(btn_up) ) { 
    Serial.println("up - all up");
    const char text[] = "U";
    radio.write(&text, sizeof(text));   
  }

   if ( !digitalRead(btn_right) ) { 
    Serial.println("right - lid up");
    const char text[] = "L";
    radio.write(&text, sizeof(text));   
   }

   if ( !digitalRead(btn_down) ) { 
    Serial.println("down - both down");
    const char text[] = "D";
    radio.write(&text, sizeof(text));   
   }

}

bool middle(int val) {
  if ( (val < ulimit) && (val > llimit) ) {
    return true;
  }
  else {
    return false;
  }
}
