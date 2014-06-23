// agmatthews USERHOOKS

static void user_read_control_switch()
{
    // this is a simpler version of read_control_switch() in control_modes.pde/ino.
    //it is the function that checks if the ch5 switch has been moved to a new position
    
    static uint8_t switch_counter = 0;

    byte switchPosition = readSwitch();

    if (userOldSwitchPosition != switchPosition) {
        switch_counter++;
        if(switch_counter >= 10) {
            userOldSwitchPosition       = switchPosition;
            switch_counter          = 0;

            user_set_mode(flight_modes[switchPosition]);
            //set_mode(flight_modes[switchPosition]);
        }
    }else{
        // reset switch_counter if there's been no change
        // we don't want 10 intermittant blips causing a flight mode change
        switch_counter = 0;
    }
}


static void user_dancing_lights()
{
    //this funciton should be used to test if the copter is indeed in a specific mode
    //if this function is executed the copter led's should "dance" that is switch between yellow blue and red repeatidly
    //adapted from dancing_light() in leds.pde/ino
    //to use this function make a case for for the flight mode you want the copter led's to blink for
    bool dance=false;
    switch(control_mode)
    {
        case USER_LAND:
            bool dance=true;
            break; 
        }
    
    if(dance)
    {
        static byte step;

        if (step++ == 3)
        step = 0;

        switch(step)
        {
        case 0:
            digitalWrite(C_LED_PIN, LED_OFF);
            digitalWrite(A_LED_PIN, LED_ON);
            break;

        case 1:
            digitalWrite(A_LED_PIN, LED_OFF);
            digitalWrite(B_LED_PIN, LED_ON);
            break;

        case 2:
            digitalWrite(B_LED_PIN, LED_OFF);
            digitalWrite(C_LED_PIN, LED_ON);
            break;
        }
    }
}




void user_set_mode(byte mode)
{
//this is the function that will change the flight mode to a user flight mode.
//must put a case for that flight mode here so that the flight mode will be changed
    bool new_mode=false;
    switch(mode)
    {
        case USER_LAND:
            //here is where you should set yaw roll/pitch and throttle modes you cna use either your own user defined modes 
            //or use a mode that is already defined in the source code. if using a user defined mode use user_set_(yaw/throttle/roll_pitch)_mode()
            //for one defined by the source use set_(yaw/throttle/roll_pitch)_mode()
            
            //this sets the three control modes to my own user defined modes
            user_set_roll_pitch_mode(USER_LAND_ROLL_PITCH_STABLE);
            user_set_yaw_mode(USER_LAND_YAW_HOLD);
            user_set_throttle_mode(USER_LAND_THROTTLE);
            new_mode=true;
            break;
        
    }
    if(new_mode)
    {
    control_mode=mode;
    }
    
}


void user_set_yaw_mode(uint8_t new_yaw_mode)
{
    //this is the function where you change the Yaw mode. call this function with the dsired mode as a parameter
    //as long as there is a case for that mode in this function the mode will be changed.
    bool yaw_initialised = false;
    
    switch(new_yaw_mode)
    {
        case USER_LAND_YAW_HOLD:
            // here is where you can initilze variables for your yaw mode
            //however do not actually put code here that doese somthing besides initilizeing variables
            // instead put code that controls the yaw in use_update_yaw_mode()
            
            //i have changed nothing because i copying dirrectly from the YAW_HOLD mode
            //which doese not need anything initilized here
            yaw_initialised = true;
            break;
    }
    
    if( yaw_initialised ) {
        yaw_mode = new_yaw_mode;
    }
    return;
}


void user_set_roll_pitch_mode(uint8_t new_roll_pitch_mode)
{
    //this is the function where you change the roll/pitch mode. call this function with the desired mode as a parameter
    //as long as there is a case for that mode in this function the mode will be changed.
    bool roll_pitch_initialised = false;
    
    switch(new_roll_pitch_mode)
    {
        case USER_LAND_ROLL_PITCH_STABLE:
            // here is where you can initilze variables for your roll/pitch mode mode
            //however do not actually put code here that doese somthing besides initilizeing variables
            // instead put code that controls the roll/pitch in use_update_roll_pitch_mode()
            
            //i changed nothing here because this mode is a copy of ROLL_PITCH_STABLE
            //and requires no variables to be initilized
            
             roll_pitch_initialised = true;
            break;
    }
    
    if( roll_pitch_initialised ) 
    {
        roll_pitch_mode = new_roll_pitch_mode;
    }
    return;
}

