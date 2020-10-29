
void setup() {
  Serial.begin(9600); 
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
myESC.write(90);            // may not be necessary
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


wdt_enable(WDTO_250MS); // Enable low level watchdog timer

}
