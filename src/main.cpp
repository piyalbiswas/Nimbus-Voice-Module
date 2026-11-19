#include <Arduino.h>
#include "microphone.h"
#include "voice_capture.h"
#include "audio_processor.h"

#define PCM_BUFFER_SIZE 256

// Temporary voice detection threshold.
// We will calibrate this using real microphone readings.
#define VOICE_THRESHOLD 500.0f

static int16_t pcmBuffer[PCM_BUFFER_SIZE];
static uint32_t lastAudioLogMs = 0;

bool recording = false;

void setup() {

    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("===== NIMBUS VOICE MODULE =====");

    Serial.println("Initializing microphone...");

    if (!microphoneBegin()) {

        Serial.println("Microphone initialization failed!");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("Microphone ready!");

    Serial.println();
    Serial.println("Commands:");
    Serial.println("Type 'r' to start recording");
    Serial.println("Type 's' to stop recording");
}

void loop() {

    // Read commands from Serial Monitor.
    if (Serial.available()) {

        char command = Serial.read();

        if (command == 'r' && !recording) {

            if (voiceCaptureBegin()) {

                recording = true;

                Serial.println("Recording started...");
            }
        }

        if (command == 's' && recording) {

            voiceCaptureStop();

            recording = false;

            Serial.println("Recording stopped.");
        }
    }

    // Capture and analyze audio while recording.
    if (recording && voiceCaptureIsActive()) {

        size_t samplesRead = 0;

        if (voiceCaptureRead(
            pcmBuffer,
            PCM_BUFFER_SIZE,
            &samplesRead
        )) {

            // Calculate audio energy.
            float audioLevel = calculateAudioRMS(
                pcmBuffer,
                samplesRead
            );

            // Detect voice using the RMS threshold.
            bool voiceDetected = detectVoice(
                pcmBuffer,
                samplesRead,
                VOICE_THRESHOLD
            );

            if (millis() - lastAudioLogMs >= 250) {

                Serial.print("Audio RMS: ");
                Serial.println(audioLevel);

                Serial.println(
                    voiceDetected ? "Voice detected!" : "No voice detected."
                );

                lastAudioLogMs = millis();
            }
        }
    }
}