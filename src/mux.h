/* This file defines methods for controlling the 74HC4051N 8-channel analog
 * multiplexer.
 */
#include <stdint.h>

#define MUX_S0_PIN 12
#define MUX_S1_PIN 11
#define MUX_S2_PIN 10

#define MUX_OUT_PIN 26

/** Initializes all the pins related to the multiplexer. */
void mux_init(void);

/**
 * Reads the analog value of the specified channel of the multiplexer.
 *
 * @param channel A number between 0-7 indicating the channel to read.
 */
uint16_t mux_read(unsigned int channel);
