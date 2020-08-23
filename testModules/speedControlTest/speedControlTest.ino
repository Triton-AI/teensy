
/////// RC Reciever Initializations ///////////
#include "PWM.hpp"                          ///
const int steeringPin = 7;
const int throttlePin = 8;
const int modePin = 9;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);
PWM modeRC(modePin);
int steeringAngle = 90;
int throttlePosition = 90;
int driveMode = 1;

int throttlePWM = 90; // Initializing neutral throttle
int wideOpenThrottle = 180;
int neutral = 90;
int wideOpenReverse = 0;

int steeringPWM = 90; // Initializing neutral steering
int fullRight = 0;
int fullLeft = 180;


int maxRcRange = 2000;
int minRcRange = 1000;
int neutralRC = 1500;
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
double max_Speed = 20; // ticks/s
double min_Speed = -1*max_Speed; // ticks/s

double error = 0;; //error of speed = set_speed - avg_speed
double lastError = 0;  //last error of speed
double cumError = 0;   //sum error of speed
double rateError = 0;  //rate of change of error

///////////////////////
float kp = 0.01;
float ki = 0.01;
float kd = 0;
///////////////////////

double effortPID = 0;
double maxEffort = max_Speed;     // previously defined in ticks/s
double minEffort = min_Speed;     // previously defined in ticks/s

double currentTime, previousTime;
//double elapsedTime;

///



void setup() {
  Serial.begin(9600); 
  pinMode(pinA, INPUT_PULLUP);   // configure the I.0 as a INPUTS
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), detectAPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  attachInterrupt(digitalPinToInterrupt(pinB), detectBPulses, RISING);    // enable external interrupt with a rising edge of PhaseB
  attachInterrupt(digitalPinToInterrupt(pinC), detectCPulses, RISING);    // enable external interrupt with a rising edge of PhaseC
  timeO = micros();   // Initialize initial time


////////////////////////////////////////
//      RC Setup                      //
throttleRC.begin(true); // PWM on pin 2 reading PWM HIGH duration
steeringRC.begin(true);
modeRC.begin(true);

                                      
myServo.attach(servoPin);
myServo.write(90);          // may not be necessary
myESC.attach(escPin);
myESC.write(90);            // may not be necessary
                                      //
////////////////////////////////////////
}

