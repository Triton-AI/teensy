
///////////////// Getting Latest Steering Value /////////////////////

void getSteering() {
  g_rcSteer = steeringRC.getValue();   
} // getSteering()


///////////////// Getting Latest Throttle Value /////////////////////
void getThrottle() {

  g_rcThrottle = throttleRC.getValue();
  
  // Trigger Deadzone
  if (abs(g_rcThrottle - neutralRC) < 10){
    g_rcThrottle = neutralRC;
  }
  
} // getThrottle()


///////////////// Getting Latest Drive Mode /////////////////////
void getDriveMode() {
  int mode;
  mode = modeRC.getValue(); 
  
  if (mode > 1700) {
    g_driveModeEnum = rcDrive;
  }
  else if (mode > 1200) {
    
    g_driveModeEnum = roboDrive;
  }
  else {

    g_driveModeEnum = eStop;
    g_throttlePWM = neutral;
  }
  
}

/*
void smootheSteering() {
  // shift array values
  for(i=steeringArraySize-1;i>0;i--)
    steeringArray[i] = steeringArray[i-1];
    
  steeringArray[0] = 

  steeringSum = 0;

  for(i=0;i<steeringArraySize;i++){
    steeringSum += steeringArray[i];
  }

  steeringAngle = steeringSum/steeringArraySize;
} // smootheSteering()

void smootheThrottle() {
   // shift array values
  for(i=throttleArraySize-1;i>0;i--)
    throttleArray[i] = throttleArray[i-1];
  throttleArray[0] = 

  throttleSum = 0;

  for(i=0;i<throttleArraySize;i++){
    throttleSum += throttleArray[i];
  }
  throttlePosition = throttleSum/throttleArraySize;
  } // smootheThrottle()

  */
  
 

  
