#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "mux.h"
#include "sensor.h"
#include "pid.h"
#include "motors.h"
#include "defines.h"

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

uint16_t light_cal[8];
uint16_t dark_cal[8];
uint16_t sensor_array_1[8];
uint16_t sensor_array_2[8];
volatile uint16_t *current_sensor_array;

void core1_main() {
    // Initialize the sensor array pointer
    current_sensor_array = sensor_array_1;

    while (true) {
        // Read and calibrate sensors
        uint16_t* next_sensor_array = (current_sensor_array == sensor_array_1) ? sensor_array_2 : sensor_array_1;
        sensor_read(next_sensor_array);
        sensor_apply_calibration(next_sensor_array, light_cal, dark_cal, next_sensor_array);

        // Switch the sensor array pointer
        current_sensor_array = next_sensor_array;
    }
}


int main(void) {
	stdio_init_all();

    mux_init();
    setup_motors();

    // Perform calibration on core 0
    if (USE_CALIBRATION) {
        wait_usr();
        uint16_t min = sensor_calibrate(16384, light_cal);
        printf("Min: %d\n", min);
        wait_usr();
        uint16_t max = sensor_calibrate(16384, dark_cal);
        printf("Max: %d\n", max);
    } else {
        //TODO: Implement default calibration
		return 1;
    }

	for (int i = 0; i < 8; i++) {
        printf("Light: %d, Dark: %d\n", light_cal[i], dark_cal[i]);
    }

	multicore_launch_core1(core1_main);
	wait_usr();

	while (true) {
        uint16_t sensor[8];
        sensor[0] = current_sensor_array[0];
        sensor[4] = current_sensor_array[4];
		sensor[7] = current_sensor_array[7];

	    if (sensor[0] > BLACK_THRESHOLD && sensor[4] > BLACK_THRESHOLD && sensor[7] > BLACK_THRESHOLD) {
		    drive_left_motor(0);
		    drive_right_motor(0);
		    break;
	    }

        float speed = pid(sensor);
        drive_motors(speed);
    }
}

