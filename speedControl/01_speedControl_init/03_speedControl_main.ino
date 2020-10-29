
void loop() {

 // the program is alive...for now. 
  wdt_reset();

 // Get latest from SBC
 recvWithEndMarker();
 runFunction();

///////////////// Calculating Vehicle Speed ////////////////////

 calcSpeed();
    
///////////////// Getting Latest RC Values /////////////////////

 getSteering();
 getThrottle();
 getDriveMode();

///////////////// Determining Drive Mode ///////////////////////

switch (g_driveModeEnum) {
  case rcDrive:
    pidControl(g_rcThrottle);
    steeringControl(g_rcSteer);
    break;
    
  case roboDrive:
    pidControl(g_roboThrottle);
    steeringControl(g_roboSteer);
    break;
    
  default:
    g_throttlePWM = neutral;
    steeringControl(g_rcSteer);

} // end switch case

///////////////////////Write to Motors ////////////////////////////
  writeToServo(g_steeringPWM);
  writeToESC(g_throttlePWM);
}                                                             
