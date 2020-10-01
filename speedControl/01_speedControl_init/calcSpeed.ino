void calcSpeed() {
 //////////////////////////////////////////////////////////////
    // Measuring period
    timeO = timeF;
    timeF = micros();     // Geting the final time to get the speed 
    elapsedTime = timeF - timeO;
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

  //////////////////////////////////////////////////////////////
    // Averaging period
    omegaAvg = (omegaA + omegaB + omegaC)/3;

    if (inReverse == true) {
        omegaA *= (-1);
        omegaB *= (-1);
        omegaC *= (-1);
        omegaAvg *= (-1);
    }

    avg_speed = omegaAvg;

}
