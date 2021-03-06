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
int timeMax = 0;

//void WatchdogReset(){ //this just resets the watchdog every time it is called. The 5 in the watchdogTimer is not the amount of time. If you want the amout of time look in 01_speedControl_init
//  static elapsedMillis watchdogTimer;  
// 
//  if (watchdogTimer > 5){ //The 5 in the watchdogTimer is not the amount of time. If you want the amout of time look in 01_speedControl_init
//    watchdogTimer = 0;
//    noInterrupts(); //kick the dog
//    WDOG_REFRESH = 0xA602;
//    WDOG_REFRESH = 0xB480;
//    interrupts();
//  }
//}


void loop() {
int startTime = micros();

//This resets the Teensy's internal watchdog. If the watchdog is not reset for longer than 3times the normal loop time it will set the mode to estop
//WatchdogReset();                        // lowlevel watchdog
wdt.feed();



// loopcount and heartbeat.remaining() are printed as test variables to check if the watchdogs are both working properly. To test the lowlevel watchdog, you can have the main loop delay for
// a given amount of time by sending "delay_X" over serial (where X is the delay time in seconds)  This function can be found in 08_serialAPI

//loopcount++;
//Serial.print("Loopcount: \t");
//Serial.print(loopcount);
//Serial.print("\t remaining heartbeat \t");
//Serial.print(heartbeat.remaining());
//Serial.print("\t driveMode \t");
//Serial.println(g_driveModeEnum);

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

bool inRange = 0;
switch (g_driveModeEnum) {
  case rcDrive:
    pidControl(g_rcThrottle); // This maps g_rcThrottle to update g_throttle & probably don't need to have pidControl***************************
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steering
    sendSpeed();   //<============================================================= do we need the argument for these 3 send functions? locations?
    sendThrottle();
    sendSteering();
    inRange = 1;
    break;

  case roboDrive:
    //heartbeat watchdog// resets the Teensy's hearbeet timer. If the heartbeat is not "detected" for longer than 3times the normal expected time to recieve a message it will set the mode to estop
    if(heartbeat.justFinished()){ // if the timer set in 08_serialAPI runs out, this function will be triggered
      Serial.print("Heartbeat Lost");
      g_driveModeEnum = eStop; // sets the drivemode to eStop;
      }
    digitalWrite(13,HIGH);
    digitalWrite(13,LOW);
    pidControl(g_roboThrottle);  // This maps g_roboThrottle to update g_throttle
    steeringControl(g_roboSteer); //This maps g_roboSteer to update g_steering
    sendSpeed();                 //print current speed
    sendThrottle();              //print throttle
    sendSteering();              //print steering
    inRange = 1;
    break;

  case eStop:
    g_throttle = g_neutralThrottle;
    steeringControl(g_rcSteer); //This maps g_rcSteer to update g_steering
    inRange = 1;

} // end switch case
if(inRange ==0){
  g_driveModeEnum = eStop;
  Serial.println("driveMode out of range, going to estop");
}
  

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
//delay(5000);
int timeElapse = micros() - startTime;
if(timeElapse> timeMax){
  timeMax = timeElapse;
  }
Serial.print(timeMax);
Serial.print("\t");

}
