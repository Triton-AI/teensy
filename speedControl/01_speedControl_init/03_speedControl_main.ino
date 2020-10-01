void loop() {
///////////////// Calculating Vehicle Speed ////////////////////

 calcSpeed();
    
///////////////// Getting Latest RC Values /////////////////////
   
  // shift array values
  for(i=steeringArraySize-1;i>0;i--)
    steeringArray[i] = steeringArray[i-1];
    
  steeringArray[0] = steeringRC.getValue();

  steeringSum = 0;

  for(i=0;i<steeringArraySize;i++){
    steeringSum += steeringArray[i];
  }

  steeringAngle = steeringSum/steeringArraySize;
 
  
   // shift array values
  for(i=throttleArraySize-1;i>0;i--)
    throttleArray[i] = throttleArray[i-1];
  throttleArray[0] = throttleRC.getValue();

  throttleSum = 0;

  for(i=0;i<throttleArraySize;i++){
    throttleSum += throttleArray[i];
  }
  throttlePosition = throttleSum/throttleArraySize;
  
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

  // Trigger Deadzone
  if (abs(throttlePosition - neutralRC) < 10){
    throttlePosition = neutralRC;
  }


steeringPWM = map(steeringAngle,minRcRange,maxRcRange,fullRight,fullLeft);
writeToServo(steeringPWM);
///////////////// Determining Drive Mode ///////////////////////

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
  writeToESC(throttlePWM);
}                                                             
