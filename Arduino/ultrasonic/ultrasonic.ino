// 초음파센서의 송신부를 8번핀으로 선언하고 수신부는 9번핀으로 선언합니다.
int trig = 11;
int echo = 12;
int cnt = 0;
int data =0;
 
 
// 실행시 가장 먼저 호출되는 함수이며, 최초 1회만 실행됩니다.
// 변수를 선언하거나 초기화를 위한 코드를 포함합니다.
void setup() {
  // 초음파센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 9600bps)
  Serial.begin(9600);
  //초음파 송신부-> OUTPUT, 초음파 수신부 -> INPUT,  LED핀 -> OUTPUT
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
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
  if(cnt >= 5){
    data = 1;
  }else{
    data = 0;
  }
  Serial.print(data);
  Serial.println("");
  // 0.2초 동안 대기합니다.
  delay(200);
}
