
/////// Speed Controller ///////////
                                  //
const byte pin_a = 2;   //for encoder pulse A
const byte pin_b = 3;   //for encoder pulse B
const byte pin_c = 4;   //for encoder pulse C
int encoderA = 0;
int encoderB = 0;
int encoderC = 0;

unsigned long currentTime, previousTime;
double elapsedTime;

double a_speed, b_speed, c_speed;
int m_direction = 0;
double avg_speed = 0;
                                      //
////////////////////////////////////////
void setup() {


////////////////////////////////////////
                                      //
  pinMode(pin_a,INPUT_PULLUP);
  pinMode(pin_b,INPUT_PULLUP);
  pinMode(pin_c,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pin_a), detect_a, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_b), detect_b, RISING);
  attachInterrupt(digitalPinToInterrupt(pin_c), detect_c, RISING);

                                      //
////////////////////////////////////////

Serial.begin(9600);
Serial.println("<Arduino is ready>");     // For testing, omit for production
}

void loop() {
  currentTime = millis();                //get current time
  elapsedTime = (double)(currentTime - previousTime)*1000;        //compute time elapsed from previous computation

  
  a_speed = (encoderA)/elapsedTime;  //calculate motor speed, unit is ticks/s
  b_speed = (encoderB)/elapsedTime;  //calculate motor speed, unit is ticks/s
  c_speed = (encoderC)/elapsedTime;  //calculate motor speed, unit is ticks/s
    
  avg_speed = (a_speed + b_speed + c_speed)/3;  // Average of 3 encoder phase speeds

  
  //print out speed
if (Serial.available() <= 0) {
//  Serial.print("speed ");
//  Serial.println(avg_speed);         //Print speed (ticks/s) value

  Serial.print("A speed: ");
  Serial.print("\t");
  Serial.print(a_speed);
  Serial.print("\t");
 Serial.print("B speed: ");
  Serial.print("\t");
  Serial.print(b_speed);
  Serial.print("\t");
  Serial.print("C speed: ");
  Serial.print("\t");
  Serial.print(c_speed);
  Serial.print("\t");

 Serial.print("time");
 Serial.println(elapsedTime);
//  Serial.print("A ticks: ");
//  Serial.print("\t");
//  Serial.print(encoderA);
//  Serial.print("\t");
//  Serial.print("B ticks: ");
//  Serial.print("\t");
//  Serial.print(encoderB);
//  Serial.print("\t");
//  Serial.print("C ticks: ");
//  Serial.print("\t");
//  Serial.print(encoderC);
//  Serial.println("\t");
  }
  
  encoderA=0;
  encoderB=0;
  encoderC=0;
  previousTime = currentTime;                        //remember current time

}//////////////////////////// End of Loop ///////////////////////////////////

void detect_a() {
  encoderA+=1; //increasing encoder at new pulse
  m_direction = digitalRead(pin_b); //read direction of motor
  currentTime = micros();                //get current time
  aPeriod = (double)(currentTime - previousTime)/1000000;        //compute time elapsed from previous computation

  //a_speed = (encoderA)/elapsedTime;  //calculate motor speed, unit is ticks/s
  if(aPeriod != 0){
  a_speed = 1.0e6 / aPeriod;
  } else
  {
}

void detect_b() {
  encoderB+=1; //increasing encoder at new pulse
}

void detect_c() {
  encoderC+=1; //increasing encoder at new pulse
}
