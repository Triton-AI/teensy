

byte SerialTXBuffer[5];
byte SerialRXBuffer[5];


void setup()  
{

 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

WriteRegister(9,PWMOutput);  // Set motor speed

}




/* WriteRegister():
Writes a single byte to the PIC16F1829, 
"Value" to the register pointed at by "Index".  
Returns the response 
*/
byte WriteRegister(byte Index, byte Value)
{
byte i = 0;
byte checksum = 0;
byte ack = 0;

SerialTXBuffer[0] = 210;
SerialTXBuffer[1] = 1;
SerialTXBuffer[2] = 3;
SerialTXBuffer[3] = Index;
SerialTXBuffer[4] = Value;

for (i=0;i<6;i++)
  {
  if (i!=5)
    {
    mySerial.write(SerialTXBuffer[i]);
    checksum += SerialTXBuffer[i];    
    }
  else
    mySerial.write(checksum);     
  }
  delay(5);

  if (mySerial.available())
    ack = mySerial.read();

  return ack;
}
