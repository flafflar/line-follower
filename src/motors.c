#include "motors.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

const float MAX_SPEED = 65535;
const float TARGET_SPEED = 16000;

/**
 * Left motor PWM GPIO15
 * Left motor GND GPIO14
 * Right motor PWM GPIO17
 * Right motor GND GPIO16
*/

void setup_motors (){
	int slice15;
	int channel15;
	int slice17;
	int channel17;

	pwm_config config = pwm_get_default_config();

	gpio_init(14);
	gpio_set_dir(14, true);
	gpio_put(14, 0);

	gpio_init(15);
	gpio_set_dir(15, true);
	gpio_set_function(15, GPIO_FUNC_PWM);
	slice15 = pwm_gpio_to_slice_num(15);
	pwm_init(slice15, &config, true);
	channel15 = pwm_gpio_to_channel(15);
	pwm_set_chan_level(slice15, channel15, 0);

	gpio_init(16);
	gpio_set_dir(16, true);
	gpio_put(16, 0);

	gpio_init(17);
	gpio_set_dir(17, true);
	gpio_set_function(17, GPIO_FUNC_PWM);
	slice17 = pwm_gpio_to_slice_num(17);
	pwm_init(slice17, &config, true);
	channel17 = pwm_gpio_to_channel(17);
	pwm_set_chan_level(slice17, channel17, 0);
}

void drive_right_motor(uint16_t speed){
	int slice17 = pwm_gpio_to_slice_num(17);
	int channel17 = pwm_gpio_to_channel(17);
	pwm_set_chan_level(slice17, channel17, speed);
}

void drive_left_motor(uint16_t speed){
	int slice15 = pwm_gpio_to_slice_num(15);
	int channel15 = pwm_gpio_to_channel(15);
	pwm_set_chan_level(slice15, channel15, speed);
}

static float normalize_speed(float speed) {
    if (speed > MAX_SPEED) {
        return MAX_SPEED;
    } else if (speed < 0) {
        return 0;
    }
    return speed;
}

void drive_motors(int16_t speed){
    float left_speed = normalize_speed(TARGET_SPEED - speed);
    float right_speed = normalize_speed(TARGET_SPEED + speed);
    drive_left_motor(left_speed);
    drive_right_motor(right_speed);
}
