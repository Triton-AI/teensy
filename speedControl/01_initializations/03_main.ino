void loop() {
///////////////// Calculating Vehicle Speed ////////////////////

 //////////////////////////////////////////////////////////////
    // Measuring period
    timeO = timeF;
    timeF = micros();     // Geting the final time to get the speed 
    elapsedTime = timeF - timeO;
    if (timeF - tocA > 1e5) {
      omegaA = 0;
    }
    else{
      omegaA = (1e6/omegaAPeriod);
    }
    
    if (timeF - tocB > 1e5) {
      omegaB = 0;
    }
    else{
      omegaB = (1e6/omegaBPeriod);
    }
    
    if (timeF - tocC > 1e5) {
      omegaC = 0;
    }
    else{
      omegaC = (1e6/omegaCPeriod);
    }
    omegaAvg = (omegaA + omegaB + omegaC)/3;

    if (inReverse == true) {
        omegaA *= (-1);
        omegaB *= (-1);
        omegaC *= (-1);
        omegaAvg *= (-1);
    }

////// Printing Speed to Serial Monitor
//    Serial.print("Speed(omegaA): "); 
//    Serial.print(omegaA);     // Printing speed value
//    Serial.print(" ticks/s\t");
//    Serial.print("Speed(omegaB): "); 
//    Serial.print(omegaB);     // Printing speed value
//    Serial.print(" ticks/s\t");
//    Serial.print("Speed(omegaC): "); 
//    Serial.print(omegaC);     // Printing speed value
//    Serial.print(" ticks/s\t");
    //Serial.print("\nSpeed(omegaAvg): "); 
    Serial.print("\n");
    Serial.print(omegaAvg);     // Printing speed value
    //Serial.print(" ticks/s\t");

    avg_speed = omegaAvg;
    
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

/*
    Serial.print("\nSteering Angle: ");
    Serial.print(steeringAngle);
    Serial.print("\t");
    Serial.print("Throttle Position: ");
    Serial.print(throttlePosition);
    Serial.print("\t");
    Serial.print("Drive Mode: ");
    Serial.print(driveMode); 
*/
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
