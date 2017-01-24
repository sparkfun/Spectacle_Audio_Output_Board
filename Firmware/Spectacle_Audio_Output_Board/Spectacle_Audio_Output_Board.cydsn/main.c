/****************************************************************************
main.c
Main code file for Spectacle Audio Board Firmware
Mike Hord @ SparkFun Electronics
24 Jan 2017
https://github.com/sparkfun/Spectacle_Audio_Output_Board

This file contains main(), which configures all the various hardware
components for operation and runs the main loop that handles incoming data
from the director board.

Development environment specifics:
Developed in PSoC Creator 4.0
Board V10

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
****************************************************************************/

#include <project.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "spectacle.h"
#include "audio.h"
#include "programming.h"

#define I2C_BUFFER_SIZE 256
#define I2C_BUFFER_RW_BOUNDARY 256

// systemTimer is incremented in the tickISR, which occurs once a millisecond.
//  It's the timebase for the entire firmware, upon which all other timing
//  is based.
volatile int32 systemTimer = 0;
CY_ISR_PROTO(tickISR);


// behaviorListLen is the variable which tracks the number of behaviors that
// the director board has passed into the servo board. During normal operation,
// we will iterate over the object 'behaviors' 'behaviorListLen' times every 10
// milliseconds.
int behaviorListLen = 0;

struct audio *behaviors;

// mailboxes is where our channel data comes in. A spectacle system can have
// up to 64 channels of behaviors, each of which is an int16.
int16 mailboxes[128];

volatile uint8 *I2C_Mem;

int main()
{
  CyGlobalIntEnable; /* Enable global interrupts. */
  
  // Clear the mailboxes of any cruft that may be there.
  bzero(mailboxes, 128);

  // Point I2C_Mem to mailboxes, for a more generalized I2C space addressing.
  I2C_Mem = (uint8*)mailboxes;

  I2C_Mem[PROG_ENABLE_REG] = 0;
  I2C_Mem[PROG_READY_REG] = 0;
  I2C_Mem[DATA_READY_REG] = 0;
  I2C_Mem[BOARD_ID_REG] = BOARD_ID;

  // This is the startup code for the incoming I2C peripheral. We first enable
  // the peripheral, then tell it what it needs to know about the memory it
  // will be looking at for later decision making.
  EZI2C_Start();
  EZI2C_EzI2CSetBuffer1(I2C_BUFFER_SIZE, I2C_BUFFER_RW_BOUNDARY, (uint8*)mailboxes);
  
  // Start the "to PC" UART. This is only used during development!
  UART_PC_Start();  
  UART_PC_UartPutString("Hello world\n");

  // Start the "to VS1000" UART. This is the path used to control the audio
  // codec chip.
  UART_VS_Start();

  // "Power cycle" the VS1000 chip. Without this, the chip will probably
  // (although not assuredly) not work. Don't go removing it!
  POWBTN_Write(1);
  CyDelay(5);
  POWBTN_Write(0);

  // ARM devices have an internal system tick which can be used as a timebase
  // without having to tie up additional system resources. Here we set it up to
  // point to our system tick ISR and configure it to occur once every 48000
  // clock ticks, which is once per millisecond on our 48MHz processor.
  CyIntSetSysVector((SysTick_IRQn + 16), tickISR);
  SysTick_Config(48000);
  
  // Create our timing "tick" variables. These are used to record when the last
  // iteration of an action in the main loop happened and to trigger the next
  // iteration when some number of clock ticks have passed.
  int32 _100HzTick = 0;
  int32 _2HzTick = 0;

  // Iterator.
  int i = 0;

  // Pending audio flag. This depends on the pre- and postdelays, NOT on the
  // actual sound being playing or not.
  uint8 pending = 0;
  //I2C_Mem[CONFIGURED_REG] = 0;
  behaviors = malloc(64*sizeof(behaviors));
  // Turn LED on. This is a useful check on whether or not the code made it
  // through the initialization process. If it did, the LED will come on. If
  // not, the LED stays off. If it gets hung in the loop, the LED will stop
  // blinking.
  LED_Write(1);

  for(;;)
  {
    uint32 rxBuffer = UART_VS_UartGetByte();
    if (!(rxBuffer & UART_VS_UART_RX_UNDERFLOW)) UART_PC_UartPutChar(rxBuffer);
    // One hundred times per second,
    if ((systemTimer - 10) > _100HzTick)
    {
      _100HzTick = systemTimer;

      if (I2C_Mem[PROG_ENABLE_REG] == 1)
      {
        program();
      }
      if (I2C_Mem[CONFIGURED_REG] == 1)
      {
        EZI2C_EzI2CSetAddress1(I2C_Mem[I2C_ADDR_REG]);
        I2C_Mem[CONFIGURED_REG] = 0;
        I2C_OUT_EN_Write(1);
      }
      // Behavior loop. We'll cycle through this loop for each behavior that we
      // received from the director board, checking that behavior's mailbox to
      // see if any data has been received for that channel. We'll then do
      // something based on that behavior.
      for (i = 0; i < behaviorListLen; i++)
      {
        if (pending == 0) // if we aren't playing an uninterruptable sound
        {
          if (behaviors[i].active == 1) // snd is in process
          {
            if (behaviors[i].playing == 1) // and it's playing
            {
              // check for full delay has passed. If yes...
              if (behaviors[i].audioTimer + behaviors[i].predelay +
                  behaviors[i].postdelay < systemTimer)
              {
                behaviors[i].playing = 0;
                behaviors[i].active = 0;
              }
              else // full time delay has not passed yet
              {
              }
            }
            else // sound is in process but NOT playing
            {
              // ...check for predelay has passed and if it has...
              if (behaviors[i].audioTimer + behaviors[i].predelay < systemTimer)
              {
                // ...start playing the sound.
                behaviors[i].playing = 1;
                playSound(behaviors[i].soundIndex);
              }
              else // sound is in process, not ready to play yet
              {
              }
            }
          }
          else // snd is NOT in process...should it be?
          {
            if (behaviors[i].threshold < mailboxes[behaviors[i].channel])
            {
              if (behaviors[i].interruptable == NO_INT)
              {
                pending = 1;
                behaviors[i].pending = 1;
              }
              behaviors[i].active = 1;
              behaviors[i].audioTimer = systemTimer;
            }
          }
        } // (pending == 0)
        else if (pending == 1) // We ARE playing an uninterruptable sound
        {
          if (behaviors[i].pending == 1) // and this is the sound
          {
            if (behaviors[i].playing == 1) // and the sound IS playing
            {
              // check for full delay has passed. If yes...
              if (behaviors[i].audioTimer + behaviors[i].predelay +
                  behaviors[i].postdelay < systemTimer)
              {
                // if yes, clear flags
                pending = 0;
                behaviors[i].pending = 0;
                behaviors[i].active = 0;
                behaviors[i].playing = 0;
              }
              else
              {
                // if no, pass
              }
            }
            else // if the sound is NOT playing...
            {
              // ...check for predelay has passed and if it has...
              if (behaviors[i].audioTimer + behaviors[i].predelay < systemTimer)
              {
                // ...start playing the sound.
                behaviors[i].playing = 1;
                playSound(behaviors[i].soundIndex);
              }
              else // if predelay hasn't passed yet, just pass.
              {
              }
            }
          }
          else
          {
          }
        }
      } // for (i = 0; i < behaviorListLen; i++)
    } // END 100Hz timer loop

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


CY_ISR(tickISR)
{
  systemTimer++;
}

/* [] END OF FILE */

