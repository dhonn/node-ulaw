// encode.c
#include <stdint.h>
#include <stddef.h>
#include <math.h>

// Constants for μ-law encoding
const int BIAS = 0x84;      // Bias for μ-law (132)
const int CLIP = 32635;     // Maximum PCM value to avoid overflow

// μ-law encoding table
static const uint8_t ulaw_encoding_table[256] = {
    0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

// Function to encode 16-bit PCM to 8-bit μ-law
uint8_t pcm_to_ulaw(int16_t pcm_val) {

    int sign, exponent, mantissa;
    uint8_t ulaw;

    // Get the sign bit (0 for positive, 1 for negative)
    sign = (pcm_val >> 8) & 0x80;

    // Convert to absolute value
    if (sign) pcm_val = -pcm_val;

    // Clip the input to avoid overflow
    if (pcm_val > CLIP) pcm_val = CLIP;

    // Add bias
    pcm_val += BIAS;

    // Use the table to find the exponent
    exponent = ulaw_encoding_table[(pcm_val>>7) & 0xFF];

    // Calculate mantissa (4 bits)
    mantissa = (pcm_val >> (exponent + 3)) & 0x0F;

    // Combine sign, exponent, and mantissa
    ulaw = ~(sign | (exponent << 4) | mantissa);

    return ulaw;
}

void encode_samples(int16_t* pcm_buffer, uint8_t* ulaw_buffer, int length) {
    for (int i = 0; i < length; i++) {
        ulaw_buffer[i] = pcm_to_ulaw(pcm_buffer[i]);
    }
}