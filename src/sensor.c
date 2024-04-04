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
        norm[s] = (values[s] - min) * factor;
    }
}