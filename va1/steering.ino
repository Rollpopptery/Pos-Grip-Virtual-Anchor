

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

// can't use enums until Arduino fixes them up,
// so they don't have to be put in a header file
// until then, use constants
//
const int POS_UNKOWN = 0;
const int POS_HOME_CLOCKWISE = 1;
const int POS_HOME_ANTICLOCK = 2;

// Steering Motor control
//
// The steering has only 2 known positions:
//
// 1. Hit the home sensor from clockwise direction
// 2. Hit the home sensor from anti clockwise direction
//
// i.e there is no positional feedback from the steering motor.
//
//
// As the steering is controlled, we hit the home sensor
// We must not rotate through the home sensor.
// i.e The motor can't keep turning in one direction and wind up the cables.
//
// So, whenever we hit the home sensor, we make note of this.
// We know at this stage we are at one of the two known positions.
//
// As we rotate away from the home sensor (we must always rotate away from the Home position), 
// The poistion of the steering motor becomes Unkown again.
//




//Class with the state of the steering,
// Also, control functions for Steering
//
class Steering{ 

  public:
  
  // State of the steering 
  //
  // if this is greater than 0, then the motor is moving
  //
  int motorSpeed  ;
  int motorDirection  ;

  // timer, shows the start time of the current movement
  //
  unsigned long timer  ;
  unsigned long duration ;

  // Steering position
  //
  int Position ;

 
  void Stop();
  void StartMove(int dir, int Speed, unsigned long timeout);

  // called whenever we are moving
  //
  void checkForPosSensor();
  
  
  
};


// The object
//
Steering steering;



// The hall effect home sensor; check to see if we have hit it, 
// Stop the motor if we have
// Maintain the position variable
//
void Steering::checkForPosSensor()
{
  int homeSense = digitalRead(DIGITAL_HOME_SENSE);

  if (homeSense == 0)
  {
    // we have hit the home sensor   
    // stop the motor 
    //
    Stop();

    // We know our position
    //    
    if (motorDirection == CLOCKWISE)
    {
      Position = POS_HOME_CLOCKWISE;      
    }
    else
    {
      Position = POS_HOME_ANTICLOCK;
    }     
  }
  else
  {
    // if the sensor is 1, then we don't know where we are
    //
    Position = POS_UNKOWN;
    
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
  timer = millis();
  duration = timeout;
  
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



//--------------------------------------------------------------------
// called repeatedly from main task loop, this is the Steering 'Thread'
//
//
void Steering_StateMachine()
{
  // is Steering moving ? , monitor the timeout
  //
  if (steering.motorSpeed > 0)
  {
    if ( (unsigned long)(millis() - steering.timer) > steering.duration)
    {
      // finished move / timeout
      //
      steering.Stop();    
      
    }

    // check for home sensor
    //
    steering.checkForPosSensor();    
  } 
  
}
