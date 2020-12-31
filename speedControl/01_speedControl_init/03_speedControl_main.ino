/*
 * Main loop 
 * 
 * Tasks:
 *   * Reset watchdog timer
 *   * Calculate current vehicle speed
 *   * Receive current driver inputs from RC controller
 *   * Seitch betwen input sources for steering & throttle
 *   * Send commands to motors
 * 
 * 
 */
void loop() {

 // the program is alive...for now. 
  wdt_reset();

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
    
  case eStop:
    g_throttlePWM = g_neutralPWM;
    steeringControl(g_rcSteer);

} // end switch case

///////////////////////Write to Motors ////////////////////////////
  writeToServo(g_steeringPWM);
  writeToESC(g_throttlePWM);
}                                                             
