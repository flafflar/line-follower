#include "sensor.h"

#include "mux.h"

void sensor_read(uint16_t values[8]) {
    for (int ch = 0; ch < 8; ch++) {
        values[ch] = mux_read(ch);
    }
}

void sensor_normalize(
    uint16_t values[8],
    uint16_t min,
    uint16_t max,
    uint16_t norm[8]
) {
    uint16_t factor = 65535 / (int) (max - min);
    for (int s = 0; s < 8; s++) {
        if (values[s] <= min) {
            norm[s] = 0;
        } else if (values[s] >= max) {
            norm[s] = 65535;
        } else {
            norm[s] = (values[s] - min) * factor;
        }
    }
}

uint16_t sensor_calibrate(int samples, uint16_t out[8]) {
    // Initialize the sum array with zeroes
    uint32_t sum[8] = {};

    for (int i = 0; i < samples; i++) {
        uint16_t values[8];
        sensor_read(values);
        for (int s = 0; s < 8; s++) {
            sum[s] += values[s];
        }
    }

    uint32_t total_sum = 0;
    for (int s = 0; s < 8; s++) {
        total_sum += sum[s];
        out[s] = sum[s] / samples;
    }

    return total_sum / 8 / samples;
}

void sensor_apply_calibration(
    uint16_t values[8],
    uint16_t min[8],
    uint16_t max[8],
    uint16_t calibrated[8]
) {
    for (int s = 0; s < 8; s++) {
        if (values[s] <= min[s]) {
            calibrated[s] = 0;
        } else if (values[s] >= max[s]) {
            calibrated[s] = 65535;
        } else {
            uint16_t factor = 65535 / (int) (max[s] - min[s]);
            calibrated[s] = (values[s] - min[s]) * factor;
        }
    }
}