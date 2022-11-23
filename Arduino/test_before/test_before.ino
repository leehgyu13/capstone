#include <SoftwareSerial.h>
// 초음파센서의 송신부를 8번핀으로 선언하고 수신부는 9번핀으로 선언합니다.
int trig = 11;
int echo = 12;
int cnt = 0;
int data =0;
int ParkID;
int CarID;
int CarExist;
int ChargeStatus;
int cntduration = 3; //지속시간
String ssid = "Arduinotest";                  // Network name to connect
String pwd = "abcdefgh";               // Network password
String dbHost = "leehgyu.iptime.org";  // Data base url
String dbPort = "8080";                  // Data base port

SoftwareSerial esp01(2, 3);

// 실행시 가장 먼저 호출되는 함수이며, 최초 1회만 실행됩니다.
// 변수를 선언하거나 초기화를 위한 코드를 포함합니다.
void setup() {
  // 초음파센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 9600bps)
  Serial.begin(9600); //시리얼모니터
  esp01.begin(9600); //와이파이 시리얼
  esp01.println("AT+RST\r\n");
  delay(2000);
  esp01.println("AT+CWMODE=3\r\n");
  delay(2000);
  connectWifi();
  delay(500);
  //초음파 송신부-> OUTPUT, 초음파 수신부 -> INPUT,  LED핀 -> OUTPUT
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
}

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

 
// setup() 함수가 호출된 이후, loop() 함수가 호출되며,
// 블록 안의 코드를 무한히 반복 실행됩니다.
void loop() {
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
  if(cnt >= cntduration){
    data = 1;
  }else{
    data = 0;
  }
  Serial.print(data);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  //delay(200);
  ParkID = 3;
  CarID = 6398;
  CarExist = data; // data update
  ChargeStatus = 1;
  // 데이터 받는 코드 만들기
  esp01.println("AT+CIPMUX=1");
  delay(1000);
  printResponse();
  Serial.println("Connecting TCP...\n");
  esp01.println("AT+CIPSTART=4,\"TCP\",\""+dbHost+"\","+dbPort);
  delay(1000);
  printResponse();
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
