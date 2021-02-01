/*
 * recvWithEndMarker()
 *  * Read serial stream, using a \0 to indicate breaks in strings
 * 
 * runFunction()
 *  *  Maps input commands to functions
 * 
 * 
 */
void recvWithEndMarker() {
   static byte ndx = 0;
   char endMarker = END_MARKER;
   char splitMarker = SPLIT_MARKER;
   int splitFound = 0;
   char rc;

   // This function activates whenever a message is sent to arduino over the serial. 
   //Each message is processed such that the endMarker marks the end of a message (ARDUINO IDE SERIAL MONITER assumes this will be the new line char "\n" so that should be used if using Arduino Serial Moniter
   // if the incoming character rc is equal to the splitMarker, splitFound will be set to 1 which will send all remaining chars to the recievedNums[] array 
   //everythin befroe will be sent to the receivedChars[] array
   // for example "myVarName_##\n" will change receivedChars[] to be "myVarName" and receivedNums[] to be "##" (where ## is any length number that can be converted from string to float with atof(##)
   
   while (Serial.available() > 0 && newData == false) {
     g_driveModeEnum = roboDrive; // If the SBC talks to the arduino, we assume that it is going to be controlling the Arduino
     heartbeat.start(heartbeatTimeoutInMilliseconds); // This  restarts the heartbeat watchdog timer whenever a message is recieved from the serial
     rc = Serial.read(); // rc stands for recieved char because Serial.read() only reads one char at a time
     //Serial.print("inside recv");
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
        Serial.println(receivedChars);
        Serial.println(receivedNums);
     }
   }
}


 void runFunction(){
   if (newData == true) {
      //Serial.print(receivedChars);
      float val = atof(receivedNums); // -1, 1; ,convert string to int or float

     //JTN sends commands to Teensy during robomode
      if(strcmp(receivedChars, "commandSpeed") == 0) g_roboThrottle = (int)val;
      if(strcmp(receivedChars, "commandSteering") == 0) g_roboSteer = (int)val;
      if(strcmp(receivedChars, "commandThrottle") == 0) g_roboThrottle = (int)val; 
      if(strcmp(receivedChars, "commandShutdown") == 0) g_driveModeEnum = eStop;
      
     //Teensy send data back to JTN?????????????????????????????????????? 
//      if(strcmp(receivedChars, "sendSpeed") == 0) sendSpeed();
//      if(strcmp(receivedChars, "sendThrottle") == 0)  sendThrottle();
//      if(strcmp(receivedChars, "sendSteering") == 0)  sendSteering();
//      if(strcmp(receivedChars, "sendMode") == 0)  sendMode();
//      if(strcmp(receivedChars, "sendAll") == 0)  sendAll();

     //Calibration from JTN
      if(strcmp(receivedChars, "calMaxForward") == 0)  g_wideOpenThrottle = (int)val;
      if(strcmp(receivedChars, "calMaxReverse") == 0)  g_wideOpenReverse = (int)val;
      if(strcmp(receivedChars, "calNeutralThrottle") == 0)  g_neutralThrottle = (int)val;
      if(strcmp(receivedChars, "calMaxLeft") == 0)  g_fullLeft = (int)val;
      if(strcmp(receivedChars, "calMaxRight") == 0)  g_fullRight = (int)val;
      if(strcmp(receivedChars, "calNeutralSteering") == 0)  g_steering = (int)val;
      //Commands from JTN
      if(strcmp(receivedChars, "delay") == 0)  delay(val*1000); // dealys for val seconds; for example if "delay_2" is recieved over serial, the teensy will delay for 2 seconds
      if(strcmp(receivedChars, "driveMode")==0) g_driveModeEnum = (int)val;
   }
    newData = false;
 }

/////////////////// Sending /////////////////////////
//Teensy send info to SBC

void sendSpeed(){
  Serial.print("speed_");
  Serial.print(String(g_avgSpeed) + "\n"); // - compile error, need to turn speed into string
}


void sendThrottle(){
  Serial.print("throttle_");
  Serial.print(String(g_throttle) + "\n");
//  Serial.print(String(g_rcThrottle) + "\n");
}


void sendSteering(){
  Serial.print("steering_");
  Serial.print(String(g_steering) + "\n");
//  Serial.print(String(g_rcSteer) + "\n");
}


void sendMode(){
  Serial.print("mode_");
  Serial.print(String(g_driveModeEnum) + "\n");
}


void sendAll(){
  sendSteering();
  sendThrottle();
  sendMode();
  sendSpeed();
  
}

//////////////// Calibrating /////////////////////
//when teensy receives pwm values from JTN, UPDATE the present value.
/*
JTN: “calMaxForward_400\n”
JTN: “calMaxReverse_300\n”
JTN: “calNeutralThrottle_360\n”
JTN: “calMaxLeft_300\n”
JTN: “calMaxRight_500\n”
JTN: “calNeutralSteering_400\n”
*/

void calibration(int *toCalibrate, int val){
  Serial.print("calibrating");
  *toCalibrate = val;
  Serial.print(*toCalibrate);
}

//Try PWMs (called during user calibration process)
/*  JTN: “tryThrottle_400\n”
    JTN: “trySteering_350\n
*/
//void tryThrottle_400(){
//    
//}
