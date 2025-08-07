/*****************************************************************************************************
  File:             setID.ino
  Description:      This example demonstrates how to set the module ID
  Note:
******************************************************************************************************/
#include "BM62S6021-1.h"

// BM62S6021_1 Light_Module(2, 5, 4); //Please comment out this line of code if you don't use SW Serial
BM62S6021_1 Light_Module(22,&Serial1);//Please comment out this line of code if you  use Serial1 on BMduino
//BM62S6021_1 Light_Module(25,&Serial2);//Please comment out this line of code if you  use Serial2 on BMduino
//BM62S6021_1 Light_Module(2,&Serial3);//Please uncomment out this line of code if you use HW Seria3 on BMduino
//BM62S6021_1 Light_Module(2,&Serial4);//Please uncomment out this line of code if you use HW Seria4 on BMduino

uint8_t Target_ID = 2;          //Target ID

void setup()
{
  Serial.begin(9600);
  Light_Module.begin();
  Light_Module.reset();
  delay(1000);
  if (Light_Module.setID(1, Target_ID))
  {
    Serial.print("setID succeed!  current ID:");
    Serial.println(Target_ID);
  }
  else
  {
    Serial.println("setID fail!");
  }

}

void loop()
{

}
