

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

//--------------------------------------------------------------
//  JGB37 Brushless Gear  Motor, 100 RPM
// 
// WIRE COLOURS
//-------------------
//
// RED =   Power 12V
// BLACK = 0V gnd
// YELLOW = 'FG'   pulses feedback, open collector (pull to ground)
// WHITE =  Direction control  CW or CCW
// BLUE - not used
//


#include "steering.h"


extern int DIGITAL_HOME_SENSE ;
extern int DIGITAL_STEER_MOTOR ;
extern int DIGITAL_STEER_DIR ;
extern int DIGITAL_STEER_FB ;





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
// The poistion of the steering motor becomes Unknown again.
//






// The object
//
Steering steering;



// Called repeatedly whenever the motor is moving
// 
// The hall effect home sensor; check to see if we have hit it, 
// Stop the motor if we have
// Maintain the position variable
//
void Steering::checkForPosSensor()
{
  int homeSense = digitalRead(DIGITAL_HOME_SENSE);

  if ((homeSense == 0))
  {
    // we have hit the home sensor   
    // Allow movement, if we are attempting to move away from the sensor in the correct direction
    // Otherwise keep the motor stopped.
    //
    if (Position == POS_UNKNOWN)
    {
      // Stop the motor
      //
      Stop(); 
    }
     

    if (motorDirection == CLOCKWISE)
      {
        // we are full rotated clockwise
        //
        Position = POS_HOME_ANTICLOCK;
        AnglePulses =  MAX_ANGLE;
      }      
   else
      {
        Position = POS_HOME_CLOCKWISE;
        AnglePulses =  0;
      }
      
    }      
   
  
  else  // home sensor is 1
  {
    // if the sensor is 1, then we don't know where we are
    // 
    Position = POS_UNKNOWN;
    
  }
     
}



// Go to an angle 
// Angle between 1 and 359 degrees
//
void Steering::RotateToAngle(int angle)
{
  int pulse_angle = 0;
  int dir = CLOCKWISE;
  double temp = 0.0;
  
  if ((angle < 1) or (angle > 359))
  {
    // invalid angle
    //
    return;
  }

  // convert angle from degrees to motor pulses
  //
  temp = (float)angle * (MAX_ANGLE/ MAX_ANGLE_DEGREES);
  pulse_angle = (int)temp;
  TargetAnglePulses = pulse_angle;

  
  // what direction should we move in ?
  //

  if (TargetAnglePulses > AnglePulses)
  {
    dir = CLOCKWISE;
  }
  else
  {
    dir = ANTICLOCK;
  }
  

  
  // start the motor
  //
  StartMove(dir, 5, 10000);
  motorState = ANGLE_MOVE;
 
  
  
}


//
// Called repeatdedly to drive the motor slowly
// This controls speed with fairly long pulses of motor power
//
void Steering::SlowMovement()
{
  // milli seconds
  //
  const unsigned int timeOn = 10;
  const unsigned int timeOff = 100;
  static int state = 0;
  
  static unsigned long moveTimer = millis();

  unsigned long now = millis();

  if (state == 1)
  {
    // we are in motor ON pulse
    //
     if ((now - moveTimer)  > timeOn)
     {
        // reset the timer
        //
        moveTimer = now;
        
        // motor OFF
        //
        state = 0;
        digitalWrite(DIGITAL_STEER_MOTOR, 0);         
     }   
     
  }
  else 
  {
    // we are in the OFF pulse
    //
    if ((now - moveTimer) > timeOff)
    {
      // reset the timer
      //
      moveTimer = now;
          
      // motor ON
      //
      state = 1;
      digitalWrite(DIGITAL_STEER_MOTOR, 1);    
    }
  }    
}



//
// Called repeatdedly to drive the motor quickly
// This controls speed with fairly long pulses of motor power
//
void Steering::FastMovement()
{
  // milli seconds
  //
  const unsigned int timeOn = 50;
  const unsigned int timeOff = 100;
  static int state = 0;
  
  static unsigned long moveTimer = millis();

  unsigned long now = millis();

  digitalWrite(DIGITAL_STEER_MOTOR, 1);  

  return;
  
  if (state == 1)
  {
    // we are in motor ON pulse
    //
     if ((now - moveTimer)  > timeOn)
     {
        // reset the timer
        //
        moveTimer = now;
        
        // motor OFF
        //
        state = 0;
        digitalWrite(DIGITAL_STEER_MOTOR, 0);         
     }   
     
  }
  else 
  {
    // we are in the OFF pulse
    //
    if ((now - moveTimer) > timeOff)
    {
      // reset the timer
      //
      moveTimer = now;
          
      // motor ON
      //
      state = 1;
      digitalWrite(DIGITAL_STEER_MOTOR, 1);    
    }
  }    
}


//-------------------------------------------------------------
// interrupt called whenever motor gives us a pulse
//
void fb_pulse()
{
  if (steering.motorDirection)
  {
    // clockwise
    //
    steering.AnglePulses++;
  }
  else
  {
    // anti clockwise
    //
    steering.AnglePulses--;
    
  }
  
}



