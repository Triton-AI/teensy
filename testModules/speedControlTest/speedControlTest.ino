
#include "PWM.hpp"
const int steeringPin = 7;
const int throttlePin = 8;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);

const int pinA = 3;
const int pinB = 4;
int valuePhaseB = 0;
double tic = micros();
double toc = 0;
double omegaPeriod;
float omega = 0;

///
double avg_speed = 0;
double set_speed = 0;


double error = 0;; //error of speed = set_speed - avg_speed
double lastError = 0;  //last error of speed
double cumError = 0;   //sum error of speed
double rateError = 0;  //rate of change of error

///////////////////////
float kp = 0;
float ki = 0;
float kd = 0;
///////////////////////

double effort = 0;
double maxEffort = 5000;
double minEffort = -5000;

double currentTime, previousTime;
double elapsedTime;

///
int throttlePWM = 0;
bool reverse = false;


#include <Servo.h> 
const int escPin = 6;
Servo myESC;

void setup() {
Serial.begin(9600); 
  pinMode(pinA, INPUT_PULLUP);   // configure the I.0 as a INPUTS
  pinMode(pinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), detectPulses, RISING);    // enable external interrupt with a rising edge of PhaseA

}

void loop() {
// Measuring period
  currentTime = micros(); 
  elapsedTime = currentTime - previousTime;
  
  if (currentTime - toc > 1e5) {
    omega = 0;
    cumError = 0;    // avoid winding up integrator error
  }
  else{
    omega = (1e6/omegaPeriod);
  }
  Serial.print("Speed(omega): "); 
  Serial.print(omega);     // Printing speed value
  Serial.println(" ticks/s");

  avg_speed = omega;

  //PID program
  if (set_speed != 0){

    error = set_speed - avg_speed;                          // determine error
    cumError += (error * elapsedTime);                // compute integral
    rateError = (error - lastError)/elapsedTime;   // compute derivative
    lastError = error;
    
    effort = error*kp + cumError*ki + rateError*kd;
    if (effort > maxEffort) {
      effort = maxEffort;
    }
    else if (effort < minEffort) {
      effort = minEffort;
    }
    lastError = error;  //save last (previous) error
 
  }
  else{
    error = 0;
    lastError = 0;
    cumError = 0;
    effort = 0;
  }
 
  if (effort > 0) {
    throttlePWM = map(effort,0,maxEffort,90,180);
  }
  else{
    throttlePWM = map(effort,minEffort,0,0,90);
  }
  
  writeToESC(throttlePWM);

  lastError = error;                                //remember current error
  previousTime = currentTime;                        //remember current time
}


void detectPulses() {
   if (digitalRead(pinB) == HIGH) { //Confirm if we are moving forward
     reverse = false;
  }
  else { //Confirm if we are in reverse
      reverse = true;
  }
  
  tic = micros();
  omegaPeriod = tic - toc;
  toc = tic;
}

// Writes to ESC
 void writeToESC(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   Serial.println(intVal);
   if (intVal > 180) {
    intVal = 180;
   }
   else if (intVal < 0){
    intVal = 0;
   }
   myESC.write(intVal);
  }
  
