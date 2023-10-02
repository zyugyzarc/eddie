#define LL 2
#define LR 3
#define RL 4
#define RR 5

#include <math.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "000002";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LL, OUTPUT);
  pinMode(LR, OUTPUT);
  pinMode(RL, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(6, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void motor(int a, int b){
  digitalWrite(LL, a>0);
  digitalWrite(LR, a<0);
  digitalWrite(RL, b>0);
  digitalWrite(RR, b<0);
}

void motor(double a, double b){
  if(a > 0){
    analogWrite(LL, (int)(a*255));
    digitalWrite(LR, LOW);
  }
  else{
    analogWrite(LR, (int)(-a*255));
    digitalWrite(LL, LOW);
  }
  if(b > 0){
    analogWrite(RL, (int)(b*255));
    digitalWrite(RR, LOW);
  }
  else{
    analogWrite(RR, (int)(-b*255));
    digitalWrite(RL, LOW);
  }
}

void loop() {

  if(radio.available()){

    digitalWrite(6, HIGH);

    double buf[2];
    radio.read(&buf, sizeof(buf));

    Serial.print(buf[0]);
    Serial.print(",");
    Serial.println(buf[1]);
  
    motor(-buf[0]+buf[1], -buf[0]-buf[1]);

    delay(10);
    digitalWrite(6, LOW);
  }
}