void Steering::Init()
{
  int homeSense = 0;

  // setup digital IO
  //

  pinMode(DIGITAL_STEER_DIR, OUTPUT);
  digitalWrite(DIGITAL_STEER_DIR, 1);

  pinMode(DIGITAL_STEER_MOTOR, OUTPUT);
  digitalWrite(DIGITAL_STEER_MOTOR, 0);

  pinMode(DIGITAL_STEER_FB, INPUT);
  attachInterrupt(digitalPinToInterrupt(DIGITAL_STEER_FB), fb_pulse, FALLING);

  
  AnglePulses = 0 ;

  
  
  homeSense = digitalRead(DIGITAL_HOME_SENSE);

  if (homeSense == 0)
  {
    // assume we are in home from the clockwise direction !!
    //
    Position = POS_HOME_CLOCKWISE;
  }
  else
  {
    Position = POS_UNKNOWN;
  }
  
}

void Steering::Stop()
{
  
  digitalWrite(DIGITAL_STEER_MOTOR, 1);

  // update the state to 'stopped'
  //
  motorSpeed = 0; 
  digitalWrite(DIGITAL_STEER_MOTOR, 0);
  
  
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

  motorSpeed = Speed;
  motorDirection = dir;
  
  digitalWrite(DIGITAL_STEER_DIR, dir);

  // actual motor control is handled elsewhere
  //
  //digitalWrite(DIGITAL_STEER_MOTOR, 1);
  
}




// debug messages, when needed
//
void printDebug()
{
  Serial.print("Speed: ");
  Serial.println(steering.motorSpeed);

  Serial.print("Direction: ");
  Serial.println(steering.motorDirection);

  Serial.print("Angle in Pulses: ");
  Serial.println(steering.AnglePulses);

  Serial.print("Target Angle in Pulses: ");
  Serial.println(steering.TargetAnglePulses);

  Serial.print("Motor STATE: ");
  Serial.println(steering.motorState);

  Serial.print(" Home sensor: ");
  Serial.println(digitalRead(DIGITAL_HOME_SENSE));
    
  Serial.print("Position: ");
  if (steering.Position == POS_UNKNOWN)
  {
    Serial.println("UNKNOWN");    
  }
  else if (steering.Position ==  POS_HOME_CLOCKWISE)
  {
    Serial.println("POS_HOME_CLOCKWISE");    
  }
  else if (steering.Position ==  POS_HOME_ANTICLOCK)
  {
    Serial.println("POS_HOME_ANTICLOCK");    
  }
  else
  {
    Serial.println("ERROR");    
  }
  Serial.println();
  
}


//-----------------------------------------------------------------------
// only called once, on Power ON
// contains delays
//
//------------------------------------------------------------------------

void homeRoutine()
{
    int homeSense = 0;

    steering.motorState = HOMING;
    
    homeSense = digitalRead(DIGITAL_HOME_SENSE);


    // move clockwise a little bit
    //
    steering.StartMove(CLOCKWISE, 5, 40000);   
    digitalWrite(DIGITAL_STEER_MOTOR, 1);
    delay(50);
    
    // if we are off the sensor, we were on the clockwise side of the home position
    //
    
    
    steering.StartMove(ANTICLOCK, 5, 40000);   
    homeSense = digitalRead(DIGITAL_HOME_SENSE);

    // move anti-clockwise if we were on the sensor
    //
    while(homeSense == 0)
    {
       steering.SlowMovement();
       homeSense = digitalRead(DIGITAL_HOME_SENSE);
    }

    // move clockwise until hit the sensor
    //
    steering.Stop();
    steering.StartMove(CLOCKWISE, 5, 40000);   

    while(homeSense == 1)
    {
       steering.SlowMovement();
       homeSense = digitalRead(DIGITAL_HOME_SENSE);
    }

    steering.Stop();
    steering.Position = POS_HOME_ANTICLOCK;  // home and can move anti-clockwise
    steering.AnglePulses = 0;
    steering.motorState == NORMAL;

    steering.StartMove(ANTICLOCK, 5, 40000); 

    // we roughly know our absolute angle (fully rotated clockwise)
    //
    steering.AnglePulses = Steering::MAX_ANGLE ;
       

    // make sure we are off the sensor
    //
    digitalWrite(DIGITAL_STEER_MOTOR, 1);
    delay(50);
    steering.Stop();

    steering.motorState = NORMAL;
 
        
    
              
  
}


//--------------------------------------------------------------------
// called repeatedly from main task loop, this is the Steering 'Thread'
//
//
void Steering_StateMachine()
{
  static unsigned long debugTimer = millis();

  static float previous_angle_error = 9999;
  float angle_error = 0.0;

  if ((millis() - debugTimer) > 2000)
  {
    debugTimer = millis();
    printDebug();
   
  }
  
  // is Steering moving ? , monitor the timeout
  //
  if (steering.motorSpeed > 0)  
  {     
    
         if ( (millis() - steering.timer) > steering.duration)
         {
              // finished move / timeout
              //
              steering.Stop();         
          }
          steering.FastMovement();
          steering.checkForPosSensor(); 

          // if we are doing a 'move to angle'
          // we monitor for passing the angle
          //
          angle_error = fabs(steering.AnglePulses - steering.TargetAnglePulses);
          
          if (steering.motorState == ANGLE_MOVE)
          {
            
            if (angle_error <= previous_angle_error)
            {
              // we are still moving towards our angle
            }
            else
            {
              // we just passed our target angle
              //
              steering.Stop();
              steering.motorState == NORMAL;
            }
            previous_angle_error = angle_error;           

          }
          else
          {
            previous_angle_error = angle_error;
          }
          
          


                 
    
           
  }   
}
