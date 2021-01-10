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
   char endMarker = '\n';
   char splitMarker = '_';
   int splitFound = 0;
   char rc;
   
   while (Serial.available() > 0 && newData == false) {
     rc = Serial.read();
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
     }
   }
}


 void runFunction(){
   if (newData == true) {
      //Serial.print(receivedChars);
      float val = atof(receivedNums); // -1, 1; ,convert string to int or float

     //JTN sends commands to Teensy
      if(strcmp(receivedChars, "commandSpeed") == 0) commandSpeed((int)val);
      if(strcmp(receivedChars, "commandSteering") == 0) commandSteering((int)val);
      if(strcmp(receivedChars, "commandThrottle") == 0) commandThrottle((int)val); 
      if(strcmp(receivedChars, "commandShutdown") == 0) commandShutdown();
      
     //Teensy send data back to JTN?????????????????????????????????????? 
//      if(strcmp(receivedChars, "sendSpeed") == 0) sendSpeed();
//      if(strcmp(receivedChars, "sendThrottle") == 0)  sendThrottle();
//      if(strcmp(receivedChars, "sendSteering") == 0)  sendSteering();
//      if(strcmp(receivedChars, "sendMode") == 0)  sendMode();
//      if(strcmp(receivedChars, "sendAll") == 0)  sendAll();

     //Calibration from JTN
      if(strcmp(receivedChars, "calMaxForward") == 0)  calibration(&g_wideOpenThrottle, (int)val);
      if(strcmp(receivedChars, "calMaxReverse") == 0)  calibration(&g_neutralPWM, (int)val);
      if(strcmp(receivedChars, "calNeutralThrottle") == 0)  calibration(&g_neutralPWM, (int)val);
      if(strcmp(receivedChars, "calMaxLeft") == 0)  calibration(&g_fullLeft, (int)val);
      if(strcmp(receivedChars, "calMaxRight") == 0)  calibration(&g_fullRight, (int)val);
      if(strcmp(receivedChars, "calNeutralSteering") == 0)  calibration(&g_steeringPWM, (int)val);
      
   }
    newData = false;
 }

//////////////////// Commanding ////////////////////
 void commandSpeed(int intVal){ // Sets the desired speed from SBC
  //
  g_roboThrottle = intVal;
  
}


void commandSteering(int intVal){ // Sets the desired steering from SBC
  g_roboSteer = intVal;
}


void commandShutdown(){
  Serial.print("Shut down!");
  g_throttlePWM = g_neutralPWM;
  
}

int commandThrottle(float intVal){    //, int maxForward, int neutral, int g_wideOpenReverse){
  g_rcThrottle = intVal;
  
//  if (intVal == 0) //neutral throttle
//    return g_neutralPWM;
//
//  else if (intVal < 0) //negative throttle
//    return (int)((g_neutralPWM) + (g_neutralPWM - g_wideOpenReverse) * intVal);
//
//  else if (intVal > 0) //positive throttle
//    return (int)((g_neutralPWM) + (g_wideOpenThrottle - g_neutralPWM) * intVal);
    
}

/////////////////// Polling /////////////////////////

void sendSpeed(){
  Serial.print("speed_");
  Serial.print(String(g_avgSpeed) + "\n"); // - compile error, need to turn speed into string
}


void sendThrottle(){
  Serial.print("throttle_");
  Serial.print(String(g_rcThrottle) + "\n");
}


void sendSteering(){
  Serial.print("steering_");
  Serial.print(String(g_rcSteer) + "\n");
}


void sendMode(){
  Serial.print("mode_");
  Serial.print(String(g_rcSteer) + "\n");
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
//void calMaxForward(int *g_wideOpenThrottle, int newMaxThrottleForward){
//  *g_wideOpenThrottle = newMaxThrottleForward;
//  
//}
//void calMaxReverse(int *g_wideOpenReverse, int newMaxThrottleReverse){
//  *g_wideOpenReverse = newMaxThrottleReverse;
//}
//void calNeutralThrottle(int *g_neutralPWM, int newNeutralThrottle){
//  *g_neutralPWM = newNeutralThrottle;
//}
//void calMaxLeft(int *g_fullLeft, int newFullLeft){
//  *g_fullLeft = newFullLeft;
//}
//void calMaxRight(int *g_fullRight, int newFullRight){
//  *g_fullRight = newFullRight;
//}
//void calNeutralSteering(int *g_steeringPWM, int newNeutralSteering){
//  //*g_steeringPWM = newNeutralSteering;
//}
//
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