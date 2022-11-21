int relay = 8;

void setup() {
  //pinMode(13,OUTPUT);
  //digitalWrite(13,HIGH);
  pinMode(relay,OUTPUT);
}

void loop () {
  digitalWrite (relay,HIGH);
  delay (2000);
  digitalWrite (relay ,LOW);
  delay(2000);
}