/*
  Motor - PID speed control
  (1) Receive command from serial (via COM4): set_speed, kP, kI, kD
  (2) Control motor speed through PWM (PWM is base on PID calculation)
 
 */

/////// RC Reciever Channels ///////////
#include "PWM.hpp"
const int steeringPin = 7;
const int throttlePin = 8;
const int modePin = 9;
PWM steeringRC(steeringPin);
PWM throttleRC(throttlePin);
PWM modeRC(modePin);

////////////////////////////////////////
//        Includes                    //
#include <Servo.h>                                      
const int servoPin = 5;
Servo myServo;

const int escPin = 6;
Servo myESC;
                                      //
////////////////////////////////////////



/////// Serial Interface ///////////
String inputSt = "";
char myChar;
boolean stringComplete = false;  // whether the string is complete
boolean motor_start = false;


/////// Speed Controller ///////////
const byte pin_a = 2;   //for encoder pulse A
const byte pin_b = 3;   //for encoder pulse B
const byte pin_c = 4;   //for encoder pulse C
int encoderA = 0;
int encoderB = 0;
int encoderC = 0;
double a_speed, b_speed, c_speed;
int m_direction = 0;
double avg_speed = 0;
double set_speed = 0;
double set_steer = 0; 
double e_speed = 0; //error of speed = set_speed - avg_speed
double e_speed_pre = 0;  //last error of speed
double e_speed_sum = 0;  //sum error of speed
double pwm_pulse = 0;     //this value is 0~255
double kp = 0;
double ki = 0;
double kd = 0;
const int interruptPeriod = 0.01;  // Period of interrupt service routine, unit is in seconds
int timer1_counter; //for timer
int i=0;
unsigned long currentTime, previousTime;
double elapsedTime;

////////////////////////////// Begin Setup ////////////////////////////////
void setup() {

throttleRC.begin(true); // PWM on pin 2 reading PWM HIGH duration
steeringRC.begin(true);
modeRC.begin(true);

////////////////////////////////////////
                                      //
myServo.attach(servoPin);
myServo.write(0);          // may not be necessary
myESC.attach(escPin);
myESC.write(0);            // may not be necessary
                                      //
////////////////////////////////////////
  pinMode(pin_a,INPUT_PULLUP);
  pinMode(pin_b,INPUT_PULLUP);
  pinMode(pin_c,INPUT_PULLUP);
//  pinMode(pin_fwd,OUTPUT);
//  pinMode(pin_bwd,OUTPUT);
//  pinMode(pin_pwm,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a), detect_a, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_b), detect_b, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_c), detect_c, RISING);
  // start serial port at 9600 bps:
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");     // For testing, omit for production
  //--------------------------timer setup
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
 
  // preload timer 65536-16MHz/256/2Hz (34286 for 0.5sec) (59286 for 0.1sec)
  switch (interruptPeriod) {
    case 0.01:
      timer1_counter = 65911;  // 66536 - (65911) = 625*.0625*256 = 0.01s 
    case 0.1:
      timer1_counter = 60286; // 66536 - (60286) = 6250*.0625*256 = 0.1s
      break;
    case 0.5:
      timer1_counter = 34286; // 65536 - (34286) = 31250 *.0625*256 = 0.5s
      break;
    default:
      timer1_counter = 59286; // 0.1s
  }
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  //--------------------------timer setup
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  analogWrite(pin_pwm,0);   //stop motor
//  digitalWrite(pin_fwd,0);  //stop motor
//  digitalWrite(pin_bwd,0);  //stop motor
} ////////////////////////////// End Setup ////////////////////////////////

