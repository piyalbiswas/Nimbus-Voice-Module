#ifndef VOICE_CAPTURE_H
#define VOICE_CAPTURE_H

#include <Arduino.h>

bool voiceCaptureBegin();

void voiceCaptureStop();

bool voiceCaptureRead(
    int16_t* output,
    size_t outputCapacity,
    size_t* samplesRead
);

bool voiceCaptureIsActive();

#endif