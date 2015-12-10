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
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>


class Sound{
public:
    void initOpenAL() {
        
        ALenum errorNum = alGetError();
        // initialise OpenAL
        ALCdevice* device = alcOpenDevice(NULL);
        ALCcontext* context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        
        errorNum = alGetError();
        
        alListener3f(AL_POSITION, 0, 0, 0);
        
        errorNum = alGetError();
        
        alListener3f(AL_VELOCITY, 0, 0, 0);
        
        errorNum = alGetError();
        
        ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
        
        alListenerfv(AL_ORIENTATION, listenerOri);
        
        errorNum = alGetError();
        
        alGenSources(1, &movSound);
        alGenSources(1, &spinSound);
        alGenSources(1, &fireSound);
        alGenSources(1, &scoreSound);
        alGenSources(1, &music);
        
        errorNum = alGetError();
        
        alSourcef(movSound, AL_PITCH, 1);
        alSourcef(spinSound, AL_PITCH, 1);
        alSourcef(fireSound, AL_PITCH, 1);
        alSourcef(scoreSound, AL_PITCH, 1);
        alSourcef(music, AL_PITCH, 1);
        
        alSourcef(movSound, AL_GAIN, 0.6);
        alSourcef(spinSound, AL_GAIN, 1);
        alSourcef(fireSound, AL_GAIN, 1);
        alSourcef(scoreSound, AL_GAIN, 0.6);
        alSourcef(music, AL_GAIN, 0.4);
        
        alSource3f(movSound, AL_POSITION, 0, 0, 0);
        alSource3f(spinSound, AL_POSITION, 0, 0, 0);
        alSource3f(fireSound, AL_POSITION, 0, 0, 0);
        alSource3f(scoreSound, AL_POSITION, 0, 0, 0);
        alSource3f(music, AL_POSITION, 0, 0, 0);
        
        alSource3f(movSound, AL_VELOCITY, 0, 0, 0);
        alSource3f(spinSound, AL_VELOCITY, 0, 0, 0);
        alSource3f(fireSound, AL_VELOCITY, 0, 0, 0);
        alSource3f(scoreSound, AL_VELOCITY, 0, 0, 0);
        alSource3f(music, AL_VELOCITY, 0, 0, 0);
        
        alSourcei(movSound, AL_LOOPING, AL_FALSE);
        alSourcei(spinSound, AL_LOOPING, AL_FALSE);
        alSourcei(fireSound, AL_LOOPING, AL_FALSE);
        alSourcei(scoreSound, AL_LOOPING, AL_FALSE);
        alSourcei(music, AL_LOOPING, AL_FALSE);
        
        errorNum = alGetError();
        
        openWAV("./wav/tank_moving.wav", &movSound);
        openWAV("./wav/moving.wav", &spinSound);
        openWAV("./wav/tank_fire.wav", &fireSound);
        openWAV("./wav/moving.wav", &scoreSound);
        openWAV("./wav/moving.wav", &music);
        
        printf("Error after alBufferData, before alSourcePlay: %x\n", alGetError());
        
        playMusic();
        
        errorNum = alGetError();
        if (errorNum != AL_NO_ERROR) {
            printf("Error starting playback: %x", errorNum);
        }
    }
    
