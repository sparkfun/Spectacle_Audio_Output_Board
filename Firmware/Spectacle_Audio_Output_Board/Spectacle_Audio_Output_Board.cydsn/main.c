#include <Project.h>
#include <FS.h>
#include <string.h>
#include <Global.h>
#include <stdio.h>
#include <wav_file.h>
#include <stdbool.h>

CY_ISR_PROTO(Audio_Output_Interrupt);

volatile int32_t AudioBuffer[2][2048];
volatile int32_t *ABPointer;
volatile uint8_t emptyBufferIndex = 0;
volatile bool emptyBufferAvailable = true;
volatile uint32_t dataLeftToWrite = 0;
volatile bool dataBuffered = false;


int main()
{
    CyGlobalIntEnable;

    UART_Start();
    PWM_Start();
    Audio_Interrupt_StartEx(Audio_Output_Interrupt);
    ABPointer = &AudioBuffer[0][0];
    
    while (Yellow_Read() != 0);
    
    UART_PutString("Thundercats are GO!\n");
    
    FS_Init();

    FS_FILE * pFile;
    struct wavHeader fileInfo;
    char tempStr[64];
    
    pFile = FS_FOpen("01.wav", "rb");
    
    if (pFile)
    {
      UART_PutString("File okay!\n");

      FS_FRead(&fileInfo, 44, 1, pFile);
      sprintf(tempStr, "File type %lx\n", fileInfo.ChunkID);
      UART_PutString(tempStr);
      
      sprintf(tempStr, "File size %ld\n", fileInfo.ChunkSize);
      UART_PutString(tempStr);

      sprintf(tempStr, "Format %lx\n", fileInfo.Format);
      UART_PutString(tempStr);

      sprintf(tempStr, "SubChunk1ID %lx\n", fileInfo.SubChunk1ID);
      UART_PutString(tempStr);

      sprintf(tempStr, "SubChunk1Size %ld\n", fileInfo.SubChunk1Size);
      UART_PutString(tempStr);

      sprintf(tempStr, "AudioFormat %d\n", fileInfo.AudioFormat);
      UART_PutString(tempStr);

      sprintf(tempStr, "NumChannels %d\n", fileInfo.NumChannels);
      UART_PutString(tempStr);

      sprintf(tempStr, "SampleRate %ld\n", fileInfo.SampleRate);
      UART_PutString(tempStr);

      sprintf(tempStr, "ByteRate %ld\n", fileInfo.ByteRate);
      UART_PutString(tempStr);

      sprintf(tempStr, "BlockAlign %d\n", fileInfo.BlockAlign);
      UART_PutString(tempStr);

      sprintf(tempStr, "BitsPerSample %d\n", fileInfo.BitsPerSample);
      UART_PutString(tempStr);

      sprintf(tempStr, "SubChunk2ID %lx\n", fileInfo.SubChunk2ID);
      UART_PutString(tempStr);

      sprintf(tempStr, "SubChunk2Size %ld\n", fileInfo.SubChunk2Size);
      UART_PutString(tempStr);
    }

    uint32_t dataLeftOnCard = fileInfo.SubChunk2Size;
    dataLeftToWrite = fileInfo.SubChunk2Size;
    I2S_Start();
    for(;;)
    {
      do 
      {
        // There are two buffers, each 8192 bytes in length. If one of them is
        //  empty, we want to fill it from the SD card. The data on the SD card
        //  (should be) 24-bit PCM audio, and the buffers are 32-bit integers.
        //  
        if (emptyBufferAvailable)
        {
          emptyBufferAvailable = false;
          sprintf(tempStr, "%ld\n", dataLeftOnCard);
          UART_PutString(tempStr);
          // if there's not enough data on the card to fill the buffer, just
          //  pull the rest of the data off the card.
          if ((dataLeftOnCard <= 8192) && (dataLeftOnCard > 0))
          {
            FS_FRead(&AudioBuffer[emptyBufferIndex][0], 2, dataLeftOnCard/2, pFile);
            dataLeftOnCard = 0;
          }
          // Otherwise, pull 8k of data off the card into the buffer.
          else
          {
            FS_FRead(&AudioBuffer[emptyBufferIndex][0], 2, 4096, pFile);
            dataLeftOnCard -= 8192;
          }
        }
        /*if (Audio_Timer_initVar == 0)
        {
          Audio_Timer_Start();
        }*/
      } while (dataLeftOnCard > 0);
      UART_PutString("Done!\n");
      while(1);

    }
}

CY_ISR(Audio_Output_Interrupt)
{
  Audio_Timer_STATUS;  // Clear this flag so the interrupt can be exited.
  if (dataLeftToWrite > 0)
  {
    uint16_t audioData = (*ABPointer)>>4;
    ABPointer+=1;
    if (audioData > 4080) audioData = 4080;
    PWM_WriteCompare(audioData);
    dataLeftToWrite -= 2;
    if (ABPointer >= ((&AudioBuffer[0][0]) + 8192))
    {
      ABPointer = &AudioBuffer[0][0];
      emptyBufferAvailable = true;
      emptyBufferIndex = 1;
    }
    if (ABPointer == (&AudioBuffer[1][0]))
    {
      emptyBufferAvailable = true;
      emptyBufferIndex = 0;
    }
  }
}

/* [] END OF FILE */

