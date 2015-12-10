//
//  Sound.cpp
//  TankBattle
//
//  Created by clyde on 15/12/9.
//  Copyright © 2015年 SUTeam. All rights reserved.
//

#include "Sound.h"


static void list_audio_devices(const ALCchar *devices)
{
    const ALCchar *device = devices, *next = devices + 1;
    size_t len = 0;
    
    fprintf(stdout, "Devices list:\n");
    fprintf(stdout, "----------\n");
    while (device && *device != '\0' && next && *next != '\0') {
        fprintf(stdout, "%s\n", device);
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    fprintf(stdout, "----------\n");
}

#define TEST_ERROR(_msg)		\
error = alGetError();		\
if (error != AL_NO_ERROR) {	\
fprintf(stderr, _msg "\n");	\
return -1;		\
}

static inline ALenum to_al_format(short channels, short samples)
{
    bool stereo = (channels > 1);
    
    switch (samples) {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
    }
}

int Sound::init(std::string filename)
{
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    
    
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
        fprintf(stderr, "enumeration extension not available\n");
    
    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    
    if (!defaultDeviceName)
        defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    
    device = alcOpenDevice(defaultDeviceName);
    if (!device) {
        fprintf(stderr, "unable to open default device\n");
        return -1;
    }
    
    fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));
    
    alGetError();
    
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context)) {
        fprintf(stderr, "failed to make default context\n");
        return -1;
    }
    TEST_ERROR("make default context");
    
    /* set orientation */
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    TEST_ERROR("listener position");
    alListener3f(AL_VELOCITY, 0, 0, 0);
    TEST_ERROR("listener velocity");
    alListenerfv(AL_ORIENTATION, listenerOri);
    TEST_ERROR("listener orientation");
    
    alGenSources((ALuint)1, &source);
    TEST_ERROR("source generation");
    
    alSourcef(source, AL_PITCH, 1);
    TEST_ERROR("source pitch");
    alSourcef(source, AL_GAIN, 1);
    TEST_ERROR("source gain");
    alSource3f(source, AL_POSITION, 0, 0, 0);
    TEST_ERROR("source position");
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    TEST_ERROR("source velocity");
    alSourcei(source, AL_LOOPING, AL_FALSE);
    TEST_ERROR("source looping");
    
    Sound::loadWavFile(filename, &buffer, &size, &freq, &format);
    //TEST_ERROR("loading wav file");
    
    alSourcei(source, AL_BUFFER, buffer);
    TEST_ERROR("buffer binding");
    
    alSourcePlay(source);
    TEST_ERROR("source playing");
    
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    TEST_ERROR("source state get");
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        std::cout<<"playing";
        TEST_ERROR("source state get");
    }
    
    /* exit context */
//    alDeleteSources(1, &source);
//    alDeleteBuffers(1, &buffer);
//    device = alcGetContextsDevice(context);
//    alcMakeContextCurrent(NULL);
//    alcDestroyContext(context);
//    alcCloseDevice(device);
    
    return 0;
}

bool Sound::loadWavFile(const std::string filename, ALuint* buffer,
                 ALsizei* size, ALsizei* frequency,
                 ALenum* format) {
    //Local Declarations
    FILE* soundFile = NULL;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;
    unsigned char* data;
    unsigned char d;
    try {
        soundFile = fopen(filename.c_str(), "rb");
        if (!soundFile)
            throw (filename);
        for (int i = 0; i< 36; i++)
        {
        fread(&d, sizeof(d), 1, soundFile);
            //std::cout<< d<<" "<< i<< std::endl;
        }

        // Read in the first chunk into the struct
        //fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);
        /*
        //check for RIFF and WAVE tag in memeory
        if ((riff_header.chunkID[0] != 'R' ||
             riff_header.chunkID[1] != 'I' ||
             riff_header.chunkID[2] != 'F' ||
             riff_header.chunkID[3] != 'F') ||
            (riff_header.format[0] != 'W' ||
             riff_header.format[1] != 'A' ||
             riff_header.format[2] != 'V' ||
             riff_header.format[3] != 'E'))
            //throw ("Invalid RIFF or WAVE Header");
        
        //Read in the 2nd chunk for the wave info
        fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
        //check for fmt tag in memory
        if (wave_format.subChunkID[0] != 'f' ||
            wave_format.subChunkID[1] != 'm' ||
            wave_format.subChunkID[2] != 't' ||
            wave_format.subChunkID[3] != ' ')
            throw ("Invalid Wave Format");
        
        //check for extra parameters;
        if (wave_format.subChunkSize > 16)
            fseek(soundFile, sizeof(short), SEEK_CUR);
        */
        //Read in the the last byte of data before the sound file
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        //check for data tag in memory
        if (wave_data.subChunkID[0] != 'd' ||
            wave_data.subChunkID[1] != 'a' ||
            wave_data.subChunkID[2] != 't' ||
            wave_data.subChunkID[3] != 'a')
        {
            throw ("Invalid data header");
        }
        
        //Allocate memory for data
        data = new unsigned char[wave_data.subChunk2Size];
        
        // Read in the sound data into the soundData variable
        if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
            throw ("error loading WAVE data into struct!");
        
        //Now we set the variables that we passed in with the
        //data from the structs
        *size = wave_data.subChunk2Size;
        *frequency = wave_format.sampleRate;
        //The format is worked out by looking at the number of
        //channels and the bits per sample.
        if (wave_format.numChannels == 1) {
            if (wave_format.bitsPerSample == 8 )
                *format = AL_FORMAT_MONO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_MONO16;
        } else if (wave_format.numChannels == 2) {
            if (wave_format.bitsPerSample == 8 )
                *format = AL_FORMAT_STEREO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_STEREO16;
        }
        //create our openAL buffer and check for success
        alGenBuffers(1, buffer);
        //now we put our data into the openAL buffer and
        //check for success
        alBufferData(*buffer, *format, (void*)data,
                     *size, *frequency);
        //clean up and return true if successful
        fclose(soundFile);
        return true;
    } catch(std::string error) {
        //our catch statement for if we throw a string
        std::cerr << error << " : trying to load "
        << filename << std::endl;
        //clean up memory if wave loading fails
        if (soundFile != NULL)
            fclose(soundFile);
        //return false to indicate the failure to load wave
        return false;
    }
}
void Sound::play(){
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
    // check for errors
    
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    // check for errors
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        // check for errors
    }
};