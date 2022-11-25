/*!
 *@file getVoltageCurrentPower.ino
 *@brief Get the current, voltage, and power of electronic devices.
 *@copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 *@license     The MIT license (MIT)
 *@author [fengli](li.feng@dfrobot.com)
 *@version  V1.0
 *@date  2022-3-1
 *@url https://github.com/DFRobot/DFRobot_INA219
*/

#include <Wire.h>
#include "DFRobot_INA219.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "SimpleDHT.h"

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

//communication
SimpleDHT11 dht11;
const char* ssid = "SMSA";
const char* passwd = "smsa2ndgeneration";
HTTPClient http;
WiFiClient wifi;

int ParkID = 0;

void setup() 
{
    Serial.begin(9600);
    //Open the serial port
    while(!Serial);
    
    Serial.println();

    WiFi.begin(ssid, passwd);

    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Watiting for connection");
    }

    //Initialize the sensor
    while(ina219.begin() != true) {
        Serial.println("INA219 begin faild");
        delay(2000);
    }
    //Linear calibration
    ina219.linearCalibrate(/*The measured current before calibration*/ina219Reading_mA, /*The current measured by other current testers*/extMeterReading_mA);
    Serial.println();

    //relay
    pinMode(relay,OUTPUT);
}

void wattMeter()
{
    Serial.print("BusVoltage:   ");
    Serial.print(ina219.getBusVoltage_V(), 2);
    Serial.println("V");
    Serial.print("ShuntVoltage: ");
    Serial.print(ina219.getShuntVoltage_mV(), 3);
    Serial.println("mV");
    Serial.print("Current:      ");
    Serial.print(ina219.getCurrent_mA(), 1);
    Serial.println("mA");
    Serial.print("Power:        ");
    Serial.print(ina219.getPower_mW(), 1);
    Serial.println("mW");
    Serial.println("");
    delay(1000);
}

void loop () {
  http.begin(wifi,"http://leehgyu.iptime.org:8080/post.php");
  http.addHeader("Content-Type", "text/html;charset=UTF-8");
  int httpCode = http.POST("");
  Serial.println(httpCode);
  if(httpCode == HTTP_CODE_OK) {
    String ParkID_str = http.getString();
    ParkID = ParkID_str.toInt();
    if(ParkID != 0) {
      digitalWrite (relay,HIGH);
      wattMeter();
      //data send
    } else {
        digitalWrite (relay ,LOW);
    }
  }
}