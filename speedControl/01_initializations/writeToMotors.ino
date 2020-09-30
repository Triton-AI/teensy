// Writes to Steering Servo
 void writeToServo(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   //Serial.println(intVal);
   myServo.write(intVal);
  }

// Writes to ESC
 void writeToESC(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   //Serial.println(intVal);
   myESC.write(intVal);
  }
