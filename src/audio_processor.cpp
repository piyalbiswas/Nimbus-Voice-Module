#include "audio_processor.h"
#include <math.h>


// Convert 32-bit I2S samples into 16-bit PCM samples.
size_t convertToPCM16(
    const int32_t* input,
    size_t inputSamples,
    int16_t* output,
    size_t outputCapacity
) {
    if (input == nullptr || output == nullptr) {
        return 0;
    }

    size_t samplesToConvert = inputSamples;

    if (samplesToConvert > outputCapacity) {
        samplesToConvert = outputCapacity;
    }

    for (size_t i = 0; i < samplesToConvert; i++) {

        // Extract audio bits and scale to signed 16-bit PCM.
        int32_t sample = input[i] >> 14;

        // Prevent clipping.
        if (sample > 32767) {
            sample = 32767;
        }

        if (sample < -32768) {
            sample = -32768;
        }

        output[i] = (int16_t)sample;
    }

    return samplesToConvert;
}


// Calculate RMS audio level.
float calculateAudioRMS(
    const int16_t* samples,
    size_t sampleCount
) {
    if (samples == nullptr || sampleCount == 0) {
        return 0.0f;
    }

    double sum = 0.0;

    for (size_t i = 0; i < sampleCount; i++) {

        double sample = samples[i];

        sum += sample * sample;
    }

    return sqrt(sum / sampleCount);
}


// Detect voice using an energy threshold.
bool detectVoice(
    const int16_t* samples,
    size_t sampleCount,
    float threshold
) {
    if (samples == nullptr || sampleCount == 0) {
        return false;
    }

    float audioLevel = calculateAudioRMS(
        samples,
        sampleCount
    );

    return audioLevel >= threshold;
}