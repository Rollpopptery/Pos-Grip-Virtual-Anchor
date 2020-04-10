

//-------------------------------------------------------------------
//
// Virtual Anchor Control System
//
//
// Steering module
//
//
//
// Modified 10-April-2020  RVW
//
//
//-------------------------------------------------------------------



extern int DIGITAL_HOME_SENSE ;
extern int DIGITAL_STEER_MOTOR ;
extern int DIGITAL_STEER_DIR ;

const int CLOCKWISE = 1;
const int ANTICLOCK = 0;

// generic move function, includes breaking pause
//
// dir = direction
//

void Mmove(int dir)
{

  if (dir > 1)  return;
  if (dir < 0) return;  
  

  // stop
  //
  digitalWrite(DIGITAL_STEER_MOTOR, 0);
  delay(500);

  // start movement
  //
  digitalWrite(DIGITAL_STEER_DIR, dir);
  digitalWrite(DIGITAL_STEER_MOTOR, 1);
  
  
}


void smallMove(int dir)
{
  digitalWrite(DIGITAL_STEER_DIR, dir);
  digitalWrite(DIGITAL_STEER_MOTOR, 1);
  delay(50);  
  digitalWrite(DIGITAL_STEER_MOTOR, 0);
  delay(100);
 
  
  
}

void Mstop()
{
  digitalWrite(DIGITAL_STEER_MOTOR, 0);
}


void steering_home()
{
  

  int homeSense = digitalRead(DIGITAL_HOME_SENSE);
  unsigned long timeout = millis();


  Mmove(ANTICLOCK);
  delay(2000);


  timeout = millis();
 

  while(  (((unsigned long) millis() - timeout) < 10000)   && (homeSense == 1) )
  {
    smallMove(CLOCKWISE);   
    homeSense =  digitalRead(DIGITAL_HOME_SENSE);
    Serial.println(homeSense);
  }

  delay(1000);

  // Stop motor
  //
  Mstop();
  


  // Motor Anti-Clockwise
  //
  timeout = millis();
  
  
  while(  (((unsigned long) millis() - timeout) < 10000)   && (homeSense == 0) )
  {
   
    smallMove(ANTICLOCK);
    homeSense =  digitalRead(DIGITAL_HOME_SENSE);
    Serial.println(homeSense);
  }

  // Stop motor
  //
  Mstop();
  
}
