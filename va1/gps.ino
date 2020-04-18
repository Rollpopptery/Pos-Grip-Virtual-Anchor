//-------------------------------------------------------------------
//
// Virtual Anchor Control System
//
// GPS Module
//
// Created 10-April-2020
//
// Two GPS recievers are used to give orientation and position of the Vessel 
// Communication with the chosen GPS modules is quite bulky , they are polled at the minimum rate.
// 
// --- Device Arduino LEONARDO ---
//
// Modified 18-April-2020  RVW
//
//
//-------------------------------------------------------------------


#include <SoftwareSerial.h> 
#include <TinyGPS.h> 


float latF = 28.5458,lonF = 77.1703; // create variable for latitude and longitude object  (front of vessel)
float latR = 28.5458,lonR = 77.1703;  // rear of vessel

double vesselDirection = 0;


SoftwareSerial gpsSerialFront(9,2); //rx,tx 

TinyGPS gpsFront; // create gps object (The front reciever)
TinyGPS gpsRear;  // The aft reciever



void gpsSetup(){ 

  gpsSerialFront.begin(9600); // connect gps sensor 
  Serial1.begin(9600); // connect gps sensor 

} 


// called once every 0.1 seconds
//
void gpsPollFront(){ 

  // front reciever
  //  
  while(gpsSerialFront.available()){ // check for gps data
             
    if(gpsFront.encode(gpsSerialFront.read()))// encode gps data 
    {  
      gpsFront.f_get_position(&latF,&lonF); // get latitude and longitude   

      int sats = gpsFront.satellites();

      String latitude = String(latF,6); 
      String longitude = String(lonF,6); 
      Serial.print("F " + latitude+";"+longitude+ "  "); 
      Serial.println(sats);
      //Serial.print("NORTH: ");
      //Serial.println(gps.course_to(lat, lon, -25.527961,152.700355));  // North 
 
    } 
  }  
  
}

void gpsPollRear() {

  // rear reciever
  //  
  while(Serial1.available()){ // check for gps data
             
    if(gpsRear.encode(Serial1.read()))// encode gps data 
    {  
      gpsRear.f_get_position(&latR,&lonR); // get latitude and longitude   

      int sats = gpsRear.satellites();

      String latitude = String(latR,6); 
      String longitude = String(lonR,6); 
      Serial.print("R "+latitude+";"+longitude+ "  "); 
      Serial.println(sats);
      //Serial.print("NORTH: ");
      //Serial.println(gps.course_to(lat, lon, -25.527961,152.700355));  // North 
 
     } 
  }   
}



// update global variable: vesselDirection
//
void gpsCalculateDriection() {

  int quadrant = 0;
  double lonDif = 0.0;
  double latDif = 0.0;
  double angle = 0.0;
  double vesselLength = 0.0;



  lonDif = lonF - lonR;
  latDif = latF - latR;

  String difX = String(lonDif,6); 
  String difY = String(latDif,6); 

  Serial.print("lon lat dif = ");
  Serial.println(difX + " " + difY);

  // catch divde by zero
  //  
  if  (latDif == 0)
  {
    latDif = 0.000001;
  }
  if  (lonDif == 0)
  {
    lonDif = 0.000001;
  }   

  vesselLength = sqrt((latDif * latDif) + (lonDif *lonDif));

  angle = round( atan2 (latDif, lonDif) * 180/3.14159265 ); // radians to degrees and rounding

  angle = (double) (((int)angle + 360) % 360);

  Serial.print("quadrant ANGLE = ");
  Serial.println(angle);

  String strLength = String(vesselLength,6); 

  Serial.print("Length: ");
  Serial.println(strLength);

  
  if (latR <= latF)
  {
    // pointing north
    //
    if (lonR <= lonF)
    {
      // pointing east
      //
      quadrant = 1; //NE
    }
    else
    {
      // pointing west
      //
      quadrant = 4; //NW

      angle += 270.0;
    }    
  }
  else
  {
    // pointing south
    //
    if (lonR <= lonF)
    {
      // pointing east
      //
      quadrant = 2; //SE
      angle += 90.0;
    }
    else
    {
      // pointing west
      //
      quadrant = 3;  //SW
      angle += 180.0;
    }
  }  


  Serial.print("QUADRANT = ");
  Serial.println(quadrant);

  

  
}
 
