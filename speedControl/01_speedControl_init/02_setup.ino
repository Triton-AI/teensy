//only need next 2 lines if teensy version is greater than or equal to 4.0
#include "Watchdog_t4.h"  // found at https://github.com/tonton81/WDT_T4/  // download the zip, go to sketch, include library, ADD ZIP library..., find downloaded zip
WDT_T4<WDT1> wdt;
#include <millisDelay.h>
millisDelay heartbeat;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);

  Serial.begin(115200);
  pinMode(pinA, INPUT_PULLUP);   // configure the I.0 as a INPUTS
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), detectAPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  attachInterrupt(digitalPinToInterrupt(pinB), detectBPulses, RISING);    // enable external interrupt with a rising edge of PhaseB
  attachInterrupt(digitalPinToInterrupt(pinC), detectCPulses, RISING);    // enable external interrupt with a rising edge of PhaseC
  timeO = micros();   // Initialize initial time


  ////////////////////////////////////////
  //      RC Setup                      //
  throttleRC.begin(true); // PWM on pin 2 reading PWM HIGH duration
  steeringRC.begin(true);
  modeRC.begin(true);

  myServo.attach(servoPin);
  myServo.write(90);          // may not be necessary
  myESC.attach(escPin);
  //myESC.write(90);            // may not be necessary
                                        //
  ////////////////////////////////////////


  ////////////////////////////////////////
  //      RC Setup                      //
  //initialize the variables we're linked to
  g_Input = 0;
  g_Setpoint = 0;

  //turn the PID on
  speedPID.SetMode(AUTOMATIC);
                                        //
////////////////////////////////////////

// the following code should be placed at the end of setup() since the watchdog starts right after this


/// if(TEENSY_VERSION< 4.0) // uncomment next 7 lines
//  WDOG_UNLOCK = WDOG_UNLOCK_SEQ1;
//  WDOG_UNLOCK = WDOG_UNLOCK_SEQ2;
//  delayMicroseconds(1); // Need to wait a bit..
//  WDOG_STCTRLH = 0x0001; // Enable WDG
//  WDOG_TOVALL = watchdogTimeoutInMilliseconds; // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to. //
//  WDOG_TOVALH = 0;
//  WDOG_PRESC = 0; // This sets prescale clock so that the watchdog timer ticks at 1kHZ instead of the default 1kHZ/4 = 200 HZ
//////}////////

/// if(TEENSY_VERSION>= 4.0) // uncomment this section
  WDT_timings_t config;
  config.trigger = 1; /* in seconds, 0->128 */
  config.timeout = 1; /* in seconds, 0->128 */
  wdt.begin(config);
///}/////////////////////////


}
