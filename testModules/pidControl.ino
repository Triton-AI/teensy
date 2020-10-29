
int pidSpeedControl(int set_Speed, int real_Speed) {
  currentTime = micros();
  double pidPeriod = (currentTime - previousTime)/1e6;

  if (pidPeriod > 0.1) {
    cumError = 0;           // Avoiding winding up integrator
  }

  // shift error values
  for(i=errorArraySize-1;i>0;i--){
    errorArray[i] = errorArray[i-1];
  }

  int effort = neutral;
  int errorSum = 0;
  if (abs(set_Speed) > 0){

    errorArray[0] = set_Speed - real_Speed;                 // determine error
    for(i=0;i<errorArraySize;i++){
      errorSum += errorArray[i];
    }
    error = errorSum/errorArraySize;

    cumError += (error * pidPeriod);                // compute integral
    rateError = (error - lastError)/pidPeriod;   // compute derivative
    lastError = error;  //save last (previous) error

    effort = error*kp + cumError*ki + rateError*kd;
    //effort = error*kp;
    if (effort > maxEffort) {
      effort = maxEffort;
    }
    else if (effort < minEffort) {
      effort = minEffort;
    }


  }
  else{
    error = 0;
    lastError = 0;
    cumError = 0;
    effort = 0;
  }

    //Serial.print("\tError: ");
    Serial.print("\t");
    Serial.print(error);
    //Serial.print("\tControl Effort: ");
    Serial.print("\t");
    Serial.print(effort);


  lastError = error;                                //remember current error
  previousTime = currentTime;                       //remember current time
  return effort;
}
