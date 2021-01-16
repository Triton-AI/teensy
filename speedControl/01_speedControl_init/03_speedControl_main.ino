/*
 * Main loop
 *
 * Tasks:
 *   * Reset watchdog timer
 *   * Calculate current vehicle speed
 *   * Receive current driver inputs from RC controller
 *   * Switch betwen input sources for steering & throttle
 *   * Send commands to motors
 *
 */
void loop() {

 // the program is alive...for now.
  //wdt_reset();

 // Get latest from SBC
 recvWithEndMarker();
 runFunction();

 

///////////////// Calculating Vehicle Speed ////////////////////

 calcSpeed();

/////////////////// Getting Latest RC Values /////////////////////

 getSteering();
 getThrottle();
 getDriveMode();

/////////////////// Determining Drive Mode ///////////////////////

switch (g_driveModeEnum) {
  case rcDrive:
    pidControl(g_rcThrottle); // This maps g_rcThrottle to update g_throttlePWM & probably don't need to have pidControl***************************
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steeringPWM
    sendSpeed();   //<============================================================= do we need the argument for these 3 send functions? locations?
    sendThrottle();
    sendSteering();
    break;

  case roboDrive:
    pidControl(g_roboThrottle);  // This maps g_roboThrottle to update g_throttlePWM
    steeringControl(g_roboSteer); //This maps g_roboSteer to update g_steeringPWM
    sendSpeed();
    sendThrottle();
    sendSteering();
    break;

  case eStop:
    g_throttlePWM = g_neutralThrottle;
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steeringPWM

} // end switch case

///////////////////////Write to Motors ////////////////////////////
  writeToServo(g_neutralSteering);
  writeToESC(g_neutralThrottle);
  Serial.println(g_neutralSteering);
  Serial.println(g_neutralThrottle);
  delay(1000);
  Serial.println(g_driveModeEnum);
}
