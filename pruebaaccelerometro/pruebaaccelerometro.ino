
#include <Wire.h>
#include <SparkFun_ADXL345.h>


ADXL345 adxl = ADXL345(); 

void setup(void) 
{
  Serial.begin(9600);
  adxl.powerOn();  
  adxl.setRangeSetting(16);

}

void loop(void) 
{
 int XAxis,YAxis,ZAxis;  
 adxl.readAccel(&XAxis, &YAxis, &ZAxis);
   
  // Calculate Pitch & Roll
  int pitch = -(atan2(XAxis, sqrt(YAxis*YAxis + ZAxis*ZAxis))*180.0)/M_PI;                                            
  int roll  = (atan2(YAxis, ZAxis)*180.0)/M_PI;


  // Output
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);

  Serial.print(XAxis);
  Serial.print(", ");
  Serial.print(YAxis);
  Serial.print(",");
  Serial.println(ZAxis); 


  delay(2000);
}
