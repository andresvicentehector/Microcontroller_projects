#include <SoftwareSerial.h>
#include <Wire.h>
//

#include <ADXL345.h>
//
#include <Adafruit_Sensor.h>
#include "Adafruit_TMP006.h"
//

/****************************************
    Variables for ACELEROMETER
 ***************************************/
const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;

double refXg = 0;
double refYg = 0;
double refZg = 0;

/****************************************
    Variables for ADXL345 Functions
 ***************************************/
int i = 0;
ADXL345 acc;


/****************************************
    Variables for Bluetooth Functions
 ***************************************/
SoftwareSerial BTSerial(10, 11); // RX | TX


/****************************************
    Variables for External Temperature (LM35) - Optional
 ***************************************/
int TempExtPin = A1;                // select the input pin for the ExternalTemperature LM35
float celsius = 0, farhenheit = 0;  // temperature variables
float millivolts;                   //dichiarazione di variabile tensione (float è per i numeri con la virgola)
int sensor;

/****************************************
    Variables for TMP006  Functions
 ***************************************/
Adafruit_TMP006 tmp006;


void setup()
{
  pinMode(9, OUTPUT);                   // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(9600);

  // Bluetooth Inizialize
  BTSerial.begin(9600);                 // HC-05 default speed in AT command more
  Serial.println("Setup HC-05 End");

  delay(3000);

  // ADXL345 Inizialize
  acc.begin();
  delay(1000);
  Serial.println("Setup ACC End");


  i = 0;

}

void loop()
{

displayInfo();

}


void displayInfo()
{
  double distanceKm = 0.0;
  double distanceMeters = 0.0;

  /* ARRAY DEFINITION:

     0  - START
     1  - Latitude
     2  - N (Nord)
     3  - Longitude
     4  - E (East)
     5  - month
     6  - day
     7  - year
     8  - hh:MM.ss
     9  - speed (Km/h)
     10  - altitude (m)
     11 - satellites (number of satellites)
     12 - hdop (number of satellites in use)
     13 - roll
     14 - pitch
     15 - Xg
     16 - Yg
     17 - Zg
     18 - Audio (Optional-currently disable)
     19 - Distance (in metri)
     20 - Temperature (Esterna tramite LM35)
     21 - Temperature Tyre (Temperatura gomma posteriore tramite TMP006)
     22 - Ammortizzatore Anteriore
     23 - Ammortizzatore Posteriore
  */

  String strMessage = "";

  // ADXL sensor
  double pitch, roll, Xg, Yg, Zg;
  acc.read(&Xg, &Yg, &Zg);

  // Calibration ADXL345
  if (i == 0)
  {
    refXg = Xg; refYg = Yg; refZg = Zg;
    i = 1;
  }

  Xg = Xg - refXg;
  Yg = Yg - refYg;
  Zg = Zg - refZg + 1;

  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));

  // Roll & Pitch Equations
  roll  = -(atan2(-fYg, fZg) * 180.0) / M_PI; 
  pitch = (atan2(fXg, sqrt(fYg * fYg + fZg * fZg)) * 180.0) / M_PI;

  strMessage += roll;                     // 13
  strMessage += "|";
  strMessage += pitch;                    // 14
  strMessage += "|";
  strMessage += Xg;                       // 15
  strMessage += "|";
  strMessage += Yg;                       // 16
  strMessage += "|";
  strMessage += Zg;                       // 17
  strMessage += "|";


  float objt = tmp006.readObjTempC();
  //float diet = tmp006.readDieTempC();

  // External Temperature LM35
    sensor = analogRead(TempExtPin);          //lettura valore del sensore LM35 messo sull'ingresso analogico A1
    millivolts = ( sensor / 1023.0) * 5000;   //formula per ottenere la tensione di uscita dell'LM35 in millivolts
    celsius = millivolts / 10;                // valore espresso in gradi Celsius (l'out del sensore è 10mv per grado)

  strMessage += celsius;  // String(diet, 2);            // 20
  strMessage += "|";

  strMessage += String(objt, 2);            // 21
  strMessage += "|";

  
  strMessage += "$END|";
  
  // comment the line below before deploy
  Serial.println(strMessage);  
  
  //if (ss.overflow() )
  //  delay(10);


  BTSerial.print(strMessage);
}


/*********************************************/
/***********      FUNCTIONS  ****************/
/********************************************/





