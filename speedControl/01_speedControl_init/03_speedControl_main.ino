
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


///////////////// Determining Drive Mode ///////////////////////


///////////////////////Write to Motors ////////////////////////////
  writeToServo(steeringPWM);
  writeToESC(throttlePWM);
}                                                             
