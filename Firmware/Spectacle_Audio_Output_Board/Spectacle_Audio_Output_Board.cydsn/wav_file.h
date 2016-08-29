#ifndef __wav_file_h__
#define __wav_file_h__

#include <stdint.h>

#define RIFFChunkID  0x46464952
#define WAVEFormat 0x45564157
#define SC1Fmt 0x20746d66
struct wavHeader
{
  uint32_t ChunkID; // ASCII "RIFF", little endian
  uint32_t ChunkSize; // This will be 36 + Subchunk2Size for a wav file
  uint32_t Format; // ASCII "WAVE", little endian
  // Subchunk1- meta information about the file.
  uint32_t SubChunk1ID; // ASCII "fmt ", little endian
  uint32_t SubChunk1Size; // For PCM. This is # of bytes following this.
  uint16_t AudioFormat; // Anything else isn't a wav file
  uint16_t NumChannels; // we only support mono
  uint32_t SampleRate;
  uint32_t ByteRate;
  uint16_t BlockAlign;
  uint16_t BitsPerSample;
  // Subchunk2- data.
  uint32_t SubChunk2ID;
  uint32_t SubChunk2Size;
  void *data;
};
  
#endif

