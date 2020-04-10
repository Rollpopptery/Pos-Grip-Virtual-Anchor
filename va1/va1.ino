

//-------------------------------------------------------------------
//
// Virtual Anchor Control System
//
// Created 10-April-2020
//
//
//
// Modified 10-April-2020  RVW
//
//
//-------------------------------------------------------------------



// Globals


// IO Mapping ---Arduino LEONARDO---
//

int DIGITAL_HOME_SENSE = 2;
int DIGITAL_STEER_MOTOR = 12;
int DIGITAL_STEER_DIR = 11;


void setup() {
  
  pinMode(DIGITAL_HOME_SENSE, INPUT_PULLUP);

  pinMode(DIGITAL_STEER_DIR, OUTPUT);
  digitalWrite(DIGITAL_STEER_DIR, 1);


  pinMode(DIGITAL_STEER_MOTOR, OUTPUT);
  digitalWrite(DIGITAL_STEER_MOTOR, 0);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }  

  delay(1000);


  //steering_home();

}



void loop() {
  
  delay(1000);
  

  Serial.println(" re ");
  

  //digitalWrite(DIGITAL_STEER_MOTOR, 1);
  //delay(5000);
  //digitalWrite(DIGITAL_STEER_MOTOR, 0);
  //delay(5000);
  

}
