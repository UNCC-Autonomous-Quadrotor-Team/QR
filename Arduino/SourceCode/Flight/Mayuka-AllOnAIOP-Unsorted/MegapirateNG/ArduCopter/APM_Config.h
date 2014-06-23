// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
#ifndef __ARDUCOPTER_APMCONFIG_H__
#define __ARDUCOPTER_APMCONFIG_H__ 
// Example config file. Take a look at config.h. Any term define there can be overridden by defining it here.

// Select your sensor board
#define PIRATES_SENSOR_BOARD PIRATES_CRIUS_AIO_PRO_V2
/*
	PIRATES_ALLINONE
	PIRATES_FFIMU
	PIRATES_FREEIMU
	PIRATES_BLACKVORTEX
	PIRATES_FREEIMU_4 					// New FreeIMU 0.4.1 with MPU6000, MS5611 and 5883L
	PIRATES_DROTEK_10DOF_MPU 		// MPU6000, MS5611 and 5883L
	PIRATES_CRIUS_AIO_PRO_V1    // Crius AllInOne Pro v1(1.1)
	PIRATES_CRIUS_AIO_PRO_V2    // Crius AllInOne Pro v2
*/

// RC configuration

// PPM_SUM(CPPM) Signal processing
#define SERIAL_PPM SERIAL_PPM_DISABLED
/*
	SERIAL_PPM_DISABLED
	SERIAL_PPM_ENABLED				// For all boards, PPM_SUM pin is A8
	SERIAL_PPM_ENABLED_PL1   // Use for CRIUS AIOP Pro v2,
*/

#define TX_CHANNEL_SET	TX_mwi
/*
	TX_set1							//Graupner/Spektrum												PITCH,YAW,THROTTLE,ROLL,AUX1,AUX2,CAMPITCH,CAMROLL
	TX_standard					//standard  PPM layout Robbe/Hitec/Sanwa	ROLL,PITCH,THROTTLE,YAW,MODE,AUX2,CAMPITCH,CAMROLL
	TX_set2							//some Hitec/Sanwa/others									PITCH,ROLL,THROTTLE,YAW,AUX1,AUX2,CAMPITCH,CAMROLL
	TX_mwi							//MultiWii layout													ROLL,THROTTLE,PITCH,YAW,AUX1,AUX2,CAMPITCH,CAMROLL
*/

// Select your baro sensor
#define CONFIG_BARO AP_BARO_MS5611_I2C
/*
	AP_BARO_BMP085_PIRATES
	AP_BARO_MS5611_I2C
*/

// Warning: COPTER_LEDS is not compatible with LED_SEQUENCER, so enable only one option
//#define COPTER_LEDS ENABLED     // New feature coming from ArduCopter
//#define LED_SEQUENCER ENABLED   // Old Oleg's LED Sequencer, see leds.pde for more info

#define MAX_SONAR_RANGE 400

// This OSD works on the Serial1 port
#define OSD_PROTOCOL OSD_PROTOCOL_NONE
/*
	OSD_PROTOCOL_NONE
	OSD_PROTOCOL_SYBERIAN
	OSD_PROTOCOL_REMZIBI  // Read more at: http://www.rcgroups.com/forums/showthread.php?t=921467
	OSD_PROTOCOL_FRSKY		// FrSky Telemetry protocol
*/

// For BlackVortex, just set PIRATES_SENSOR_BOARD as PIRATES_BLACKVORTEX, GPS will be selected automatically
#define GPS_PROTOCOL GPS_PROTOCOL_NONE
/*
	GPS_PROTOCOL_NONE 	without GPS
	GPS_PROTOCOL_NMEA
	GPS_PROTOCOL_SIRF
	GPS_PROTOCOL_UBLOX     <<< Select this for UBLOX LEA-6 (CRIUS GPS boards and others)
	GPS_PROTOCOL_MTK16
	GPS_PROTOCOL_BLACKVORTEX
	GPS_PROTOCOL_AUTO	auto select GPS
*/
	