void loop() {

// Getting latest RC values
steering.getValue();
throttle.getValue();
mode.getValue(); 
 
  if (stringComplete) {
    // clear the string when COM receiving is completed
    inputSt = "";  //note: in code below, inputSt will not become blank, inputSt is blank until '\n' is received
    stringComplete = false;
  }

  //      **********     receive serial command     **********   
  if (inputSt.substring(0,5) == "start"){
//    digitalWrite(pin_fwd,1);      //run motor run forward
//    digitalWrite(pin_bwd,0);
    motor_start = true;
  }
  if (inputSt.substring(0,4) == "stop"){
//    digitalWrite(pin_fwd,0);
//    digitalWrite(pin_bwd,0);      //stop motor
    motor_start = false;
  }
  if (inputSt.substring(0,10) == "poll_speed"){
    // JTN: “poll_speed\n”
    // Teensy: “speed_12345\n” (int value, ticks/s)
    Serial.println("speed_" + string(avg_speed) + "\n" );
  }
  if (inputSt.substring(0,13) == "poll_throttle"){
    // JTN: “poll_throttle\n”
    // Teensy: “throttle_256\n” (PWM value)
    
    Serial.println("throttle_" + string(throttlePWM) + "\n" );
  }
  if (inputSt.substring(0,13) == "poll_steering"){
    // JTN: “poll_steering\n”
    // Teensy: “steering_563\n” (PWM value)
    Serial.println("steering_" + string(steeringPWM) + "\n" );
  }
    
  if (inputSt.substring(0,13) == "command_speed"){
    set_speed = inputSt.substring(13,inputSt.length()).toFloat();  //get string after command_speed
  }
  if (inputSt.substring(0,16) == "command_steering"){
    set_steer = inputSt.substring(16,inputSt.length()).toFloat();  //get string after command_steering
  }
  if (inputSt.substring(0,16) == "command_shutdown"){
    //shutdown command
  }
  if (inputSt.substring(0,6) == "set_kp"){
    kp = inputSt.substring(6,inputSt.length()).toFloat(); //get string after set_kp
  }
  if (inputSt.substring(0,6) == "set_ki"){
    ki = inputSt.substring(6,inputSt.length()).toFloat(); //get string after set_ki
  }
  if (inputSt.substring(0,6) == "set_kd"){
    kd = inputSt.substring(6,inputSt.length()).toFloat(); //get string after set_kd
  }

writeToESC(throttlePWM);
writeToServo(steeringPWM);     
}
    
void detect_a() {
  encoderA+=1; //increasing encoder at new pulse
  m_direction = digitalRead(pin_b); //read direction of motor
}

void detect_b() {
  encoderB+=1; //increasing encoder at new pulse
}

void detect_c() {
  encoderC+=1; //increasing encoder at new pulse
}
ISR(TIMER1_OVF_vect)        // interrupt service routine - tick every 0.1sec
{
  currentTime = micros();                //get current time
  elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
  TCNT1 = timer1_counter;   // set timer
  a_speed = (encoderA)/elapsedTime;  //calculate motor speed, unit is ticks/micros
  b_speed = (encoderB)/elapsedTime;  //calculate motor speed, unit is ticks/micros
  c_speed = (encoderC)/elapsedTime;  //calculate motor speed, unit is ticks/micros
  
  avg_speed = (a_speed + b_speed + c_speed)/3;  // Average of 3 encoder phase speeds
  
  encoderA=0;
  encoderB=0;
  encoderC=0;
  //print out speed
//  if (Serial.available() <= 0) {
//    Serial.print("speed");
//    Serial.println(avg_speed);         //Print speed (ticks/s) value
//    }


  //PID program
  if (motor_start){
    e_speed = set_speed - avg_speed;

    error = set_speed - avg_speed;                          // determine error
    cumError += error * elapsedTime;                // compute integral
    rateError = (error - lastError)/elapsedTime;   // compute derivative
    
    pwm_pulse = e_speed*kp + e_speed_sum*ki + (e_speed - e_speed_pre)/interruptPeriod*kd;
    e_speed_pre = e_speed;  //save last (previous) error
    e_speed_sum += e_speed * interruptPeriod; // cumulative error
    if (e_speed_sum >maxErrRange) e_speed_sum = maxErrRange;
    if (e_speed_sum <minErrRange) e_speed_sum = minErrRange;
  }
  else{
    e_speed = 0;
    e_speed_pre = 0;
    e_speed_sum = 0;
    pwm_pulse = 0;
  }
  
  //update new speed
  if (pwm_pulse <255 & pwm_pulse >0){
    writeToESC(pwm_pulse);  //set motor speed  
  }
  else{
    if (pwm_pulse>180){
      analogWrite(pin_pwm,255);
    }
    else{
      analogWrite(pin_pwm,0);
    }
  }

  lastError = error;                                //remember current error
  previousTime = currentTime;                        //remember current time
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar != '\n') {
      inputSt += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

// Writes to Steering Servo
 void writeToServo(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   Serial.println(intVal);
   myservo.write(intVal);
  }

// Writes to ESC
 void writeToESC(int intVal){
  // Serial.print("This just in ... ");
   //int intVal = atoi(receivedChars);
   Serial.println(intVal);
   myESC.write(intVal);
  }
  
