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

int loopcount = 0;

void WatchdogReset(){ //this just resets the watchdog every time it is called. The 5 in the watchdogTimer is not the amount of time. If you want the amout of time look in setup
  static elapsedMillis watchdogTimer;  
 
  if (watchdogTimer > 5){
    watchdogTimer = 0;
    
    noInterrupts();
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
    interrupts();
  }
}


void loop() {

WatchdogReset();
if(heartbeat.justFinished()){
  Serial.print("Heartbeat Lost");
  g_driveModeEnum = eStop;
  }

loopcount++;
Serial.print("Loopcount: \t");
Serial.print(loopcount);
Serial.print("\t remaining heartbeat \t");
Serial.println(heartbeat.remaining());


// if you don't refresh the watchdog timer before it runs out, the system will be rebooted

// Get latest from SBC
 recvWithEndMarker(); //receiving commands from SBC
 runFunction(); //process the command

///////////////// Calculating Vehicle Speed ////////////////////

 calcSpeed();

/////////////////// Getting Latest RC Values /////////////////////

 getSteering();
 getThrottle();
 //getDriveMode();

/////////////////// Determining Drive Mode ///////////////////////

switch (g_driveModeEnum) {
  case rcDrive:
    pidControl(g_rcThrottle); // This maps g_rcThrottle to update g_throttle & probably don't need to have pidControl***************************
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steering
    sendSpeed();   //<============================================================= do we need the argument for these 3 send functions? locations?
    sendThrottle();
    sendSteering();
    break;

  case roboDrive:
    pidControl(g_roboThrottle);  // This maps g_roboThrottle to update g_throttle
    steeringControl(g_roboSteer); //This maps g_roboSteer to update g_steering
    sendSpeed();                 //print current speed
    sendThrottle();              //print throttle
    sendSteering();              //print steering
    break;

  case eStop:
    g_throttle = g_neutralThrottle;
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steering

} // end switch case

///////////////////////Write to Motors ////////////////////////////
  writeToServo(g_steering);
  writeToESC(g_throttle);//(g_neutralThrottle);


//  Serial.print("steering: ");
//  Serial.println(g_steering);
//  Serial.print("throttle: ");
//  Serial.println(g_throttle);
//  Serial.print("Drivemode: ");
//  Serial.println(g_driveModeEnum);
//  Serial.print("===============\n");
//  delay(5000);
  
}