#define SERIAL0_BAUD			 115200	// Console port 
#define SERIAL2_BAUD			 38400	// GPS port
#define SERIAL3_BAUD			 57600	// Telemetry (MAVLINK) port

// New in 2.0.43, but unused in MegairateNG
// MPNG: Piezo uses AN5 pin in ArduCopter, we uses AN5 for CLI switch
#define PIEZO	DISABLED	
#define PIEZO_LOW_VOLTAGE	DISABLED
#define PIEZO_ARMING		DISABLED

#define FRAME_CONFIG QUAD_FRAME
/*
	QUAD_FRAME
	TRI_FRAME
	HEXA_FRAME
	Y6_FRAME
	OCTA_FRAME
	OCTA_QUAD_FRAME
	HELI_FRAME
*/

#define FRAME_ORIENTATION X_FRAME
/*
	PLUS_FRAME
	X_FRAME
	V_FRAME
*/

# define CH7_OPTION		CH7_DO_NOTHING
/*
	CH7_DO_NOTHING
	CH7_SET_HOVER
	CH7_FLIP
	CH7_SIMPLE_MODE
	CH7_RTL
	CH7_AUTO_TRIM
	CH7_ADC_FILTER (experimental)
	CH7_SAVE_WP
 	CH7_MULTI_MODE
 */

//#define TOY_EDF	ENABLED
//#define TOY_MIXER TOY_LOOKUP_TABLE

//#define RATE_ROLL_I 	0.18
//#define RATE_PITCH_I	0.18
//#define MOTORS_JD880
//#define MOTORS_JD850


//this line is where you define the name of your flight mode and give it an integer value.(the actual value is meaningless it is just used as a label) 
//the value can be anything bigger than 12 and less then 4 bytes(4294967296)
//this name will be what the parameter will be when you call user_set_mode(), so you must make a case for it in user_set_mode()

#define USER_LAND 13



//this is where you can define a custom throttle mode and give it an integer value.(the actual value is meaningless it is just used as a label)
//throttle is the control of the output of the mottors and therfoerr controls vertical motion.
//the value can be anything bigger than 10 and less then 4 bytes(4294967296)
//it is also possible to use already defines throttle modes in your code and this line does not need ot be used
//it should do nothing if left alone but to be safe if your not going to use you might want to comment it out.


#define USER_LAND_THROTTLE 11


//this is where you can define the name of your custom yaw mode and give it an integer value.(the actual value is meaningless it is just used as a label)
//yaw is the control of the direction the copter faces. it can control spinning clockwise or counter clockwise of the copter body
//the value can be anything bigger than 10 and less then 4 bytes(4294967296)
//it is also possible to use already defines yaw modes in your code and this line does not need ot be used
//it should do nothing if left alone but to be safe if your not going to use you might want to comment it out.

#define USER_LAND_YAW_HOLD 10



//this is where you can define the name of your custom roll&pitch mode and give it an integer value.(the actual value is meaningless it is just used as a label)
//roll&pitch control the angle of the copter to the ground and therfoer control left and right motion(roll) as well as forward and backward motion(pitch)
//the value can be anything bigger than 10 and less then 4 bytes(4294967296)
//it is also possible to use already defines yaw modes in your code and this line does not need ot be used
//it should do nothing if left alone but to be safe if your not going to use you might want to comment it out.



#define USER_LAND_ROLL_PITCH_STABLE 10

//this is where you assign your custom flight mdoe to be activated when you flip the ch5 switch.
//NOTE: as of 6/14/13 you must assign your flight mode here and assign FLIGHT_MODE_6 in mission planner. 
//you can do this by connecting the arducopter to mission planner then clicking on the configuration tab on the top of the window
//then click advanced params on the left of the window. now under advanced params click on adv parameter list.
// scroll down until you see FLTMODE6 in the left collum. click on the number in the collum directly to the right of it 
//and assign it to the value you assigned your costum flight mode in this folder.
//click write params on the right of the window. thats it.