    void openWAV(char* fileName, ALuint* dest) {
        alGetError();
        int errorNum = 0;
        
        ALuint buffer;
        alGenBuffers(1, &buffer);
        
        errorNum = alGetError();
        
        
        
        errorNum = alGetError();
        
        FILE* file;
        file = fopen(fileName, "r");
        if (file == NULL) {
            printf("File access error\n");
            exit(1);
        }
        
        //dumpWholeFile(file);
        
        char xbuffer[5];
        if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "RIFF") != 0)
            throw "Not a WAV file";
        
        file_read_int32_le(xbuffer, file);
        if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "WAVE") != 0)
            throw "Not a WAV file!";
        
        if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "fmt ") != 0)
            throw "Invalid WAV file!";
        
        printf("char size: %lu", sizeof(char));
        
        errorNum = alGetError();
        
        GetALErrorString(errorNum);
        GetALCErrorString(errorNum);
        
        file_read_int32_le(xbuffer, file);
        short audioFormat = file_read_int16_le(xbuffer, file);
        short channels = file_read_int16_le(xbuffer, file);
        int sampleRate = file_read_int32_le(xbuffer, file);
        int byteRate = file_read_int32_le(xbuffer, file);
        file_read_int16_le(xbuffer, file);
        short bitsPerSample = file_read_int16_le(xbuffer, file);
        
        //if (audioFormat != 16) {
        //	short extraParams = file_read_int16_le(xbuffer, file);
        //	file_ignore_bytes(file, extraParams);
        //}
        
        bool atData = false;
        
        while (!atData) {
            if (fread(xbuffer, sizeof(char), 4, file) != 4 || strcmp(xbuffer, "data") != 0)
                printf("Discarding fluff\n");
            else
                atData = true;
            //throw "Invalid WAV file";
        }
        
        int dataChunkSize = file_read_int32_le(xbuffer, file)+4;
        unsigned char* bufferData = file_allocate_and_read_bytes(file, (size_t)dataChunkSize);
        
        printf("Error before alBufferData: %x\n", alGetError());
        
        //	float duration = float(dataChunkSize)/byteRate;
        alBufferData(buffer, GetFormatFromInfo(channels, bitsPerSample), bufferData, dataChunkSize, sampleRate);
        //*dest = buffer;
        alSourcei(*dest, AL_BUFFER, buffer);
        errorNum = alGetError();
        free(bufferData);
        fclose(file);
    }
    
    void playMovSound() {
        ALint source_state;
        alGetSourcei(movSound, AL_SOURCE_STATE, &source_state);
        if (source_state == AL_PLAYING) return;
        alSourcePlay(movSound);

        printf("Sound is meant to be playing");
    }
    
    void playSpinSound() {
        ALint source_state;
        alGetSourcei(spinSound, AL_SOURCE_STATE, &source_state);
        if (source_state == AL_PLAYING) return;

        alSourcePlay(spinSound);
        
        printf("Sound is meant to be playing");
    }
    
    void playfireSound() {
        ALint source_state;
        alGetSourcei(fireSound, AL_SOURCE_STATE, &source_state);
        //if (source_state == AL_PLAYING) return;


        alSourcePlay(fireSound);
        // check for errors
        printf("Sound is meant to be playing");
    }
    
    void playScoreSound() {
        ALint source_state;
        alGetSourcei(scoreSound, AL_SOURCE_STATE, &source_state);

        if (source_state == AL_PLAYING) return;

        alSourcePlay(scoreSound);
        // check for errors
        
        printf("Sound is meant to be playing");
    }
    
    void playMusic() {
        ALint source_state;
        alGetSourcei(music, AL_SOURCE_STATE, &source_state);
        alSourcePlay(music);

        printf("Music is meant to be playing");
    }
    
    static void dumpWholeFile(FILE* file) {
        while (1) {
            printf("%c", fgetc(file));
        }
        printf("Done");
    }
    
    static inline ALenum GetFormatFromInfo(short channels, short bitsPerSample) {
        if (channels == 1) {
            if (bitsPerSample == 16) {
                return AL_FORMAT_MONO16;
            }
            return AL_FORMAT_MONO8;
        }
        if (bitsPerSample == 16) {
            return AL_FORMAT_STEREO16;
        }
        return AL_FORMAT_STEREO8;
    }
    
    static void file_ignore_bytes(FILE* file, short bytes) {
        while (bytes > 0) {
            printf("%c", (int)fgetc(file));
            bytes -= 1;
        }
    }
    
    static unsigned char* file_allocate_and_read_bytes(FILE* file, size_t chunkSize) {
        unsigned char* bytes = (unsigned char*)malloc(chunkSize);
        size_t bytesRead = fread(bytes, 1, chunkSize, file);
        printf("bytes read: %i\n", bytesRead);
        printf("Contents: ");
        for (int i=0; i<sizeof(*bytes); i++) {
            printf("%c", bytes[i]);
        }
        printf("\n");
        return bytes;
    }
    
    static unsigned int file_read_int32_le(char* buffer, FILE* file) {
        fread(buffer, 1, 4, file);
        printf("Buffer contents: ");
        for (int i=0; i<sizeof(buffer)/sizeof(char); i++) {
            printf("%c", (char)buffer[i]);
        }
        printf("\n");
        unsigned int val = *(unsigned int*)buffer;
        return CFSwapInt32LittleToHost(val);
    }
    
    static short file_read_int16_le(char* buffer, FILE* file) {
        fread(buffer, 1, 2, file);
        printf("Buffer contents: ");
        for (int i=0; i<sizeof(buffer)/sizeof(char); i++) {
            printf("%c", (char)buffer[i]);
        }
        printf("\n");
        unsigned int val = *(unsigned int*)buffer;
        return CFSwapInt32LittleToHost(val);	
    }
    
    template <typename T> T byte_reverse(T in) {
        T out;
        char* in_c = reinterpret_cast<char *> (&in);
        char* out_c = reinterpret_cast<char *> (&out);
        std::reverse_copy(in_c, in_c+sizeof(T), out_c);
        return out;
    }
    
    void muteMusic() {
        alSourcef(movSound, AL_GAIN, 0.0);
        
    }
    

    void unmuteMusic() {
        alSourcef(movSound, AL_GAIN, 1);
    }
    
    void GetALErrorString(ALenum err) {
        switch(err) {
            case AL_NO_ERROR:
                printf("AL_NO_ERROR\n");
                break;
                
            case AL_INVALID_NAME:
                printf("AL_INVALID_NAME\n");
                break;
                
            case AL_INVALID_ENUM:
                printf("AL_INVALID_ENUM\n");
                break;
                
            case AL_INVALID_VALUE:
                printf("AL_INVALID_VALUE\n");
                break;
                
            case AL_INVALID_OPERATION:
                printf("AL_INVALID_OPERATION\n");
                break;
                
            case AL_OUT_OF_MEMORY:
                printf("AL_OUT_OF_MEMORY\n");
                break;
            default:
                printf("Unknown error\n");
        };
    }
    
    void GetALCErrorString(ALenum err) {
        switch(err)
        {
            case ALC_NO_ERROR:
                printf("AL_NO_ERROR\n");
                break;
                
            case ALC_INVALID_DEVICE:
                printf("ALC_INVALID_DEVICE\n");
                break;
                
            case ALC_INVALID_CONTEXT:
                printf("ALC_INVALID_CONTEXT\n");
                break;
                
            case ALC_INVALID_ENUM:
                printf("ALC_INVALID_ENUM\n");
                break;
                
            case ALC_INVALID_VALUE:
                printf("ALC_INVALID_VALUE\n");
                break;
                
            case ALC_OUT_OF_MEMORY:
                printf("ALC_OUT_OF_MEMORY\n");
                break;
            default:
                printf("Unknown error\n");
        };
    }
    ALuint movSound;
    ALuint spinSound;
    ALuint fireSound;
    ALuint scoreSound;
    ALuint music;

};


#endif /* Sound_hpp */
