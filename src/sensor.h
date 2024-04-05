#include <stdint.h>

/**
 * Read the value of each sensor in the array.
 *
 * @param values The array to store the value of each sensor.
 */
void sensor_read(uint16_t values[8]);

/**
 * Normalize the values read from the sensors in a specified range.
 *
 * @param values The array of values read from the sensor.
 * @param min The minimum value (maps to 0)
 * @param max The maximum value (maps to 65535)
 * @param normalized The array to store the normalized values.
*/
void sensor_normalize(
    uint16_t values[8],
    uint16_t min,
    uint16_t max,
    uint16_t normalized[8]
);

/**
 * Calculate the average of the sensor's readings over a period of time, which
 * can be used to calibrate the sensor.
 *
 * @param samples How many samples to average.
 * @param out An array of the average value of each individual sensor.
 * @returns The average value of all the sensors over the period of time.
*/
uint16_t sensor_calibrate(int samples, uint16_t out[8]);

/**
 * Applies a set of calibation values to a set of raw sensor values to produce
 * a normalized result.
 *
 * @param values The raw sensor values.
 * @param min The calibration values for the minimum value (maps to 0)
 * @param max The calibration values for the maximum value (maps to 65535)
 * @param[out] calibrated The array of values after calibration.
*/
void sensor_apply_calibration(
    uint16_t values[8],
    uint16_t min[8],
    uint16_t max[8],
    uint16_t calibrated[8]
);