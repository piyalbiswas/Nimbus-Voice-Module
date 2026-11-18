# Nimbus Voice Module

Firmware component for the Nimbus ESP32-S3 voice-enabled study companion.

## Current features

- INMP441 I2S microphone interface
- 16 kHz audio sampling configuration
- 32-bit I2S to 16-bit PCM conversion
- Voice capture control
- Serial-based recording start and stop commands

## Source files

| File | Purpose |
|---|---|
| main.cpp | Main program |
| microphone.h | Microphone declarations |
| microphone.cpp | I2S microphone handling |
| audio_processor.h | Audio processing declarations |
| audio_processor.cpp | PCM conversion |
| voice_capture.h | Voice capture declarations |
| voice_capture.cpp | Audio capture management |

## Build environment

- PlatformIO
- Arduino framework
- ESP32-S3
- C++

## Current limitations

- GPIO assignments are provisional.
- Microphone operation has not yet been tested on physical hardware.
- PCM conversion requires validation with actual INMP441 samples.
- Backend/API integration is not included.
- Serial commands currently simulate push-to-talk.

## Build

Open the project in VS Code with PlatformIO and run Build.