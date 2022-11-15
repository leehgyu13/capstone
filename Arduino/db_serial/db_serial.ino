#include <SoftwareSerial.h>

int ParkID = 0;
int CarID = 6398;
int CarExist = 0;
int ChargeStatus = 1;

String ssid = "Arduinotest";                  // Network name to connect
String pwd = "abcdefgh";               // Network password
String dbHost = "leehgyu.iptime.org";  // Data base url
String dbPort = "8080";                  // Data base port

SoftwareSerial esp01(2, 3);

// 초음파센서의 송신부를 8번핀으로 선언하고 수신부는 9번핀으로 선언합니다.
int trig = 11;
int echo = 12;
int cnt = 0;
int data = 0;
int data_pre = data;

void printResponse() {
  while(esp01.available()){
    Serial.println(esp01.readStringUntil('\n'));
  }
}

void connectWifi() {
  Serial.println("Connecting wifi...\n");
  esp01.println("AT+CWJAP=\""+ssid+"\",\""+pwd+"\"\r\n");
  delay(10000);
  if(esp01.find("OK")) {
    Serial.println("wifi connected\n");
  } else {
    Serial.println("Connect timeout\n");
  }
  delay(5000);
}

void httpClient() {
  esp01.println("AT+CIPMUX=1");
  delay(5000);
  printResponse();
  Serial.println("Connecting TCP...\n");
  esp01.println("AT+CIPSTART=4,\"TCP\",\""+dbHost+"\","+dbPort);
  delay(5000);
  printResponse();
  if(Serial.find("ERROR")) return;
}

void dataSend(int ParkID, int CarID, int CarExist, int ChargeStaus){
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
  delay(5000);
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
  delay(500);
  //httpClient();
  //delay(1000);

  //sensor
  //초음파 송신부-> OUTPUT, 초음파 수신부 -> INPUT,  LED핀 -> OUTPUT
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  //sensor
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  unsigned long duration = pulseIn(echo, HIGH);
 
  // 초음파의 속도는 초당 340미터를 이동하거나, 29마이크로초 당 1센치를 이동합니다.
  // 따라서, 초음파의 이동 거리 = duration(왕복에 걸린시간) / 29 / 2 입니다.
  float distance = duration / 29.0 / 2.0;
 
  // 측정된 거리 값를 시리얼 모니터에 출력합니다.
  Serial.print(distance);
  Serial.println("cm");
  if(distance <= 10){
    cnt = cnt +1;
  }else{
    cnt = 0;
  }
  if(cnt >= 5){
    data = 1;
  }else{
    data = 0;
  }
  Serial.print(data);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  delay(200);

  //wifi
  if(data_pre != data){
    //ParkID = 0;
    //CarID = 6398;
    //CarExist = 1;
    //ChargeStatus = 1;
    //데이터 받는 코드 만들기
    CarExist = data;
    httpClient();
    delay(5000);
    Serial.println((String) "ParkID: " + ParkID);
    Serial.println((String) "CarID: " + CarID);
    Serial.println((String) "CarExist: " + CarExist);
    Serial.println((String) "ChargeStatus: " + ChargeStatus);
    delay(1000);
    dataSend(ParkID, CarID, CarExist, ChargeStatus);
    //ParkID += 1;
    //CarID += 1;
    //if(ParkID >= 3){
      //exit(0);
    //}  

    //exit(0);
    //delay(5000);
    data_pre = data;
  }
}