#define FLIGHT_MODE_6    USER_LAND
//again this stops a failsafe in set_mode() that only allows a certain number of modes.
//if you wissh to add more than one user defined mode at once then increment this number by one for each additional user mode
#define NUM_MODES 14

// agmatthews USERHOOKS
// the choice of function names is up to the user and does not have to match these
// uncomment these hooks and ensure there is a matching function un your "UserCode.pde" file
#define USERHOOK_FASTLOOP userhook_FastLoop();
#define USERHOOK_50HZLOOP userhook_50Hz();
#define USERHOOK_MEDIUMLOOP userhook_MediumLoop();
#define USERHOOK_SLOWLOOP userhook_SlowLoop();
#define USERHOOK_SUPERSLOWLOOP userhook_SuperSlowLoop();
#define USERHOOK_INIT userhook_init();

// the choice of includeed variables file (*.h) is up to the user and does not have to match this one
// Ensure the defined file exists and is in the arducopter directory
#define USERHOOK_VARIABLES "UserVariables.h"

//this stops a failsafe in set_mode() that doesnt let certain modes be activated without a gps connection
//so with this line we cna be in anymode without gps connection

//#define ALT_HOLD 10000"

// to enable, set to 1
// to disable, set to 0
//#define AUTO_THROTTLE_HOLD 1

#if PIRATES_SENSOR_BOARD == PIRATES_CRIUS_AIO_PRO_V2
	#define LOGGING_ENABLED		ENABLED
#else
	#define LOGGING_ENABLED		DISABLED
#endif

// Custom channel config - Expert Use Only.
// this for defining your own MOT_n to CH_n mapping.
// Overrides defaults (for APM1 or APM2) found in config_channels.h
// MOT_n variables are used by the Frame mixing code. You must define
// MOT_1 through MOT_m where m is the number of motors on your frame.
// CH_n variables are used for RC output. These can be CH_1 through CH_8,
// and CH_10 or CH_12. 
// Sample channel config. Must define all MOT_ channels used by
// your FRAME_TYPE.
// #define CONFIG_CHANNELS CHANNEL_CONFIG_CUSTOM
// #define MOT_1 CH_6
// #define MOT_2 CH_3
// #define MOT_3 CH_2
// #define MOT_4 CH_5
// #define MOT_5 CH_1
// #define MOT_6 CH_4
// #define MOT_7 CH_7
// #define MOT_8 CH_8


// ************** EXPERIMENTAL FEATURES *****************

// Alt hold with accelerometer
#define ACCEL_ALT_HOLD 0		// disabled by default, work in progress
#define ALT_HOLD 10000                  //this stops a failsafe in set_mode() that doesnt let certain modes be activated without a gps connection
                                        //so with this line we cna be in anymode without gps connection
#define INERTIAL_NAV DISABLED

#if INERTIAL_NAV == ENABLED
	#define ALT_HOLD_P			3
	#define ALT_HOLD_I			0
	#define ALT_HOLD_IMAX		300

	// RATE control
	#define THROTTLE_P			5		//
	#define THROTTLE_I			0.4		//
	#define THROTTLE_D			0.0		//

	#define LOITER_P			0.50
	#define LOITER_I			0.0
	#define LOITER_RATE_P		5		//
	#define LOITER_RATE_I		0.1		// Wind control
	#define LOITER_RATE_D		0.0		// try 2 or 3 for LOITER_RATE 1
#endif

// Enabling this will use the GPS lat/long coordinate to get the compass declination
//#define AUTOMATIC_DECLINATION ENABLED

//#define CLI_ENABLED DISABLED

//#define FAILSAFE

#endif //__ARDUCOPTER_APMCONFIG_H__


// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

// Example config file. Take a look at config.h. Any term define there can be overridden by defining it here.





