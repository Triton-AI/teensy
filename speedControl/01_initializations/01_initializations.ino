/////// RC Reciever Initializations ///////////
#include "PWM.hpp"                          ///

int maxRcRange = 2000;
int minRcRange = 1000;
int neutralRC = 1500;

const int steeringPin = 7;
const int throttlePin = 8;
const int modePin = 9;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);
PWM modeRC(modePin);
const int steeringArraySize = 50;
int steeringArray[steeringArraySize]; // for smoothing
int steeringSum = 0;
int steeringAngle = neutralRC;
int throttlePosition = neutralRC;
const int throttleArraySize = 5;
int throttleArray[throttleArraySize];   // for smoothing
int throttleSum = 0;
int driveMode = 1;

int throttlePWM = 90; // Initializing neutral throttle
int wideOpenThrottle = 180;
int neutral = 90;
int wideOpenReverse = 0;

int steeringPWM = 90; // Initializing neutral steering
int fullRight = 0;
int fullLeft = 180;



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
double avg_speed = 0;
bool inReverse = false;


///// Speed Controller Initializations ////////
                                            ///

double set_speed = 0;
double max_Speed = 1000; // ticks/s
double min_Speed = max_Speed * (-1); // ticks/s

const int errorArraySize = 100;
double errorArray[errorArraySize]; //error of speed = set_speed - avg_speed (made an array for smoothing purposes
double error = 0;
double lastError = 0;  //last error of speed
double cumError = 0;   //sum error of speed
double rateError = 0;  //rate of change of error

///////////////////////
float kp = .5;
float ki = 10;
float kd = 0.01;
///////////////////////

double effortPID = 0;
double maxEffort = max_Speed;     // previously defined in ticks/s
double minEffort = min_Speed;     // previously defined in ticks/s

double currentTime, previousTime;

byte i = 0;                       // Smoothing index
//double elapsedTime;

///
