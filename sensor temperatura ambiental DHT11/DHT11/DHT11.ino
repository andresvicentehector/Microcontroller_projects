
#include <SimpleDHT.h>
int pinTH = 2;//declaración para usar el pin digital 2
SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
// read without samples.
  if (dht11.read(pinTH, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  Serial.print("la temperatura exterior es ");
  Serial.print((int)temperature); Serial.print("*C, "); 
  Serial.print("con un ");
  Serial.print((int)humidity); Serial.println("%");
  Serial.print("de humedad\n");
 
  delay(3000);
}
