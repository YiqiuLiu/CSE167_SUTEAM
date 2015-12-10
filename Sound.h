//
//  Sound.hpp
//  TankBattle
//
//  Created by clyde on 15/12/9.
//  Copyright © 2015年 SUTeam. All rights reserved.
//

#ifndef Sound_hpp
#define Sound_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <iostream>
#include <stdbool.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

/*
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */
struct RIFF_Header {
    char chunkID[4];
    int chunkSize;//size not including chunkSize or chunkID
    char format[4];
};
/*
 * Struct to hold fmt subchunk data for WAVE files.
 */
struct WAVE_Format {
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
    /*
     * Struct to hold the data of the wave file
     */
};
struct WAVE_Data {
    char subChunkID[4]; //should contain the word data
    int subChunk2Size; //Stores the size of the data block
};

class Sound{
public:
    ALint source_state;
    ALboolean enumeration;
    const ALCchar *devices;
    const ALCchar *defaultDeviceName = nullptr;
    int ret;
    char *bufferData;
    ALCdevice *device;
    ALvoid *data;
    ALCcontext *context;
    ALsizei size, freq;
    ALenum format;
    ALuint buffer, source;
    ALboolean loop = AL_FALSE;
    ALCenum error;
    void play();
    bool loadWavFile(const std::string filename, ALuint* buffer,
                ALsizei* size, ALsizei* frequency,
                ALenum* format);
    int init(std::string filename);
};


#endif /* Sound_hpp */
