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
//
// getSteering();
// getThrottle();
// getDriveMode();
//
/////////////////// Determining Drive Mode ///////////////////////

switch (g_driveModeEnum) {
  case rcDrive:
    pidControl(g_rcThrottle);
    steeringControl(g_rcSteer);
    sendSpeed();   //<============================================================= do we need the argument for these 3 send functions? locations?
    sendThrottle();
    sendSteering();
    break;

  case roboDrive:
    pidControl(g_roboThrottle);
    steeringControl(g_roboSteer);
    sendSpeed();
    sendThrottle();
    sendSteering();
    break;

  case eStop:
    g_throttlePWM = g_neutralPWM;
    steeringControl(g_rcSteer);

} // end switch case

///////////////////////Write to Motors ////////////////////////////
  writeToServo(g_steeringPWM);
  writeToESC(g_throttlePWM);
  Serial.println(g_steeringPWM);
  Serial.println(g_throttlePWM);
  delay(1000);
}
