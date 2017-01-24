/****************************************************************************
audio.c
Code for controlling the VS1000D chip
Mike Hord @ SparkFun Electronics
24 Jan 2017
https://github.com/sparkfun/Spectacle_Audio_Output_Board

This file contains the function which sends the "play file" command to the 
VS1000D chip which does the heavy lifting of playing audio from SD.

Development environment specifics:
Developed in PSoC Creator 4.0
Board V10

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
****************************************************************************/

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
