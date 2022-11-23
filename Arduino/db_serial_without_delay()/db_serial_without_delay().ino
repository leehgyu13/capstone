#include <SoftwareSerial.h>

int ParkID = 1;
int CarExist = 0;

String ssid = "yeoi";                  // Network name to connect
String pwd = "qp98al76";               // Network password
String dbHost = "leehgyu.iptime.org";  // Data base url
String dbPort = "8080";                // Data base port

SoftwareSerial esp01(2, 3);

void printResponse() {
  while(esp01.available()){
    Serial.println(esp01.readStringUntil('\n'));
  }
}

void connectWifi() {
  Serial.println("Connecting wifi...\n");
  esp01.println("AT+CWJAP=\""+ssid+"\",\""+pwd+"\"\r\n");
  while(!(esp01.find("OK"))){
    Serial.println("...wifi");
  }
  Serial.println("wifi connected\n");
  delay(500);
}

void httpClient() {
  esp01.println("AT+CIPMUX=1");
  while(!(esp01.find("OK"))){
    Serial.println("...MUX");
  }
  printResponse();
  Serial.println("Connecting TCP...\n");
  esp01.println("AT+CIPSTART=4,\"TCP\",\""+dbHost+"\","+dbPort);
  while(!(esp01.find("OK"))){
    Serial.println("...TCP");
  }
  printResponse();
}

void dataSend(int ParkID, int CarExist){
  Serial.println("Sending data...\n");
  String strParkID = String(ParkID);
  String strCarExist = String(CarExist);
  String cmd = "GET http://"+dbHost+":"+dbPort+"/insert.php?ParkID="+strParkID+"&CarExist="+strCarExist+" HTTP/1.0";
  esp01.println("AT+CIPSEND=4," + String(cmd.length() + 4));
  while(!(esp01.find("OK"))){
    Serial.println("...SEND");
  }
  esp01.println(cmd);
  /*while(!(esp01.find("CLOSED"))){
    Serial.println("...send");
  }*/
  delay(200);
  esp01.println();
  //delay(200);
  printResponse();
  //delay(5000);
}

void setup() {
  Serial.begin(9600);   //시리얼모니터
  esp01.begin(9600); //와이파이 시리얼
  delay(2000);
  Serial.println("setup start");
  esp01.println("AT+RST\r\n");
  while(!(esp01.find("ready"))){
    Serial.println("...rst");
  }
  Serial.println("rst done");
  esp01.println("AT+CWMODE=3\r\n");
  while(!(esp01.find("OK"))){
    Serial.println("...mode");
  }
  Serial.println("mode done");
  connectWifi();
  Serial.println("setup end");
  //delay(500);
}

void loop() {
  Serial.println("loop start");
  httpClient();
  //delay(5000);
  Serial.println((String) "ParkID: " + ParkID);
  Serial.println((String) "CarExist: " + CarExist);
  //delay(1000);
  dataSend(ParkID, CarExist);
  if(CarExist == 0) {
    CarExist = 1;
  } else if(CarExist == 1) {
    CarExist = 0;
  }
  Serial.println("loop end");
}
