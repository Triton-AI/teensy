const int pinA = 2;
const int pinB = 3;
const int pinC = 4;

int valuePhaseB = 0;
int pulsePerMeter = 100;
volatile double pulses = 0;
double timeO = 0;
double timeF = 0;
double elapsedTime = 0;
double ticA = micros();
double tocA = 0;
double ticB = micros();
double tocB = 0;
double ticC = micros();
double tocC = 0;
double omegaAPeriod;
double omegaBPeriod;
double omegaCPeriod;
double omegaArray[4];
float omegaA = 0;
float omegaB = 0;
float omegaC = 0;
float omegaAvg = 0;
float encoderSpeed = 0;

bool reverse = false;
int arrayIndex = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(pinA, INPUT_PULLUP);   // configure the I.0 as a INPUTS
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), detectAPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  attachInterrupt(digitalPinToInterrupt(pinB), detectBPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  attachInterrupt(digitalPinToInterrupt(pinC), detectCPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  timeO = micros();   // Initialize initial time
}

void loop() {
    //////////////////////////////////////////////////////////////
    // Counting pulses
    timeF = micros();     // Geting the final time to get the speed 
    elapsedTime = timeF - timeO;
    
    //////////////////////////////////////////////////////////////
    // Measuring period
    if (timeF - tocA > 1e5) {
      omegaA = 0;
    }
    else{
      omegaA = (1e6/omegaAPeriod);
    }
    
    if (timeF - tocB > 1e5) {
      omegaB = 0;
    }
    else{
      omegaB = (1e6/omegaBPeriod);
    }
    
    if (timeF - tocC > 1e5) {
      omegaC = 0;
    }
    else{
      omegaC = (1e6/omegaCPeriod);
    }
    omegaAvg = (omegaA + omegaB + omegaC)/3;

    if (reverse == true) {
        omegaA *= (-1);
        omegaB *= (-1);
        omegaC *= (-1);
        omegaAvg *= (-1);
    }
    
    

    Serial.print("Speed(omegaA): "); 
    Serial.print(omegaA);     // Printing speed value
    Serial.print(" ticks/s\t");
    Serial.print("Speed(omegaB): "); 
    Serial.print(omegaB);     // Printing speed value
    Serial.print(" ticks/s\t");
    Serial.print("Speed(omegaC): "); 
    Serial.print(omegaC);     // Printing speed value
    Serial.print(" ticks/s\t");
    Serial.print("Speed(omegaAvg): "); 
    Serial.print(omegaAvg);     // Printing speed value
    Serial.println(" ticks/s");

    timeO = micros();    // Initialize initial time again
}

void detectAPulses() {
  if (digitalRead(pinB) == HIGH) { //Confirm if we are moving forward
     reverse = false;
  }
  else { //Confirm if we are in reverse
      reverse = true;
  }
  
  ticA = micros();
  omegaAPeriod = ticA - tocA;
  tocA = ticA;
}

void detectBPulses() {
  ticB = micros();
  omegaBPeriod = ticB - tocB;
  tocB = ticB;
}

void detectCPulses() {
  ticC = micros();
  omegaCPeriod = ticC - tocC;
  tocC = ticC;
}
