void getSteering() {
  steeringAngle = steeringRC.getValue();   
} // getSteering()

void getThrottle() {

  throttlePosition = throttleRC.getValue();
  
  // Trigger Deadzone
  if (abs(throttlePosition - neutralRC) < 10){
    throttlePosition = neutralRC;
  }
  
} // getThrottle()

void getDriveMode() {

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
  
 

  
