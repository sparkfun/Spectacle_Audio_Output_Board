#include <Wire.h>

void setup() 
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Hello, world!");
}

void loop() 
{
  if (digitalRead(2) != HIGH)
  {
    sendCmd(1, 0);
    Serial.println("Set");
    delay(50);
    while(digitalRead(2) == LOW);
  }
  if (digitalRead(3) != HIGH)
  {
    sendCmd(1, 1);
    Serial.println("Fade");
    delay(50);
    while(digitalRead(3) == LOW);
  }
  if (digitalRead(4) != HIGH)
  {
    sendCmd(1, 2);
    Serial.println("Rainbow");
    delay(50);
    while(digitalRead(4) == LOW);
  }
  if (digitalRead(5) != HIGH)
  {
    sendCmd(1,3);
    Serial.println("Scan");
    delay(50);
    while(digitalRead(5) == LOW);
  }
  if (digitalRead(6) != HIGH)
  {
    sendCmd(1, 4);
    Serial.println("Twinkle");
    delay(50);
    while(digitalRead(6) == LOW);
  }
}

void sendCmd(byte change, byte soundID)
{
  Wire.beginTransmission(0x09);
  Wire.write(2);
  Wire.write(1);
  Wire.write(soundID);
  Wire.endTransmission();
}

