//#include <i2cmaster.h>

#include <Wire.h>
#include "Adafruit_TMP007.h"
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <SimpleDHT.h>
#include <ADXL345.h>
//#include "BluetoothSerial.h"

ADXL345 acc;
const float alpha = 0.5;
double fXg = 0;
double fYg = 0; 
double fZg = 0;
int i;
double refXg = 0;
double refYg = 0;
double refZg = 0;

//BluetoothSerial SerialBT;

SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;
int pinTH = 2;//declaración para usar el pin digital 2


SoftwareSerial BTSerial(10, 11); // RX | TX//PRUEBA BLUETOOOTH

Adafruit_TMP007 tmpcd; /*objeto llamado tmpcd para el control del flanco central delantero
                       utilizando por defecto el puerto 0x40*/
Adafruit_TMP007 tmpct(0x41); /*objeto llamado tmpct para el control del flanco central trasero 
                               utilizando el puerto I2C 0x41*/
float cd = 0;
float ct = 0;    


Adafruit_MLX90614 tmpid = Adafruit_MLX90614();//5A
Adafruit_MLX90614 tmpdd = Adafruit_MLX90614(0x56);//0x56
Adafruit_MLX90614 tmpit = Adafruit_MLX90614(0x57);//0x57
Adafruit_MLX90614 tmpdt = Adafruit_MLX90614(0x58);//0x58

float id=0;
float dd=0;
float it=0;
float dt=0;

void setup() {
pinMode(9, OUTPUT);    
digitalWrite(9, LOW);   
Serial.begin(115200);
BTSerial.begin(9600);
BTSerial.flush();
//SerialBT.begin("ESP32telemetría");
 acc.begin();
 tmpid.begin();  
 tmpdd.begin();  
 tmpit.begin();  
 tmpdt.begin();
 tmpcd.begin(TMP007_CFG_4SAMPLE);
 tmpct.begin(TMP007_CFG_4SAMPLE);
 i=0 ;
 delay(500);
 // Wire.begin(21,22,100000);
//i2c_init();

}

void loop() {

  String mensaje = "";
  char pepe[]="a/r/d/u/i/N/O";
  //byte juan="4411/44444/7744/444";

  dht11.read(pinTH, &temperature, &humidity, NULL);
  Serial.print("la temperatura exterior es "); Serial.print((int)temperature); Serial.println("*C, "); 
  Serial.print("la humedad es del "); Serial.print((int)humidity); Serial.println("%"); 
   
  float cd = tmpcd.readObjTempC();
  float ct = tmpct.readObjTempC();
  Serial.print("Temperatura flanco central delantero: "); Serial.print(cd); Serial.println("*C");
  Serial.print("Temperatura flanco central trasero: "); Serial.print(ct); Serial.println("*C");

  id=tmpid.readObjectTempC();
  dd=tmpdd.readObjectTempC();
  it=tmpit.readObjectTempC();
  dt=tmpdt.readObjectTempC();
  Serial.print("Temperatura flanco izquierdo delantero: "); Serial.print(id); Serial.println("*C");
 Serial.print("Temperatura flanco derecho delantero: "); Serial.print(dd); Serial.println("*C");
  Serial.print("Temperatura flanco izquierdo trasero: "); Serial.print(it); Serial.println("*C");
  Serial.print("Temperatura flanco derecho trasero: "); Serial.print(dt); Serial.println("*C");


  double pitch, roll, Xg, Yg, Zg;
  acc.read(&Xg, &Yg, &Zg);

   if (i == 0)
  {
    refXg = Xg; refYg = Yg; refZg = Zg;
    i = 1;
  }

  Xg = Xg - refXg;
  Yg = Yg - refYg;
  Zg = Zg - refZg + 1;

  //Low Pass Filter
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));

  //Roll & Pitch Equations
  roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;

  Serial.print("Pitch: "); Serial.println(pitch);
  Serial.print("Roll: ");Serial.println(roll);

 mensaje += id;
    mensaje +="|";
 mensaje += cd;
   mensaje += "|";
 mensaje += dd;
   mensaje += "|";
 mensaje += it;
     mensaje += "|";
 mensaje += ct;
    mensaje += "|";
 mensaje += dt;
  mensaje += "|";
 mensaje += temperature;
    mensaje += "|";
 mensaje += humidity;
    mensaje += "|";   
  mensaje += pitch;
    mensaje += "|";
 mensaje += roll;
    mensaje += "|";  

//Serial.println(mensaje);
Serial.println("");
//BTSerial.print(mensaje);
BTSerial.print("3/4/5/6/7");
//SerialBT.write(3);
delay(500);

}
