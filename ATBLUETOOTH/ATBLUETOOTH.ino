#include <SoftwareSerial.h>
 
#define RxD 11
#define TxD 10
#define KEY 6
 
//SoftwareSerial BTSerial(RxD, TxD);
 
void setup()
{
pinMode(KEY, OUTPUT);
digitalWrite(KEY, HIGH);   // Como se mencionó en configuración colocar a KEY en estado alto.
  
delay(500);
  
Serial1.flush();
delay(500);
Serial1.begin(38400); // comunicación directa al Modulo.
Serial.begin(9600);        // comunicación directa al Monitor.
Serial.println("Enter AT commands:");
  
Serial1.print("AT\r\n");
delay(100);
  
}
  
void loop()
{
if (Serial1.available())
Serial.write(Serial1.read());
  
if (Serial.available())
Serial1.write(Serial.read());
}
