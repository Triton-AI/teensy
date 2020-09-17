
// to use this upload it to the Teensy and then open the serial port (ctl+Shift+M) and then the serial monitor will come up.
//Write into the serial port what speed you want the car to go.
//This code uses the servo library so 1000 is full negative  1500 is neutral and 2000 is full speed forward.



const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
char receivedNums[numChars];
boolean newData = false;



////////////////////////////////////////
                                      //
#include <Servo.h>                                      
int pwmPin = 5;
Servo myservo;
                                      //
////////////////////////////////////////





void setup() {

////////////////////////////////////////
                                      //
myservo.attach(pwmPin);
                                      //
////////////////////////////////////////
 
 Serial.begin(9600);
// Serial.println("<Arduino is ready>");
}

void loop() {
 recvWithEndMarker();
 runFunction();
}

void recvWithEndMarker() {
   static byte ndx = 0;
   char endMarker = '\n';
   char splitMarker = '_';
   int splitFound = 0;
   char rc;
   
   while (Serial.available() > 0 && newData == false) {
     rc = Serial.read();
     if(rc == splitMarker){
          receivedChars[ndx] = '\0'; // terminate the string
          ndx = 0;
          splitFound = 1; 
       }
     else if (rc != endMarker) {
       if (splitFound == 0){
          receivedChars[ndx] = rc;
       }
       else{
          receivedNums[ndx] = rc;
       }       
       ndx++;
       if (ndx >= numChars) {
         ndx = numChars - 1;
       }
     }
     else {
        if(splitFound == 0){
            receivedChars[ndx] = '\0'; // terminate the string 
        }
        else{
            receivedNums[ndx] = '\0'; // terminate the string
        }
        ndx = 0;
        newData = true;
     }
   }
}




 void runFunction(){
   if (newData == true) {
//      Serial.print(receivedChars);
      int intVal = atoi(receivedNums);
      if(strcmp(receivedChars,"commandSpeed") == 0) commandSpeed(intVal);
      if(strcmp(receivedChars, "commandSteering") == 0) commandSteering(intVal);
      if(strcmp(receivedChars, "commandShutdown") == 0) commandShutdown();
      if(strcmp(receivedChars, "pollSpeed") == 0) pollSpeed();
      if(strcmp(receivedChars, "pollThrottle") == 0)  pollThrottle();
      if(strcmp(receivedChars, "pollSteering") == 0)  pollSteering();
      if(strcmp(receivedChars, "pollMode") == 0)  pollMode();
      if(strcmp(receivedChars, "pollAll") == 0)  pollAll();
   }
    newData = false;
 }


 void commandSpeed(int intVal){
  Serial.print("Setting Speed to ");
  Serial.println(intVal);
  myservo.write(intVal);
}


void commandSteering(int intVal){
  Serial.print("Setting Steering to ");
  Serial.println(intVal);
  myservo.write(intVal);
}


void commandShutdown(){
  
}


void pollSpeed(){
  
}


void pollThrottle(){
  
}


void pollSteering(){
  
}


void pollMode(){
  
}


void pollAll(){
  
}



  