void user_set_throttle_mode(byte new_throttle_mode)
{
    //this is th fuction where you change the throttle mode. call this functions with the desired mode as a parameter
    //as long as there is a case for that mode in this function the mode will be changed.
    bool throttle_initialised = false;

    if( new_throttle_mode == throttle_mode ) {
        return;
    }
    
    switch(new_throttle_mode)
    {
        case USER_LAND_THROTTLE:
            // here is where you can initilze variable for your throttle mode
            //however do not actually put code here that doese somthing besides initilizeing variables
            // instead put code that controls the throttle in use_update_throttle_mode()
            
            //this is a coppy of THROTTLE_LAND and all code here was coppied from that throttle mode
           land_complete = false;   // mark landing as incomplete
            ground_detector = 0;          // A counter that goes up if our climb rate stalls out.
            landing_boost = 0 ;
         //   controller_desired_alt = current_loc.alt;   // reset controller desired altitude to current altitude
     //    target_WP = current_loc.alt;
     set_next_WP(&current_loc);
                 alt_change_flag = DESCENDING;
            // Set target altitude to LAND_START_ALT if we are high, below this altitude the get_throttle_rate_stabilized will take care of setting the next_WP.alt
            if (current_loc.alt>= next_WP.alt) {
                set_new_altitude(next_WP.alt);
                alt_change_flag = REACHED_ALT;
            }
            throttle_initialised = true;
            break;
        }
    if( throttle_initialised ) {
        throttle_mode = new_throttle_mode;
        int16_t desired_speed;
        // reset some variables used for logging
        desired_speed = 0;
        nav_throttle = 0;
    }
    return;
}

void user_update_yaw_mode()
{
    //this fuction is where the yaw mode is executed. or in other words if you made a new yaw mode put a case for it here
    //this is where any code that actuaally controls the yaw should go. you do not need to call this function
    //it should already be placed in the correct userloop.
    switch(yaw_mode)
    {
        case USER_LAND_YAW_HOLD:
            // here is whre you should write code for your yaw mode.
            //this functions gets called at a frequency of 100 Hz thats once every milisecond, keep that in mind when writing code here
            
            //this is a copy of what is done for YAW_HOLD
            //this function should keep the yaw stable
            get_yaw_rate_stabilized_ef(g.rc_4.control_in);
            break;
    }
    return;
}

void user_update_roll_pitch_mode()
{
// this is function is where your roll/pitch mode will be executed.be sure to put a case for your roll/pitch mode here
// this function will contorl your roll /pitch mode when it is active. this function does not need to be called it should be in the correct userloop already
    switch(roll_pitch_mode)
    {
        case USER_LAND_ROLL_PITCH_STABLE:
            //here is where you should write the code for your roll/pitch mode that will decide what the function will do
            //this function gets called a frequency of 100Hz thats once every milisecond, keep thta in mind when writing code here
            
            //this is a copy of what is done in ROLL_PITCH_STABLE mode
            // apply SIMPLE mode transform
            if(do_simple && new_radio_frame) {
                update_simple_mode();
            }

            control_roll            = g.rc_1.control_in;
            control_pitch           = g.rc_2.control_in;
            //this should keep the roll and pitch stable
            get_stabilize_roll(control_roll);
            get_stabilize_pitch(control_pitch);
            break;
    }
    return;
}

void user_update_throttle_mode()
{
    //this is the function that will update the throttle mode. any code that will control what the throttle does should go here.
    //this funcion does not need ot be called it should already be in the correct userloop
    switch(throttle_mode)
    {       
        case USER_LAND_THROTTLE:
            //here is where you should pu the code for your specific throttle mode. it willcontrol what the throttle does.
            //this function gets called at a frequency of 50 Hz thats once every 2 miliseconds, keep that in mind when writing code here.
            
            //this is a copy of THROTTLE_LAND
            //this makes the copter land
            throttle_mode = THROTTLE_AUTO;
            break;
    }
    return;
}

void userhook_init()
{
    // put your initialisation code here
    // this function gets called on startup by the copter.
    //anything that you feel need to happen on startup should be put in here.


}

void userhook_FastLoop()
{
    //put your 100Hz code here
    //put any fucnction or code here that you think should be called every milisecond
    
    user_read_control_switch();
    user_update_yaw_mode();
    user_update_roll_pitch_mode();  
    
}
void userhook_MediumLoop()
{
    //put your 10Hz code here
    // put any function or code here you think should be called every thenth of a second
    user_dancing_lights();
}

void userhook_SlowLoop()
{
    //put your 3 1/3Hz code here
    // put any function or code here you think should be called every three thenths of a second
    
}
void userhook_SuperSlowLoop()
{
    //put your 1Hz code here
    // put any function or code here you think should be called every second
    
    
}
void userhook_50Hz()
{
    // put your 50Hz code here
    // put any function or code here you think should be called every two miliseconds
    user_update_throttle_mode();
}

