#include "microphone.h"
#include <driver/i2s.h>
#include <math.h>

#define I2S_PORT I2S_NUM_0

#define I2S_BCLK 4
#define I2S_WS   5
#define I2S_SD   6

static int32_t audioBuffer[AUDIO_BUFFER_SIZE];

bool microphoneBegin() {

    i2s_config_t config = {};

    config.mode = (i2s_mode_t)(
        I2S_MODE_MASTER | I2S_MODE_RX
    );

    config.sample_rate = SAMPLE_RATE;
    config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
    config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    config.communication_format = I2S_COMM_FORMAT_STAND_I2S;

    config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    config.dma_buf_count = 8;
    config.dma_buf_len = AUDIO_BUFFER_SIZE;

    config.use_apll = false;
    config.tx_desc_auto_clear = false;
    config.fixed_mclk = 0;

    i2s_pin_config_t pins = {};

    pins.bck_io_num = I2S_BCLK;
    pins.ws_io_num = I2S_WS;
    pins.data_out_num = I2S_PIN_NO_CHANGE;
    pins.data_in_num = I2S_SD;

    if (i2s_driver_install(
        I2S_PORT, &config, 0, NULL
    ) != ESP_OK) {
        return false;
    }

    if (i2s_set_pin(I2S_PORT, &pins) != ESP_OK) {
        i2s_driver_uninstall(I2S_PORT);
        return false;
    }

    i2s_zero_dma_buffer(I2S_PORT);

    return true;
}

void microphoneFlush() {
    i2s_zero_dma_buffer(I2S_PORT);
}

bool microphoneReadAudio(
    int32_t* buffer,
    size_t bufferSize,
    size_t* bytesRead
) {
    if (buffer == nullptr ||
        bytesRead == nullptr ||
        bufferSize == 0) {
        return false;
    }

    *bytesRead = 0;

    esp_err_t result = i2s_read(
        I2S_PORT,
        buffer,
        bufferSize,
        bytesRead,
        portMAX_DELAY
    );

    if (result != ESP_OK ||
        *bytesRead == 0 ||
        *bytesRead > bufferSize) {
        *bytesRead = 0;
        return false;
    }

    return true;
}

float microphoneReadLevel() {

    size_t bytesRead = 0;

    if (!microphoneReadAudio(
        audioBuffer,
        sizeof(audioBuffer),
        &bytesRead
    )) {
        return 0.0f;
    }

    size_t samples = bytesRead / sizeof(int32_t);

    double sum = 0;

    for (size_t i = 0; i < samples; i++) {

        int32_t sample = audioBuffer[i] >> 16;
        sum += (double)sample * sample;
    }

    if (samples == 0) {
        return 0.0f;
    }

    return sqrt(sum / samples);
}