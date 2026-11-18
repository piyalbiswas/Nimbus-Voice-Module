#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <Arduino.h>

// Convert 32-bit I2S samples into 16-bit PCM samples.
size_t convertToPCM16(
    const int32_t* input,
    size_t inputSamples,
    int16_t* output,
    size_t outputCapacity
);

#endif