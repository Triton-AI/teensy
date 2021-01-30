#include "PWM.hpp"  // https://github.com/xkam1x/Arduino-PWM-Reader                   
#include <PID_v1.h> // https://github.com/br3ttb/Arduino-PID-Library
//#include <KalmanFilter.h>

//KalmanFilter kalmanfilter;
/////// RC Reciever Initializations ///////////
                                            ///
//PWM values from RC controller
int g_maxRcRange = 2000;
int g_minRcRange = 1000;
int g_neutralRC = 1500;

enum driveMode {
  rcDrive,
  roboDrive,
  eStop
};

const int steeringPin = 7;
const int throttlePin = 8;
const int modePin = 9;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);
PWM modeRC(modePin);

//human drive mode
int g_rcSteer = g_neutralRC;        // gets set by RC receiver [1000,2000]
int g_rcThrottle = g_neutralRC;     // gets set by RC receiver [1000,2000]
//auto drive mode
int g_roboSteer = g_neutralRC;      // gets set by serial interface [1000,2000]
int g_roboThrottle = g_neutralRC;   // gets set by serial interface [1000,2000]

driveMode g_driveModeEnum = roboDrive; //eStop; // gets set by RC receiver

int g_throttle = 90; // Initializing neutral throttle
int g_wideOpenThrottle = 180;
int g_neutralThrottle= 90;
int g_wideOpenReverse = 0;

int g_steering = 90; // Initializing neutral steering // ***************************We need this steeringNeutral instead of just having center value. NEED TO TALK Ian *****************************
int g_fullRight = 0;
int g_fullLeft = 180;
                                            ///
////// End RC Reciever Initializations ////////


/////// Servo & ESC Initializations ///////////
                                            ///
#include <Servo.h>

const int servoPin = 5;
Servo myServo;
const int escPin = 6;
Servo myESC;
                                            ///
/////// End Servo & ESC Initializations ///////

//////// Speedometer Initializations //////////
                                            ///
const int pinA = 2;
const int pinB = 3;
const int pinC = 4;

int valuePhaseB = 0;
volatile double pulses = 0;
double timeO = 0;
double timeF = 0;
double elapsedTime = 0;
double ticA = micros();
double tocA = 0;
double ticB = micros();
double tocB = 0;
double ticC = micros();
double tocC = 0;

double omegaAPeriod;
double omegaBPeriod;
double omegaCPeriod;
double omegaArray[4];
float omegaA = 0;
float omegaB = 0;
float omegaC = 0;
float omegaAvg = 0;
float encoderSpeed = 0;
double g_avgSpeed = 0;
bool inReverse = false;


////////////////Kalman_Filter//////////////////
float Q_angle = 0.001, Q_gyro = 0.005;
float R_angle = 0.5 , C_0 = 1;
float timeChange = 5; //Filter sampling time interval (unit:milliseconds)
float dt = timeChange * 0.001; //Note:The value of dt is the filter sampling time
////////////////Kalman_Filter//////////////////

///// Speed Controller Initializations ////////
                                            ///
//////////// PID Gains ///////////
const float kp = .5;
const float ki = 10;
const float kd = 0.01;
//////////////////////////////////
//Define Variables we'll be connecting to
double g_Setpoint, g_Input, g_Output;

//Specify the links and initial tuning parameters
PID speedPID(&g_Input, &g_Output, &g_Setpoint,kp,ki,kd, DIRECT);


double set_speed = 0;
double g_max_Speed = 1000; // ticks/s
double g_min_Speed = g_max_Speed * (-1); // ticks/s

const int errorArraySize = 100;
double errorArray[errorArraySize]; //error of speed = set_speed - avg_speed (made an array for smoothing purposes
double error = 0;
double lastError = 0;  //last error of speed
double cumError = 0;   //sum error of speed
double rateError = 0;  //rate of change of error



double effortPID = 0;
double g_maxEffort = g_max_Speed;     // previously defined in ticks/s
double g_minEffort = g_min_Speed;     // previously defined in ticks/s

double currentTime, previousTime;

byte i = 0;                       // Smoothing index

/////////////// Serial API ///////////
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
char receivedNums[numChars];
boolean newData = false;
//////////////////////////////////////


//////////////////WATCHDOG ////////////

#define watchdogTimeoutInMilliseconds 4000 ///4000 is equal to 4 seconds

#define heartbeatTimeoutInMilliseconds 7000


//double elapsedTime;

///
