/*****************************************************************************************************
  File:             readLux.ino
  Description:      This example demonstrates how to obtain ambient light intensity.
  Note:
******************************************************************************************************/
#include "BM62S6021-1.h"

// BM62S6021_1 Light_Module(2, 5, 4); //Please comment out this line of code if you don't use SW Serial
BM62S6021_1 Light_Module(22,&Serial1);//Please comment out this line of code if you  use Serial1 on BMduino
//BM62S6021_1 Light_Module(25,&Serial2);//Please comment out this line of code if you  use Serial2 on BMduino
//BM62S6021_1 Light_Module(2,&Serial3);//Please uncomment out this line of code if you use HW Seria3 on BMduino
//BM62S6021_1 Light_Module(2,&Serial4);//Please uncomment out this line of code if you use HW Seria4 on BMduino

uint32_t lux1, lux2, lux3;

void setup()
{
  Serial.begin(9600);
  Light_Module.begin();
  delay(20);
}

void loop()
{

  /*******uncomment to get light intensity of single module  ******************/
  lux1 = Light_Module.readLux(1);
  Serial.print("Lux1:");
  Serial.println(lux1);

  /*******uncomment to get light intensity of multiple modules ******************/
  lux2 = Light_Module.readLux(2);
  Serial.print("Lux2:");
  Serial.println(lux2);

  lux3 = Light_Module.readLux(2);
  Serial.print("Lux3:");
  Serial.println(lux3);

  delay(1000);
}
