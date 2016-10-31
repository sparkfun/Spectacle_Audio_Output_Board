#include <project.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "spectacle.h"

volatile int32 systemTimer = 0;

CY_ISR_PROTO(tickISR);
void playSound(uint8 soundIndex);

Board registerSpace;

int main()
{
  CyGlobalIntEnable; /* Enable global interrupts. */
  
  EZI2C_Start();
  EZI2C_EzI2CSetBuffer1(I2C_BUFFER_SIZE, I2C_BUFFER_RW_BOUNDARY, (uint8*)&registerSpace);
  
  UART_PC_Start();  
  UART_VS_Start();
  
  CyIntSetSysVector((SysTick_IRQn + 16), tickISR);
  SysTick_Config(48000);
  
  int32 _100HzTick = 0;
  uint8 temp;
  int32 _2HzTick = 0;
  char buffer[64];
  uint32 rxBuffer = 0;
  
  uint8 *boardID = 0;
  uint8 *boardType = 0;
  uint8 *change = 0;
  uint8 *soundID = 0;
  
  boardID = &registerSpace.boardID;
  boardType = &registerSpace.type;
  change = &registerSpace.change;
  soundID = &registerSpace.data[0];
  
  UART_PC_UartPutString("Hello world\n");
  playSound(1);
  LED_Write(1);
  
  for(;;)
  {
    rxBuffer = UART_VS_UartGetByte();
    if (!(rxBuffer & UART_VS_UART_RX_UNDERFLOW)) UART_PC_UartPutChar(rxBuffer);
    // One hundred times per second,
    if ((systemTimer - 10) > _100HzTick)
    {
      _100HzTick = systemTimer;
      if (*change == 1)
      {
        *change = 0;
        playSound(*soundID);
      }
    }
    if ((systemTimer - 500) > _2HzTick)
    {
      _2HzTick = systemTimer;
      if (LED_Read() != 0)
      {
        LED_Write(0);
      }
      else
      {
        LED_Write(1);
      }
    }
  }
}

void playSound(uint8 soundIndex)
{
  char buffer[16];
  UART_VS_UartPutString("C\n");
  CyDelay(50);
  sprintf(buffer, "P%-8.2uOGG\n", soundIndex);
  UART_VS_UartPutString(buffer);
}

CY_ISR(tickISR)
{
  systemTimer++;
}



/* [] END OF FILE */
