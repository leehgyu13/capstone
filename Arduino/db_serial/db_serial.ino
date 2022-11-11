#include <SoftwareSerial.h>

int ParkID;
int CarID;
int CarExist;
int ChargeStatus;

SoftwareSerial esp01(2, 3);

void setup() {
  Serial.begin(9600);   //시리얼모니터
  esp01.begin(9600); //와이파이 시리얼
  delay(2000);
  esp01.println("AT+RST\r\n");
  delay(2000);
  esp01.println("AT+CWMODE=3\r\n");
  delay(2000);
  esp01.println("AT+CWJAP=\"yeoi\",\"qp98al76\"\r\n");
  delay(5000);
}

void printResponse() {
  while(esp01.available()){
    Serial.println(esp01.readStringUntil('\n'));
  }
}

void loop() {
  ParkID = 3;
  CarID = 6398;
  CarExist = 1;
  ChargeStatus = 1;
  //Serial.println(ParkID);
  //Serial.println(", ");
  //Serial.println(CarID);
  //Serial.println(", ");
  //Serial.println(CarExist);
  //Serial.println(", ");
  //Serial.println(ChargeStatus);
  //Serial.println("\n");
  esp01.println("AT+CIPMUX=1");
  delay(1000);
  printResponse();
  esp01.println("AT+CIPSTART=4,\"TCP\",\"leehgyu.iptime.org\",8080");
  delay(1000);
  printResponse();
  String strParkID = String(ParkID);
  String strCarID = String(CarID);
  String strCarExist = String(CarExist);
  String strChargeStatus = String(ChargeStatus);
  String cmd = "GET http://leehgyu.iptime.org:8080/insert.php?ParkID="+strParkID+"&CarID="+strCarID+"&CarExist="+strCarExist+"&Chargestatus="+strChargeStatus+" HTTP/1.0";
  esp01.println("AT+CIPSEND=4," + String(cmd.length() + 4));
  delay(1000);
  esp01.println(cmd);
  delay(200);
  esp01.println();
  delay(200);
  printResponse();

  exit(0);
  //delay(5000);
}