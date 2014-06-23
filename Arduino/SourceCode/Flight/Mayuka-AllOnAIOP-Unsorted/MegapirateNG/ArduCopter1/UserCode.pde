// agmatthews USERHOOKS
// agmatthews USERHOOKS

void userhook_init()
{
  // put your initialisation code here

}

void userhook_50Hz()
{
  int Height =0;
  int  setHeight =60;
  THROTTLE_INPUT=g.rc_3.control_in ;
  Height= sonar_alt;
  /*Semi-auto Mode*/
  if (Mode == 1)                //Semi-auto mode
  {
    if(OnGround == true)         //Check whether Quad is on ground,height less than 16cm
    {
      if(Height<setHeight)      //SetHeight : 100 cm , Check Height from sonar
      {          
        if((THROTTLE_INPUT > 1350) && (THROTTLE_INPUT < 1650))    //Throttle stick in MIDDLE position
        {
          Auto_Takeoff();            // Activate auto take off function
        }
      }
    }

    else                            // The flying machine has taken off
    {
      if(THROTTLE_INPUT < 1100)     // If throttle stick is pushed all the way down
      {
      Auto_Land();                 //Activate auto landing function
      }
    }
  }






}


void Auto_Takeoff()                
{  

  uint32_t AutoTOtimer = millis();
  if((millis()-AutoT0timer) > 200)    // Change throttle every 200 millisecond
  {

    THROTTLE_TRIM += 20;             // increase the throttle by 20
    if(THROTTLE_TRIM > 1500)          // reset the throttle if it extends 1700 ( To prevent from full throttle)
      THROTTLE_TRIM = 1000;          
  //  AutoT0mode = true;                // set the auto take off mode flag
    AutoT0timer = millis();
    g.rc_3.servo_out = THROTTLE_TRIM;
  }
}

void Auto_Land()
{
 int  Height=sonar_alt;
  
  
  if(OnGround == false)                // If the flying machine is still in air 
  {
//    if( Height > 150)                  // if the flying machine is higher than 1.5 m
//      AltSetpoint = 130;                  // set the altitude setpoint to 1.3 m
//    else
//      AltSetpoint = 0;                      // After if goes below 1.5 m, set the altitude setpoint to 0 for landing
  
  
  uint32_t AutoLDtimer = millis();
    if((millis()-AutoLDtimer) > 200)    // Change throttle every 200 millisecond
  {

    THROTTLE_TRIM -= 20;             // increase the throttle by 20
    if(THROTTLE_TRIM < 1000)          // reset the throttle if it extends 1700 ( To prevent from full throttle)
      THROTTLE_TRIM = 1500;          
  //  AutoT0mode = true;                // set the auto take off mode flag
    AutoLDtimer = millis();
    g.rc_3.servo_out = THROTTLE_TRIM;
  }
}


if(Height<20)
{
  
  OnGround= true;
}
}
