
void loop() {

 // the program is alive...for now. 
  wdt_reset();

  
///////////////// Calculating Vehicle Speed ////////////////////

 calcSpeed();
    
///////////////// Getting Latest RC Values /////////////////////

 getSteering();
 getThrottle();
 getDriveMode();

////////////////////// Map Steering ////////////////////////////
steeringPWM = map(steeringAngle,minRcRange,maxRcRange,fullRight,fullLeft);

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

///////////////////////Write to Motors ////////////////////////////
  writeToServo(steeringPWM);
  writeToESC(throttlePWM);
}                                                             
