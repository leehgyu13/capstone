
#include <Wire.h>
#include "DFRobot_INA219.h"
#include <SoftwareSerial.h>


/**
 * @fn DFRobot_INA219_IIC
 * @brief pWire I2C controller pointer
 * @param i2caddr  I2C address
 * @n INA219_I2C_ADDRESS1  0x40   A0 = 0  A1 = 0
 * @n INA219_I2C_ADDRESS2  0x41   A0 = 1  A1 = 0
 * @n INA219_I2C_ADDRESS3  0x44   A0 = 0  A1 = 1
 * @n INA219_I2C_ADDRESS4  0x45   A0 = 1  A1 = 1   
  */
DFRobot_INA219_IIC     ina219(&Wire, INA219_I2C_ADDRESS4);

// Revise the following two paramters according to actual reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1000;
float extMeterReading_mA = 1000;

//relay
int relay = 8;
char cmd;

int ParkID = 0;



void wattMeter()
{
    Serial.println(ina219.getPower_mW(), 5);
}




void setup() 
{
    Serial.begin(9600);
    delay(2000);

    //Initialize the sensor
    while(ina219.begin() != true) {
        Serial.println("INA219 begin faild");
        delay(2000);
    }
    
    //Linear calibration
    ina219.linearCalibrate(/*The measured current before calibration*/ina219Reading_mA, /*The current measured by other current testers*/extMeterReading_mA);
  
    //relay
    pinMode(relay,OUTPUT);
    delay(1000);
}

void loop() {
  while(Serial.available() <= 0){
    delay(100);
  }
  cmd = Serial.read();
  if(cmd == 'a'){
    digitalWrite (relay,HIGH);
    wattMeter();
  }
  if(cmd == 'b'){
    digitalWrite(relay, LOW);
  }
}
