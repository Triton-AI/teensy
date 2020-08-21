
/////// RC Reciever Channels ///////////
#include "PWM.hpp"
const int steeringPin = 7;
const int throttlePin = 8;
const int modePin = 9;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);
PWM modeRC(modePin);
int steeringAngle = 90;
int throttlePosition = 90;
bool driveMode = true;

int throttlePWM = 90;
int steeringPWM = 90;
bool reverse = false;

int maxRcRange = 2000;
int minRcRange = 1000;
int neutralRC = 1500;


//////////////////////////
#include <Servo.h> 

const int servoPin = 5;
Servo myServo;
const int escPin = 6;
Servo myESC;

//////////////////////////


const int pinA = 3;
const int pinB = 4;

void setup() {
  
throttleRC.begin(true); // PWM on pin 2 reading PWM HIGH duration
steeringRC.begin(true);
modeRC.begin(true);

////////////////////////////////////////
                                      //
myServo.attach(servoPin);
myServo.write(90);          // may not be necessary
myESC.attach(escPin);
myESC.write(90);            // may not be necessary
                                      //
////////////////////////////////////////

}

void loop() {
  
// Getting latest RC values
steeringAngle = steeringRC.getValue();
throttlePosition = throttleRC.getValue();
driveMode = modeRC.getValue(); 

if (throttlePosition > 0) {
    throttlePWM = map(throttlePosition,neutralRC,maxRcRange,90,180);
  }
  else{
    throttlePWM = map(throttlePosition,minRcRange,neutralRC,0,90);
  }

if (steeringAngle > 0) {
  steeringPWM = map(steeringAngle,neutralRC,maxRcRange,90,180);
}
else{
  steeringPWM = map(steeringAngle,minRcRange,neutralRC,0,90);
}


writeToESC(throttlePWM);

}


// Writes to Steering Servo
 void writeToServo(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   Serial.println(intVal);
   myServo.write(intVal);
  }

// Writes to ESC
 void writeToESC(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   Serial.println(intVal);
   myESC.write(intVal);
  }
