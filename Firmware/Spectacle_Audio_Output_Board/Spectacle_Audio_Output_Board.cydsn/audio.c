#include <project.h>
#include <stdio.h>
#include "audio.h"

void playSound(uint8 soundIndex)
{
  char buffer[16];
  UART_VS_UartPutString("C\n");
  CyDelay(50);
  sprintf(buffer, "P%-8.2uOGG\n", soundIndex);
  UART_VS_UartPutString(buffer);
}

struct audio audioInit(uint8 channel, int16 threshold, uint8 soundIndex,
                       uint8 interruptable, uint8 pending, int32 predelay,
                       int32 postdelay, int32 audioTimer)
{
  struct audio temp = {channel, threshold, soundIndex, interruptable, 
                       pending, predelay, postdelay, audioTimer};
  return temp;
}