void loop() {
///////////////// Calculating Vehicle Speed ////////////////////

 //////////////////////////////////////////////////////////////
    // Measuring period
    timeO = timeF;
    timeF = micros();     // Geting the final time to get the speed 
    elapsedTime = timeF - timeO;
    if (timeF - tocA > 1e5) {
      omegaA = 0;
    }
    else{
      omegaA = (1e6/omegaAPeriod);
    }
    
    if (timeF - tocB > 1e5) {
      omegaB = 0;
    }
    else{
      omegaB = (1e6/omegaBPeriod);
    }
    
    if (timeF - tocC > 1e5) {
      omegaC = 0;
    }
    else{
      omegaC = (1e6/omegaCPeriod);
    }
    omegaAvg = (omegaA + omegaB + omegaC)/3;

    if (inReverse == true) {
        omegaA *= (-1);
        omegaB *= (-1);
        omegaC *= (-1);
        omegaAvg *= (-1);
    }

////// Printing Speed to Serial Monitor
//    Serial.print("Speed(omegaA): "); 
//    Serial.print(omegaA);     // Printing speed value
//    Serial.print(" ticks/s\t");
//    Serial.print("Speed(omegaB): "); 
//    Serial.print(omegaB);     // Printing speed value
//    Serial.print(" ticks/s\t");
//    Serial.print("Speed(omegaC): "); 
//    Serial.print(omegaC);     // Printing speed value
//    Serial.print(" ticks/s\t");
    Serial.print("\tSpeed(omegaAvg): "); 
    Serial.print(omegaAvg);     // Printing speed value
    Serial.print(" ticks/s\t");

    avg_speed = omegaAvg;

    


    
///////////////// Getting Latest RC Values /////////////////////

  steeringAngle = steeringRC.getValue();
  throttlePosition = throttleRC.getValue();
  driveMode = modeRC.getValue(); 

  // Checking to see if RC values within range
  if (steeringAngle >> maxRcRange) {
    steeringAngle = neutralRC;
    throttlePosition = neutralRC;
    driveMode = neutralRC;
  }
  else if (steeringAngle << minRcRange) {
    steeringAngle = neutralRC;
    throttlePosition = neutralRC;
    driveMode = neutralRC;
  }


    Serial.print("\nSteering Angle: ");
    Serial.print(steeringAngle);
    Serial.print("\t");
    Serial.print("Throttle Position: ");
    Serial.print(throttlePosition);
    Serial.print("\t");
    Serial.print("Drive Mode: ");
    Serial.print(driveMode); 

steeringPWM = map(steeringAngle,minRcRange,maxRcRange,fullRight,fullLeft);
writeToServo(steeringPWM);
///////////////// Determining Drive Mode ///////////////////////

  if (driveMode >1700) {
    throttlePWM = map(throttlePosition,minRcRange,maxRcRange,wideOpenReverse,wideOpenThrottle);
    writeToESC(throttlePWM);
  }
  else if (driveMode > 1200) {
    
    set_speed = map(throttlePosition,minRcRange,maxRcRange,min_Speed,max_Speed);
    effortPID = pidSpeedControl(set_speed,avg_speed);
    throttlePWM = map(effortPID,minEffort,maxEffort,wideOpenReverse,wideOpenThrottle);
    writeToESC(throttlePWM);
  }
  else {
    writeToESC(neutral);
  }
}                                                                                                ///
/////////////////////////////////////// End of Main Loop ///////////////////////////////////////////

void detectAPulses() {
  if (digitalRead(pinB) == HIGH) { //Confirm if we are moving forward
     inReverse = false;
  }
  else { //Confirm if we are in reverse
      inReverse = true;
  }
  
  ticA = micros();
  omegaAPeriod = ticA - tocA;
  tocA = ticA;
}

void detectBPulses() {
  ticB = micros();
  omegaBPeriod = ticB - tocB;
  tocB = ticB;
}

void detectCPulses() {
  ticC = micros();
  omegaCPeriod = ticC - tocC;
  tocC = ticC;
}


// Writes to Steering Servo
 void writeToServo(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   //Serial.println(intVal);
   myServo.write(intVal);
  }

// Writes to ESC
 void writeToESC(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   //Serial.println(intVal);
   myESC.write(intVal);
  }


  //PID program

int pidSpeedControl(int set_Speed, int real_Speed) {
  Serial.print("\tIn Speed Control Mode - set speed: "); 
  Serial.print(set_Speed);
  currentTime = micros();
  double pidPeriod = currentTime - previousTime;

  if (pidPeriod > 5e5) {
    cumError = 0;           // Avoiding winding up integrator
  }
  
  int effort = neutral;
  if (set_Speed != 0){

    error = set_Speed - real_Speed;                          // determine error
    cumError += (error * pidPeriod);                // compute integral
    rateError = (error - lastError)/pidPeriod;   // compute derivative
    lastError = error;  //save last (previous) error
    
    effort = error*kp + cumError*ki + rateError*kd;
    if (effort > maxEffort) {
      effort = maxEffort;
    }
    else if (effort < minEffort) {
      effort = minEffort;
    }
    
    
    Serial.print("\tControl Effort: ");
    Serial.print(effort);
    
 
  }
  else{
    error = 0;
    lastError = 0;
    cumError = 0;
    effort = 0;
  }
  
    Serial.print("\tSensed Speed: ");
    Serial.print(real_Speed);
  

  lastError = error;                                //remember current error
  previousTime = currentTime;                       //remember current time
  return effort;
}
