#include "motors.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include "defines.h"

/**
 * Left motor PWM GPIO15
 * Left motor GND GPIO14
 * Right motor PWM GPIO17
 * Right motor GND GPIO16
*/

void setup_motors (){
	int slice14;
	int channel14;
	int slice15;
	int channel15;
	int slice16;
	int channel16;
	int slice17;
	int channel17;

	pwm_config config = pwm_get_default_config();

	gpio_init(14);
	gpio_set_dir(14, true);
	gpio_set_function(14, GPIO_FUNC_PWM);
	slice14 = pwm_gpio_to_slice_num(14);
	pwm_init(slice14, &config, true);
	channel14 = pwm_gpio_to_channel(14);
	pwm_set_chan_level(slice14, channel14, 0);

	gpio_init(15);
	gpio_set_dir(15, true);
	gpio_set_function(15, GPIO_FUNC_PWM);
	slice15 = pwm_gpio_to_slice_num(15);
	pwm_init(slice15, &config, true);
	channel15 = pwm_gpio_to_channel(15);
	pwm_set_chan_level(slice15, channel15, 0);

	gpio_init(16);
	gpio_set_dir(16, true);
	gpio_set_function(16, GPIO_FUNC_PWM);
	slice16 = pwm_gpio_to_slice_num(16);
	pwm_init(slice16, &config, true);
	channel16 = pwm_gpio_to_channel(16);
	pwm_set_chan_level(slice16, channel16, 0);

	gpio_init(17);
	gpio_set_dir(17, true);
	gpio_set_function(17, GPIO_FUNC_PWM);
	slice17 = pwm_gpio_to_slice_num(17);
	pwm_init(slice17, &config, true);
	channel17 = pwm_gpio_to_channel(17);
	pwm_set_chan_level(slice17, channel17, 0);
}

void drive_right_motor(double speed){
	if (speed > MAX_SPEED){
		speed = MAX_SPEED;
	}
	else if (speed < 0){
		if (-speed > MAX_REVERSE_SPEED){
			speed = -MAX_REVERSE_SPEED;
		}
		int slice16 = pwm_gpio_to_slice_num(16);
		int channel16 = pwm_gpio_to_channel(16);
		pwm_set_chan_level(slice16, channel16, -(int)speed);
		int slice17 = pwm_gpio_to_slice_num(17);
		int channel17 = pwm_gpio_to_channel(17);
		pwm_set_chan_level(slice17, channel17, 0);
		return;
	}
	int slice16 = pwm_gpio_to_slice_num(16);
	int channel16 = pwm_gpio_to_channel(16);
	pwm_set_chan_level(slice16, channel16, 0);
	int slice17 = pwm_gpio_to_slice_num(17);
	int channel17 = pwm_gpio_to_channel(17);
	pwm_set_chan_level(slice17, channel17, (int)speed);
}

void drive_left_motor(double speed){
	if (speed > MAX_SPEED){
		speed = MAX_SPEED;
	}
	else if (speed < 0){
		if (-speed > MAX_REVERSE_SPEED){
			speed = -MAX_REVERSE_SPEED;
		}
		int slice14 = pwm_gpio_to_slice_num(14);
		int channel14 = pwm_gpio_to_channel(14);
		pwm_set_chan_level(slice14, channel14, -(int)speed);
		int slice15 = pwm_gpio_to_slice_num(15);
		int channel15 = pwm_gpio_to_channel(15);
		pwm_set_chan_level(slice15, channel15, 0);
		return;
	}
	int slice14 = pwm_gpio_to_slice_num(14);
	int channel14 = pwm_gpio_to_channel(14);
	pwm_set_chan_level(slice14, channel14, 0);
	int slice15 = pwm_gpio_to_slice_num(15);
	int channel15 = pwm_gpio_to_channel(15);
	pwm_set_chan_level(slice15, channel15, (int)speed);
}

void drive_motors(float speed){
	float left_speed = TARGET_SPEED - speed;
	float right_speed = TARGET_SPEED + speed;
	drive_left_motor((double)left_speed);
	drive_right_motor((double)right_speed);
}
