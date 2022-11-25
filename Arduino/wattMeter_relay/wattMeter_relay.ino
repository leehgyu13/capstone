#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>

#include <Wire.h>
#include "DFRobot_INA219.h"
#include <SoftwareSerial.h>
#include <WiFiEsp.h>

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

//Serial
char ssid[] = "SMSA";                  // Network name to connect
char pwd[] = "smsa2ndgeneration";               // Network password
char server[] = "leehgyu.iptime.org";
int status = WL_IDLE_STATUS;
int ParkID = 0;

WiFiEspClient client;
SoftwareSerial esp01(2, 3);

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

void dataSend(int ParkID, int CarExist){
  Serial.println("Sending data...\n");
  String strParkID = String(ParkID);
  String strCarExist = String(CarExist);
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 8080)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /insert.php?ParkID="+strParkID+"&CarExist="+strCarExist+" HTTP/1.1");
    client.println("Host: leehgyu.iptime.org");
    client.println("Connection: close");
    client.println();
  }
}

void dataRecv(){
  Serial.println("data receiving...\n");
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 8080)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /post.php HTTP/1.1");
    client.println("Host: leehgyu.iptime.org");
    client.println("Connection: close");
    client.println();
    Serial.println("receive");
  }
}

void setup() 
{
    Serial.begin(9600);
    esp01.begin(9600);
    WiFi.init(&esp01);
    delay(2000);
    // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pwd);
  }
  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  printWifiStatus();
  Serial.println();

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

void loop() {
  dataRecv();
  while (client.available()) {
    char ParkID_char = client.read();
    Serial.write(ParkID_char);
    ParkID = ParkID_char - '0';
  }
  Serial.println("\n"+ParkID);
  if(ParkID != 0) {
    digitalWrite (relay,HIGH);
    wattMeter();
    //data send
  } else {
    digitalWrite (relay ,LOW);
  }
}

void printWifiStatus(){
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}