#include <stdio.h>
#include "pico/stdlib.h"

#include "mux.h"
#include "sensor.h"

/**
 * Waits for the USR switch on pin 24 to be pressed.
 */
void wait_usr() {
	gpio_init(24);
	gpio_set_dir(24, GPIO_IN);
	gpio_pull_up(24);
	while (!gpio_get(24));
	sleep_ms(10);
	while (gpio_get(24));
}

int main(void) {
	stdio_init_all();

	mux_init();

	wait_usr();
	uint16_t light_cal[8];
	uint16_t min = sensor_calibrate(256, light_cal);
	printf("Min: %d\n", min);

	wait_usr();
	uint16_t dark_cal[8];
	uint16_t max = sensor_calibrate(256, dark_cal);
	printf("Max: %d\n", max);

	while (true) {
		uint16_t sensor[8];
		sensor_read(sensor);
		sensor_apply_calibration(sensor, light_cal, dark_cal, sensor);

		for (int s = 0; s < 8; s++) {
			printf("%05d ", sensor[s]);
		}
		printf("\r");

		sleep_ms(100);
	}
}
