#include <Wire.h>

enum {YES_INT, NO_INT};

#define PROG_ENABLE_REG 200
#define PROG_READY_REG  201
#define DATA_READY_REG  202
#define INC_STRUCT_TOP  128

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

  // Teach the daughter board about the behaviors we want it to use.
  sendByte(PROG_ENABLE_REG, 1);
  while (progReady() == 0);
  audioInit(0, 100, 0, NO_INT, 0, 10, 1000, 0);
  audioInit(1, 100, 1, YES_INT, 0, 10, 1000, 0);
  audioInit(2, 100, 2, YES_INT, 0, 10, 1000, 0);
  audioInit(3, 100, 3, YES_INT, 0, 10, 1000, 0);
  audioInit(4, 100, 4, YES_INT, 0, 10, 1000, 0);
  sendByte(PROG_ENABLE_REG, 0);
}

void loop() 
{
  if (digitalRead(2) != HIGH)
  {
    sendCmd(0, 500);
    while(digitalRead(2) == LOW)
    {
    }
    sendCmd(0, 0);
  }
  if (digitalRead(3) != HIGH)
  {
    sendCmd(1, 500);
    while(digitalRead(3) == LOW)
    {
    }
    sendCmd(1,0);
  }
  if (digitalRead(4) != HIGH)
  {
      sendCmd(2, 500);
    while(digitalRead(4) == LOW)
    {
    }
    sendCmd(2, 0);
  }
  if (digitalRead(5) != HIGH)
  {
      sendCmd(3, 500);
    while(digitalRead(5) == LOW)
    {
      delay(15);
    }
    sendCmd(3, 0);
  }
  if (digitalRead(6) != HIGH)
  {
      sendCmd(4, 500);
    while(digitalRead(6) == LOW)
    {
    }
    sendCmd(4, 0);
  }
}

void sendCmd(byte offset, uint16_t value)
{
  Wire.beginTransmission(0x08);
  Wire.write(2*offset);
  Wire.write(value);
  Wire.write(value>>8);
  Wire.endTransmission();
} 

void sendByte(byte offset, byte value)
{
  Wire.beginTransmission(0x08);
  Wire.write(offset);
  Wire.write(value);
  Wire.endTransmission();
} 

void sendWord(byte offset, uint16_t value)
{
  Wire.beginTransmission(0x08);
  Wire.write(offset);
  Wire.write(value);
  Wire.write(value>>8);
  Wire.endTransmission();
}

void sendLWord(byte offset, uint32_t value)
{
  Wire.beginTransmission(0x08);
  Wire.write(offset);
  Wire.write(value);
  Wire.write(value>>8);
  Wire.write(value>>16);
  Wire.write(value>>24);
  Wire.endTransmission();
}

byte progReady()
{
  byte resp = 0;
  Wire.beginTransmission(0x08);
  Wire.write(PROG_READY_REG);
  Wire.endTransmission();
  Wire.requestFrom(0x08, 1);
  while (Wire.available() == 0);
  return Wire.read();
}

byte dataAccepted()
{
  Wire.beginTransmission(0x08);
  Wire.write(DATA_READY_REG);
  Wire.endTransmission();
  Wire.requestFrom(0x08, 1);
  while (Wire.available() == 0);
  return Wire.read();
}


void audioInit(uint8_t channel, int16_t threshold, uint8_t soundIndex, 
               uint8_t interruptable, uint8_t pending, int32_t predelay, 
               int32_t postdelay, int32_t audioTimer)
{
  sendByte(INC_STRUCT_TOP, channel);
  sendWord(INC_STRUCT_TOP + 1, threshold);
  sendByte(INC_STRUCT_TOP + 3, soundIndex);
  sendByte(INC_STRUCT_TOP + 4, interruptable);
  sendWord(INC_STRUCT_TOP + 5, pending);
  sendWord(INC_STRUCT_TOP + 6, predelay);
  sendLWord(INC_STRUCT_TOP + 10, postdelay);
  sendLWord(INC_STRUCT_TOP + 14, audioTimer);
  sendLWord(INC_STRUCT_TOP + 18, 0);
  sendLWord(INC_STRUCT_TOP + 19, 0);
  
  sendByte(DATA_READY_REG, 1);
  while (dataAccepted() == 1);
}

