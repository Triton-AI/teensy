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
      if(strcmp(receivedChars, "calMaxForward") == 0) ;
      if(strcmp(receivedChars, "calMaxReverse") == 0) ;
      if(strcmp(receivedChars, "calNeutralThrottle") == 0) ;
      if(strcmp(receivedChars, "calMaxLeft") == 0) ;
      if(strcmp(receivedChars, "calMaxRight") == 0) ;
      if(strcmp(receivedChars, "calNeutralSteering") == 0) ;
      
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
  g_throttlePWM = neutral;
  
}


/////////////////// Polling /////////////////////////

void pollSpeed(){
  serial.print("speed_");
  serial.print(g_avgSpeed + "\n");
}


void pollThrottle(){
  serial.print("throttle_");
  serial.print(g_rcThrottle + "\n");
}


void pollSteering(){
  serial.print("steering_");
  serial.print(g_rcSteer + "\n");
}


void pollMode(){
  serial.print("mode_");
  serial.print(g_rcSteer + "\n");
}


void pollAll(){
  pollSteering();
  pollThrottle();
  pollMode();
  pollSpeed();
  
}


//////////////// Calibrating /////////////////////
