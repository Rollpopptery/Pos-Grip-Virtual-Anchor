

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


#include "steering.h"


// Globals


// IO Mapping ---Arduino LEONARDO---
//

int DIGITAL_HOME_SENSE = 7;
int DIGITAL_STEER_MOTOR = 12;
int DIGITAL_STEER_DIR = 11;
int DIGITAL_STEER_FB = 3;    // feedback pulses from steering motor (can have interrupts)

extern Steering steering;

extern void gpsSetup(void);
extern void gpsPoll(void);
extern void Steering_StateMachine(void);

extern void I2Csetup(void);

void setup() {

  //I2Csetup();
 // HMC588setup();
  
  pinMode(DIGITAL_HOME_SENSE, INPUT_PULLUP);
  
   
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }  

  Serial.println("Setup GPS...");

  gpsSetup();

  delay(200);

  //steering.Init();

  

  
 // home the steering
 //
 //homeRoutine();
 //steering.StartMove(ANTICLOCK, 5, 40000);
 //steering.RotateToAngle(270);  


}



void loop() {



  static unsigned long gpsTimer = millis();
  static unsigned long directionTimer = millis();

  unsigned long now = millis();

   

  
  if ((now - gpsTimer)  > 20)
  {
    // check on the GPS recievers
    //
   // Serial.println("poll GPS...");
    gpsTimer = now;
    gpsPollFront();
    gpsPollRear();
    
  }

  if ((now - directionTimer) > 2000)
  {
    directionTimer = now;
    gpsCalculateDriection();
  }
  
  
  // Call the various statemachines (effectively simultaneous threads)
  //
  Steering_StateMachine();
 
    
   
  

}
