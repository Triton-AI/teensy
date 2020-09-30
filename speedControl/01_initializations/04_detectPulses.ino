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
