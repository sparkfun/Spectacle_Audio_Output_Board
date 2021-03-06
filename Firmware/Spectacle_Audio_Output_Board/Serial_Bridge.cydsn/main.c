/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main()
{
  CyGlobalIntEnable; /* Enable global interrupts. */
  UART_PC_Start();
  UART_PC_UartPutString("1234\n");
  UART_VS_Start();

  POWBTN_Write(1);
  CyDelay(5);
  POWBTN_Write(0);

  uint32 rxBuffer = 0;

  /* Place your initialization/startup code here (e.g. MyInst_Start()) */

  for(;;)
  {
    rxBuffer = UART_PC_UartGetByte();
    if (!(rxBuffer & UART_PC_UART_RX_UNDERFLOW)) UART_VS_UartPutChar(rxBuffer);
    rxBuffer = UART_VS_UartGetByte();
    if (!(rxBuffer & UART_VS_UART_RX_UNDERFLOW)) UART_PC_UartPutChar(rxBuffer);
  }
}

/* [] END OF FILE */
