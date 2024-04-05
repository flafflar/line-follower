#include <stdio.h>
#include "pico/stdlib.h"

#include "mux.h"
#include "sensor.h"

int main(void) {
	stdio_init_all();

	mux_init();

	while (true) {
		uint16_t sensor[8];
		sensor_read(sensor);

		for (int s = 0; s < 8; s++) {
			printf("%05d ", sensor[s]);
		}
		printf("\r");

		sleep_ms(1000);
	}
}
