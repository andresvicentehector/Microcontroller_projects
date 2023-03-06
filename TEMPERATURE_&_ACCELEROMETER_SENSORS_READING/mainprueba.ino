#include <SparkFun_ADXL345.h>
#include <TinyGPS++.h>
//#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
//#include <LiquidCrystal.h>
#include <SimpleDHT.h>
#include <Adafruit_TMP007.h>
#include <Wire.h>
//#include <SPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
int i=0;
#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);

int pinTH = 2;//pin sensor de temperatura ambiental
SoftwareSerial BTSerial(10,11); // RX-TX definición puerto serie para el bluetooth
SoftwareSerial GPSSerial(3,4); // RX-TX definición puerto serie para el GPS
TinyGPSPlus gps; //objeto gps
SimpleDHT11 dht11; //objeto llamada dh11
ADXL345 acelerometro = ADXL345();

Adafruit_TMP007 tmpcd;
Adafruit_TMP007 tmpct(0x41);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);//escucha por el puerto serie a 9600 baudios
 BTSerial.begin(9600);//escucha puerto serie para el bluetooth a 9600 baudios
 //GPSSerial.begin(9600);
// display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
// display.display();


 acelerometro.powerOn();                     //encendemos acelerómetro
 acelerometro.setRangeSetting(16);    // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity
  
}

void loop() {

int Xg,Yg,Zg,refXg,refYg,refZg;
refXg=0;
refYg=0;
refZg=0 ;  

double roll,pitch;

// first loop to 
if (i == 0) 
{
refXg = Xg; refYg = Yg; refZg = Zg;
i = 1;
}

// subtract the zero point
Xg = Xg - refXg;
Yg = Yg - refYg;
Zg = Zg - refZg + 1;

acelerometro.readAccel(&Xg, &Yg, &Zg); 

roll  = -(atan2(-Yg, Zg) * 180.0) / M_PI; 
pitch = (atan2(Xg, sqrt(Yg * Yg + Zg * Zg)) * 180.0) / M_PI;


//Escucha GPS
while(GPSSerial.available())
{
gps.encode(GPSSerial.read());
}

 
  Serial.println("=================================");
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinTH, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  Serial.print("la temperatura de la cueva es: ");
  Serial.print((int)temperature); Serial.print("*C, "); 
  Serial.print("con un ");
  Serial.print((int)humidity); Serial.println("%");
  Serial.print("de humedad\n");
  Serial.println("=================================");
 
  if(gps.location.isUpdated()){
  Serial.print("latitud:"); Serial.println(gps.location.lat(), 6); // Latitude in degrees (double)
  Serial.print("longitud:");Serial.println(gps.location.lng(), 6); // Longitude in degrees (double)
  Serial.print("velocidad en km/h:");Serial.println(gps.speed.kmph(),5);
  Serial.print("altura en metros:");Serial.println(gps.altitude.meters()); // Altitude in meters (double)
  }
  
  Serial.println("=================================");
  float cd = tmpcd.readObjTempC();
  Serial.print("temperatura centro rueda delantera: "); Serial.print(cd); Serial.println("*C");
  float ecd = tmpcd.readDieTempC();
  Serial.print("temp exterior: "); Serial.print(ecd); Serial.println("*C");
  
  float ct = tmpct.readObjTempC();
  Serial.print("temperatura centro rueda trasera: "); Serial.print(ct); Serial.println("*C");
  float ect = tmpct.readDieTempC();
  Serial.print("temperatura exterior: "); Serial.print(ect); Serial.println("*C");  

  
 Serial.println("=================================");
 Serial.print(Xg);
 Serial.print(", ");
 Serial.print(Yg);
 Serial.print(", ");
 Serial.print(Zg);
 Serial.print(", ");
 Serial.print(roll);
 Serial.print(", ");
 Serial.print(pitch);
 Serial.print("\n");
 
  delay(4000); // 4 seconds per reading for 16 samples per reading






}
