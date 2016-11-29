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

