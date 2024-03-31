#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "mux.h"

int main(void) {
	stdio_init_all();

	mux_init();
	
	int channel = 0;
	while (true) {
		int c = getchar_timeout_us(0);
		if (c != PICO_ERROR_TIMEOUT) {
			channel = (c - '0') % 8;
		}
		
		uint16_t value = mux_read(channel);
		printf("Channel %d: %04d \r", channel, value);
	}
}
