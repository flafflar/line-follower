#include "mux.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

static void mux_init_pin(int gpio) {
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_OUT);
}

void mux_init(void) {
	mux_init_pin(MUX_S0_PIN);
	mux_init_pin(MUX_S1_PIN);
	mux_init_pin(MUX_S2_PIN);

	adc_init();
	adc_gpio_init(MUX_OUT_PIN);
	adc_select_input(MUX_OUT_PIN - 26);
}

uint16_t mux_read(unsigned int channel) {
	// Extract the last 3 bits from the channel number
	int s0 = channel & 1;
	int s1 = (channel >> 1) & 1;
	int s2 = (channel >> 2) & 1;

	// Write the bits to the respective pins
	// The chip uses negative logic for these pins, so the values must be
	// inverted
	gpio_put(MUX_S0_PIN, s0);
	gpio_put(MUX_S1_PIN, s1);
	gpio_put(MUX_S2_PIN, s2);

	// The propagation delay of the chip is at most 1us, so wait for that
	// time before reading
	sleep_us(1);

	// Read the analog value from the common output
	return adc_read();
}
