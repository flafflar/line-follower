#include <stdio.h>
#include "pico/stdlib.h"

#include "mux.h"
#include "sensor.h"
#include "pid.h"
#include "motors.h"

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
    setup_motors();

	wait_usr();
	uint16_t light_cal[8];
	uint16_t min = sensor_calibrate(16384, light_cal);
	printf("Min: %d\n", min);

	wait_usr();
	uint16_t dark_cal[8];
	uint16_t max = sensor_calibrate(16384, dark_cal);
	printf("Max: %d\n", max);

    float center = 3.5;

	while (true) {
		uint16_t sensor[8];
		sensor_read(sensor);
		sensor_apply_calibration(sensor, light_cal, dark_cal, sensor);

        float c = sensor_calculate_center(sensor);
        if (c == -1) {
            center = c < 3.5 ? 0 : 7;
        } else {
            center = c;
        }

        float speed = pid(center - 3.5);
        drive_motors(speed);
	}
}
