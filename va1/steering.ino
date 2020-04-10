

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



// single 'singleton' class with the state of the steering,
// Also, control functions for Steering
//
class Steering{ 


  public:
  
  // State of the steering 
  //
  // if this is greater than 0, then the motor is moving
  //
  static int motorSpeed  ;
  static int motorDirection  ;

  // timer, shows the start time of the current movement
  //
  static unsigned long timer  ;
  static unsigned long duration ;

 
  
  static void Stop();
  static void StartMove(int dir, int Speed, unsigned long timeout);
  
  
  
};



//
// called repeatedly from main task loop, this is the Steering 'Thread'
//
void Steering_StateMachine()
{
  // is Steering moving ? , monitor the timeout
  //
  if (Steering::motorSpeed > 0)
  {
    if ( (unsigned long)(millis() - Steering::timer) > Steering::duration)
    {
      // finished move / timeout
      //
      Steering::Stop();    
      
    }
  }

  
  
  
}

void Steering::Stop()
{
  
  digitalWrite(DIGITAL_STEER_MOTOR, 1);

  // update the state to 'stopped'
  //
  motorSpeed = 0;
  
  
}



// start the motor moving
// provide a time / timeout.
// dir is the Direction
//
void Steering::StartMove(int dir, int Speed, unsigned long timeout)
{

  // set the direction , and start the motor
  //
  Steering::timer = millis();
  Steering::duration = timeout;
  
  digitalWrite(DIGITAL_STEER_DIR, dir);
  digitalWrite(DIGITAL_STEER_MOTOR, 1);
  
}


















//---- ignore below here


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
