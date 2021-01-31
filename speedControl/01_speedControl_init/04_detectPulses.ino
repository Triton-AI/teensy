/*
 * Detect pulses
 *   * read in 3 phases of pulses
 *   * determine direction of rotation
 *   * calculate period of rotation per phase
 * 
 * 
  */
void detectAPulses() {
  if (digitalRead(pinB) == HIGH) { //Confirm if we are moving forward
     inReverse = false;
  }
  else { //Confirm if we are in reverse
      inReverse = true;
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
