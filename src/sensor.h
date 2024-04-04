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