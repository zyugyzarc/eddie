#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "000002";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, OUTPUT);
}

double buf[2];

void loop() {
  digitalWrite(2, HIGH);
  buf[0] = (analogRead(A2)/512.0) - 1;
  buf[1] = (analogRead(A1)/512.0) - 1;
  Serial.print(buf[0]);
  Serial.print(",");
  Serial.println(buf[1]);
  radio.write(&buf, sizeof(buf));
  delay(10);
  digitalWrite(2, LOW);
  delay(10);
}
