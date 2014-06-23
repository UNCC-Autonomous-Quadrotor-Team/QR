// agmatthews USERHOOKS
// user defined variables

// example variables used in Wii camera testing - replace with your own variables
#if WII_CAMERA == 1
WiiCamera               ircam;
int                     WiiRange=0;
int                     WiiRotation=0;
int                     WiiDisplacementX=0;
int                     WiiDisplacementY=0;
#endif


#if SHOW_LEDS == 1
static boolean   fast_leds = false;                     // do we need 50hz leds or is 10hz sufficient (to save cpu cycles)
static int led_counter = 0 ;                            // this global variable is used for the motorled flash rate timer
#endif
#if RELAY_LEDS == 1            // global variables for motor leds and battery monitoring through onboard relay
static int rl_state   = 0 ;    // this global variable is used to save the on/off state of the relay
static int rl_counter = 0 ;    // this global variable is used for the relay switch rate timer
#endif