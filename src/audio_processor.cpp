#include "audio_processor.h"

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

        // Extract the upper audio bits and scale to signed 16-bit PCM.
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