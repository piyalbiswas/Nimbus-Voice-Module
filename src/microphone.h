#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>

#define SAMPLE_RATE 16000
#define AUDIO_BUFFER_SIZE 256

bool microphoneBegin();

float microphoneReadLevel();

bool microphoneReadAudio(
    int32_t* buffer,
    size_t bufferSize,
    size_t* bytesRead
);

#endif