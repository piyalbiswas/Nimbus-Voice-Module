#include <Arduino.h>
#include "microphone.h"
#include "voice_capture.h"

#define PCM_BUFFER_SIZE 256

static int16_t pcmBuffer[PCM_BUFFER_SIZE];

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

    // Capture audio while recording.
    if (recording && voiceCaptureIsActive()) {

        size_t samplesRead = 0;

        if (voiceCaptureRead(
            pcmBuffer,
            PCM_BUFFER_SIZE,
            &samplesRead
        )) {
            Serial.print("Captured PCM samples: ");
            Serial.println(samplesRead);
        }
    }
}