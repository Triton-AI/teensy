
///////////////// Getting Latest Steering Value /////////////////////

void getSteering() {
  steeringAngle = steeringRC.getValue();   
  steeringPWM = map(steeringAngle,minRcRange,maxRcRange,fullRight,fullLeft);
} // getSteering()


///////////////// Getting Latest Throttle Value /////////////////////
void getThrottle() {

  throttlePosition = throttleRC.getValue();
  
  // Trigger Deadzone
  if (abs(throttlePosition - neutralRC) < 10){
    throttlePosition = neutralRC;
  }
  
} // getThrottle()


///////////////// Getting Latest Drive Mode /////////////////////
void getDriveMode() {

 driveMode = modeRC.getValue(); 
  
  if (driveMode > 1700) {
    throttlePWM = map(throttlePosition,minRcRange,maxRcRange,wideOpenReverse,wideOpenThrottle);
  }
  else if (driveMode > 1200) {
    
    set_speed = map(throttlePosition,minRcRange,maxRcRange,min_Speed,max_Speed);
    effortPID = pidSpeedControl(set_speed,avg_speed);
    //Serial.print("\tIn Speed Control Mode - set speed: "); 
    //Serial.println(set_speed);
    throttlePWM = map(effortPID,minEffort,maxEffort,wideOpenReverse,wideOpenThrottle);
  }
  else {
    throttlePWM = neutral;
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
  
 

  
