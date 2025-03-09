#ifndef INPUT_AUDIO_CPP
#define INPUT_AUDIO_CPP

#include "miniaudio.h"
#include <cstdint>

// ===========================================================
// Input Audio Shenanignans
// ===========================================================

#define AUDIO_BUFFER_SIZE 1024

float audioBuffer[AUDIO_BUFFER_SIZE];

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, uint32_t frameCount)
{
    const float* inputSamples = (const float*)pInput;

    // Copy the input audio data into the audioBuffer
    for (uint32_t i = 0; i < frameCount; i++) {
        audioBuffer[i] = inputSamples[i];
    }
}
float calculate_loud() {
    float sum = 0.0f;

    // Calculate the square of each sample and sum them
    for (int i = 0; i < 1024; i++) {
        sum += audioBuffer[i] * audioBuffer[i];
    }

    // Return the RMS (root mean square) of the audio samples as the loudness
    return sqrtf(sum / 1024.0f);
}

#endif // INPUT_AUDIO_CPP