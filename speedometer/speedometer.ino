const int pinA = 3;
const int pinB = 4;
int valuePhaseB = 0;
int pulsePerMeter = 100;
volatile long pulses = 0;
unsigned long timeO = 0;
unsigned long timeF = 0;
float encoderSpeed = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(pinA, INPUT_PULLUP);   // configure the I.0 as a INPUTS
  pinMode(pinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), detectPulses, RISING);    // enable external interrupt with a rising edge of PhaseA
  timeO = micros();   // Initialize initial time
}

void loop() {
  if (Serial.available() && Serial.read() == 'p') {
    timeF = micros();     // Geting the final time to get the speed 
    encoderSpeed = ((float)pulses/pulsePerMeter) / ((float)(timeF - timeO) / (1000000.0));   // Meters/sec
    Serial.print("Speed: "); 
    Serial.print(encoderSpeed);     // Printing speed value
    Serial.println(" m/s");
    pulses = 0;    // Initialize pulses to count again
    timeO = micros();    // Initialize initial time again
   }
}

void detectPulses() {
  if (digitalRead(pinB) == LOW) { //Confirm if we are turning on CW direction.
    pulses++;    // Adding pulses
  }
  else { //Confirm if we are turning on CCW direction.
    pulses--;    // Substraing pulses
  }
}
