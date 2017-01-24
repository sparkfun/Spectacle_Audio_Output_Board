/****************************************************************************
audio.h
Declarations for audio behavioral struct
Mike Hord @ SparkFun Electronics
24 Jan 2017
https://github.com/sparkfun/Spectacle_Audio_Output_Board

This file contains the main behavioral struct for the audio board. Every
behavior the board is taught by the director boards will have one instance
of this struct, and its size is 1:1 with the data stored by the director.

Development environment specifics:
Developed in PSoC Creator 4.0
Board V10

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
****************************************************************************/
    
#ifndef __audio_h__
#define __audio_h__
#include "project.h"
#pragma pack(1)
struct audio
{
  uint8 channel;
  int16 threshold;
  uint8 soundIndex;
  uint8 interruptable;
  uint8 pending;
  int32 predelay;
  int32 postdelay;
  int32 audioTimer;
  uint8 active;
  uint8 playing;
};
#pragma pack()

enum {YES_INT, NO_INT};

struct audio audioInit(uint8 channel, int16 threshold, uint8 soundIndex,
                       uint8 interruptable, uint8 pending, int32 predelay,
                       int32 postdelay, int32 audioTimer);
void playSound(uint8 soundIndex);

#endif

