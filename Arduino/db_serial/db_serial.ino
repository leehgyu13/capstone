#include <SoftwareSerial.h>

int ParkID;
int CarID;
int CarExist;
int ChargeStatus;
String ssid = "yeoi";                  // Network name to connect
String pwd = "qp98al76";               // Network password
String dbHost = "leehgyu.iptime.org";  // Data base url
String dbPort = "8080";                  // Data base port

SoftwareSerial esp01(2, 3);

void printResponse() {
  while(esp01.available()){
    Serial.println(esp01.readStringUntil('\n'));
  }
}

void connectWifi() {
  Serial.println("Connecting wifi...\n");
  esp01.println("AT+CWJAP=\""+ssid+"\",\""+pwd+"\"\r\n");
  delay(5000);
  if(esp01.find("OK")) {
    Serial.println("wifi connected\n");
  } else {
    Serial.println("Connect timeout\n");
  }
  //delay(1000);
}

void httpClient() {
  ParkID = 3;
  CarID = 6398;
  CarExist = 1;
  ChargeStatus = 1;
  // 데이터 받는 코드 만들기
  esp01.println("AT+CIPMUX=1");
  delay(1000);
  printResponse();
  Serial.println("Connecting TCP...\n");
  esp01.println("AT+CIPSTART=4,\"TCP\",\""+dbHost+"\","+dbPort);
  delay(1000);
  printResponse();
  if(Serial.find("ERROR")) return;
  
  Serial.println("Sending data...\n");
  String strParkID = String(ParkID);
  String strCarID = String(CarID);
  String strCarExist = String(CarExist);
  String strChargeStatus = String(ChargeStatus);
  String cmd = "GET http://"+dbHost+":"+dbPort+"/insert.php?ParkID="+strParkID+"&CarID="+strCarID+"&CarExist="+strCarExist+"&Chargestatus="+strChargeStatus+" HTTP/1.0";
  esp01.println("AT+CIPSEND=4," + String(cmd.length() + 4));
  delay(1000);
  esp01.println(cmd);
  delay(200);
  esp01.println();
  delay(200);
  printResponse();
}

void setup() {
  Serial.begin(9600);   //시리얼모니터
  esp01.begin(9600); //와이파이 시리얼
  delay(2000);
  esp01.println("AT+RST\r\n");
  delay(2000);
  esp01.println("AT+CWMODE=3\r\n");
  delay(2000);
  connectWifi();
  //delay(500);
}

void loop() {
  httpClient();
  //delay(1000);

  exit(0);
  //delay(5000);
}
