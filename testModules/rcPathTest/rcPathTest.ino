/*    /// Test RC Path ///

Use this code to test your RC controller path.

With your RC receiver connected to PWM inputs on your 
teensy robocar board (pins 7,8, and 9) you should be able 
to demonstrate throttle, steering, and changing modes (Ch3)

*** Make sure your car is resting securely on a stand, wheels free to spin ****
*///
///


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


void setup() {
  

////////////////////////////////////////
//      RC Setup                      //
throttleRC.begin(true); // PWM on pin 2 reading PWM HIGH duration
steeringRC.begin(true);
modeRC.begin(true);

                                      
myServo.attach(servoPin);
//myServo.write(90);          // may not be necessary
myESC.attach(escPin);
//myESC.write(90);            // may not be necessary
                                      //
////////////////////////////////////////

}

void loop() {
  
// Getting latest RC values
steeringAngle = steeringRC.getValue();
throttlePosition = throttleRC.getValue();
driveMode = modeRC.getValue(); 

    Serial.print(steeringAngle);
    Serial.print("\t");
    Serial.print(throttlePosition);
    Serial.print("\t");
    Serial.println(driveMode); 



steeringPWM = map(steeringAngle,minRcRange,maxRcRange,fullRight,fullLeft);

if (driveMode >1700) {
  throttlePWM = map(throttlePosition,minRcRange,maxRcRange,wideOpenReverse,wideOpenThrottle);
  writeToESC(throttlePWM);
}
else {
  writeToESC(neutral);
}

writeToServo(steeringPWM);


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
