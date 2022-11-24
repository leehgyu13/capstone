#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int ParkID = 0;
int CarID = 6398;
int CarExist = 0;
int ChargeStatus = 1;
int leftparking = 3;
String ssid = "Arduinotest";                  // Network name to connect
String pwd = "abcdefgh";               // Network password
String dbHost = "leehgyu.iptime.org";  // Data base url
String dbPort = "8080";                  // Data base port

SoftwareSerial esp01(2, 3);
// SDA : A4, SCL : A5 
// rx : 3, tx : 2
// 초음파센서의 송신부를 8번핀으로 선언하고 수신부는 9번핀으로 선언합니다.
int trig1 = 11;
int echo1 = 12;
int trig2 = 6;
int echo2 = 7;
int trig3 = 8;
int echo3 = 9;

int cnt1 = 0;
int cnt2 = 0;
int cnt3 = 0;
int data1 = 0;
int data_pre1 = data1;
int data2 = 0;
int data_pre2 = data2;
int data3 = 0;
int data_pre3 = data3;

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
  delay(200);
  esp01.println();
  printResponse();
}

void setup() {
  Serial.begin(9600);   //시리얼모니터
  esp01.begin(9600); //와이파이 시리얼
  delay(2000);
  esp01.println("AT+RST\r\n");
  //delay(2000);
  while(!(esp01.find("ready"))){
    Serial.println("...rst");
  }
  esp01.println("AT+CWMODE=3\r\n");
  //delay(2000);
  while(!(esp01.find("OK"))){
    Serial.println("...mode");
  }
  connectWifi();

  // lcd
  lcd.init();
  lcd.backlight();
  
  //sensor
  //초음파 송신부-> OUTPUT, 초음파 수신부 -> INPUT,  LED핀 -> OUTPUT
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
}

void loop() {
  //sensor
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
 
  unsigned long duration1 = pulseIn(echo1, HIGH);
 
  // 초음파의 속도는 초당 340미터를 이동하거나, 29마이크로초 당 1센치를 이동합니다.
  // 따라서, 초음파의 이동 거리 = duration(왕복에 걸린시간) / 29 / 2 입니다.
  float distance1 = duration1 / 29.0 / 2.0;
 
  // 측정된 거리 값를 시리얼 모니터에 출력합니다.
  Serial.print(distance1);
  Serial.println("cm");
  if(distance1 <= 10){
    cnt1 = cnt1 +1;
  }else{
    cnt1 = 0;
  }
  if(cnt1 >= 3){
    data1 = 1;
  }else{
    data1 = 0;
  }
  Serial.println("sensor 1 : ");
  Serial.print(data1);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  delay(800);

  //wifi
  if(data_pre1 != data1){
    CarExist = data1;
    ParkID = 1;
    httpClient();
    Serial.println((String) "ParkID: " + ParkID);
    Serial.println((String) "CarExist: " + CarExist);
    dataSend(ParkID, CarExist);
    data_pre1 = data1;
  }

    //sensor
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
 
  unsigned long duration2 = pulseIn(echo2, HIGH);
 
  // 초음파의 속도는 초당 340미터를 이동하거나, 29마이크로초 당 1센치를 이동합니다.
  // 따라서, 초음파의 이동 거리 = duration(왕복에 걸린시간) / 29 / 2 입니다.
  float distance2 = duration2 / 29.0 / 2.0;
 
  // 측정된 거리 값를 시리얼 모니터에 출력합니다.
  Serial.print(distance2);
  Serial.println("cm");
  if(distance2 <= 10){
    cnt2 = cnt2 +1;
  }else{
    cnt2 = 0;
  }
  if(cnt2 >= 3){
    data2 = 1;
  }else{
    data2 = 0;
  }
  Serial.println("sensor 2 : ");
  Serial.print(data2);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  delay(800);

  //wifi
  if(data_pre2 != data2){
    CarExist = data2;
    ParkID = 2;
    httpClient();
    Serial.println((String) "ParkID: " + ParkID);
    Serial.println((String) "CarExist: " + CarExist);
    dataSend(ParkID, CarExist);
    data_pre2 = data2;
  }   
      //sensor
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
 
  unsigned long duration3 = pulseIn(echo3, HIGH);
 
  // 초음파의 속도는 초당 340미터를 이동하거나, 29마이크로초 당 1센치를 이동합니다.
  // 따라서, 초음파의 이동 거리 = duration(왕복에 걸린시간) / 29 / 2 입니다.
  float distance3 = duration3 / 29.0 / 2.0;
 
  // 측정된 거리 값를 시리얼 모니터에 출력합니다.
  Serial.print(distance3);
  Serial.println("cm");
  if(distance3 <= 10){
    cnt3 = cnt3 +1;
  }else{
    cnt3 = 0;
  }
  if(cnt3 >= 3){
    data3 = 1;
  }else{
    data3 = 0;
  }
  Serial.println("sensor 3 : ");
  Serial.print(data3);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  delay(800);

  //wifi
  if(data_pre3 != data3){
    CarExist = data3;
    ParkID = 3;
    httpClient();
    Serial.println((String) "ParkID: " + ParkID);
    Serial.println((String) "CarExist: " + CarExist);
    dataSend(ParkID, CarExist);
    data_pre3 = data3;
  } 
  
  leftparking = 3-(data1+data2+data3);
  lcd.setCursor(1,0);
  lcd.print("S");
  lcd.setCursor(2,0);
  lcd.print("K");
  lcd.setCursor(3,0);
  lcd.print("K");
  lcd.setCursor(4,0);
  lcd.print("U");
  lcd.setCursor(8,0);
  lcd.print("P");
  lcd.setCursor(9,0);
  lcd.print("A");
  lcd.setCursor(10,0);
  lcd.print("R");
  lcd.setCursor(11,0);
  lcd.print("K");
  lcd.setCursor(12,0);
  lcd.print("I");
  lcd.setCursor(13,0);
  lcd.print("N");
  lcd.setCursor(14,0);
  lcd.print("G");

  lcd.setCursor(2,1);
  lcd.print(leftparking);
  lcd.setCursor(4,1);
  lcd.print("e");
  lcd.setCursor(5,1);
  lcd.print("m");
  lcd.setCursor(6,1);
  lcd.print("p");
  lcd.setCursor(7,1);
  lcd.print("t");
  lcd.setCursor(8,1);
  lcd.print("y");
  
  lcd.setCursor(10,1);
  lcd.print("s");
  lcd.setCursor(11,1);
  lcd.print("p");
  lcd.setCursor(12,1);
  lcd.print("a");
  lcd.setCursor(13,1);
  lcd.print("c");
  lcd.setCursor(14,1);
  lcd.print("e");
  }
