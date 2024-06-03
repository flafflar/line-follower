#include <stdio.h>
#include <stdint.h>
#include "defines.h"

float pid(uint16_t values[8]) {
    /*
    A0 A1 -- -- A4 A5 A6 A7
    A4 will be used for pid and A0 and A7 will be used for extreme cases. It will stop the appropriate motor.
    */
    static float integral = 0;
    static float prev_error = 0;
    float offset = values[4]/65535.0*7.0-3.5;
    float derivative = offset - prev_error;
    prev_error = offset;
    if (values[0] > 32000){
        offset = -OUTER_SENSOR_OFFSET;
        return Kp * offset + Ki * integral + Kd * derivative;
    }
    else if(values[7] > 32000){
        offset = OUTER_SENSOR_OFFSET;
        return Kp * offset + Ki * integral + Kd * derivative;
    }
    integral += offset;
    if (integral > INTEGRAL_LIMIT) {
        integral = INTEGRAL_LIMIT;
    } else if (integral < -INTEGRAL_LIMIT) {
        integral = -INTEGRAL_LIMIT;
    }
    prev_error = offset;
    return (Kp * offset + Ki * integral + Kd * derivative);
}