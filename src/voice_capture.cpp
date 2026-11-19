#include "voice_capture.h"
#include "microphone.h"
#include "audio_processor.h"

#define RAW_BUFFER_SAMPLES 256

static int32_t rawBuffer[RAW_BUFFER_SAMPLES];
static bool captureActive = false;

bool voiceCaptureBegin() {
    microphoneFlush();
    captureActive = true;
    return true;
}

void voiceCaptureStop() {
    captureActive = false;
}

bool voiceCaptureIsActive() {
    return captureActive;
}

bool voiceCaptureRead(
    int16_t* output,
    size_t outputCapacity,
    size_t* samplesRead
) {
    if (!captureActive ||
        output == nullptr ||
        samplesRead == nullptr) {
        return false;
    }

    *samplesRead = 0;

    size_t bytesRead = 0;

    if (!microphoneReadAudio(
        rawBuffer,
        sizeof(rawBuffer),
        &bytesRead
    )) {
        return false;
    }

    size_t rawSamples = bytesRead / sizeof(int32_t);

    *samplesRead = convertToPCM16(
        rawBuffer,
        rawSamples,
        output,
        outputCapacity
    );

    return *samplesRead > 0;
}