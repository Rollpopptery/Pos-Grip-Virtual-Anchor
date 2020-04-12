
#ifndef _STEERING_H
#define _STEERING_H



const int CLOCKWISE = 1;
const int ANTICLOCK = 0;


const int POS_UNKNOWN = 0;
const int POS_HOME_CLOCKWISE = 1;
const int POS_HOME_ANTICLOCK = 2;


enum STATES_STEERING{NORMAL, HOMING, ANGLE_MOVE};

// Class with the state of the steering,
// Also, control functions for Steering
//
class Steering{ 


  ;

  public:
  
  // State of the steering 
  //
  // if this is greater than 0, then the motor is moving
  //
  int motorSpeed  ;
  int motorDirection  ;

  const static int MAX_ANGLE = 2800;
  const static float MAX_ANGLE_DEGREES = 340.0;

  STATES_STEERING motorState ;

  // timer, shows the start time of the current movement
  //
  unsigned long timer  ;
  unsigned long duration ;

  // Steering position
  //
  int Position ;  // 3 posible known positions (HOME from clockwise, HOME from anti clockwise, UNKNOWN)
  int AnglePulses ;
  int Angle;

  int TargetAnglePulses ;  // the desired angle

 
  void Stop();
  void StartMove(int dir, int Speed, unsigned long timeout);

  void RotateToAngle(int angle);

  // called once when object is made
  //
  void Init();
  
  // called frequently by state machine/s
  //
  void checkForPosSensor();
  void SlowMovement();
  void FastMovement();
 
 
    
};


#endif
