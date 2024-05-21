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

/**
 * Computes a linear interpolation between two points.
 *
 * @param a The first point.
 * @param b The second point.
 * @param t The parameter of the interpolation. Should be in the range [0, 1],
 * otherwise the function will caluculate an extrapolation.
 * @returns The interpolated value.
 */
static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

/**
 * Computes the x value at which the line passing from points (x1, y1) and
 * (x2, y2) intercepts the x axis.
 *
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 * @returns The x value at which the line has y = 0.
 */
static float x_intercept(float x1, float y1, float x2, float y2) {
    return x1 - y1 * (x2 - x1) / (y2 - y1);
}

/**
 * Finds the fractional index at which the function defined by the given points
 * has its maximum value.
 *
 * @param valyes The values of the function to find the peak.
 * @returns The index of the peak.
 */
static float find_peak(float values[8]) {
    // Calculate the numeric derivative of the values
    float derivative[8] = {[0] = 0}; // Set the initial value to zero
    for (int i = 1; i < 8; i++) {
        derivative[i] = values[i] - values[i - 1];
    }

    // The derivative of the first point is always 0 since there is no other
    // point previous to it. If the peak value is somewhere near sensor 0, the
    // second derivative is used to give the missing derivative a better value
    float second_derivative_0 = derivative[1] - derivative[0];
    float second_derivative_1 = derivative[2] - derivative[1];
    if (second_derivative_1 < 0 && second_derivative_1 < second_derivative_0) {
        derivative[0] = derivative[1] - second_derivative_1;
    }

    // Calculate the points at which the derivative of the function is zero
    // The derivative is assumed to be piecewise linear
    float zeroes[8];
    int zero_count = 0;
    for (int i = 1; i < 8; i++) {
        float x = x_intercept(i - 1, derivative[i - 1], i, derivative[i]);
        // Check that the intercept is actually between the two edge points
        if (i - 1 <= x && x <= i) {
            zeroes[zero_count++] = x;
        }
    }

    // Since the points at which the derivative is zero are possible maxima,
    // calculate the y value of the sensor data at each point
    float critical[8];
    for (int i = 0; i < zero_count; i++) {
        // The point is between two integer values, so calculate the two
        // adjacent integers in order to interpolate between the sensor's value
        // at that points
        int s = (int) zeroes[i];
        // Since the points have distance 1, the t parameter does not need to
        // be scaled, the subtraction is enough
        float t = zeroes[i] - s;

        critical[i] = lerp(values[s], values[s + 1], t);
    }

    // Find the maximum of the critical points
    float y_max = -1, x_max = -1;
    for (int i = 0; i < zero_count; i++) {
        if (critical[i] > y_max) {
            y_max = critical[i];
            x_max = zeroes[i];
        }
    }

    // Return the x value of the maximum point
    return x_max;
}

float sensor_calculate_center(uint16_t values[8]) {
    // Normalize the values as floats in the range 0-1
    // Also calculate the maximum value
    float norm[8];
    float max = 0;
    for (int s = 0; s < 8; s++) {
        norm[s] = values[s] / 65535.0;
	if (norm[s] > max) max = norm[s];
    }

    // If there are only small values, the sensor probably sees only white, so
    // return an invalid value
    if (max < 0.5) {
        return -1.0f;
    }

    // Flip the values so they go left-to-right
    float norm_flipped[8];
    for (int s = 0; s < 8; s++) {
        norm_flipped[7 - s] = norm[s];
    }

    // Calculate the center (which is the point that the sensor value peaks)
    // starting from the left
    float center_r = find_peak(norm);
    // Calculate the center, this time starting from the right
    float center_l = 8.0f - find_peak(norm_flipped);
    // Average the two centers
    float center_avg = (center_r + center_l) / 2.0f;

    // Return the calculated center depending on which side it is located
    if (center_avg < 3.5) {
        return center_r / 8.0f * 7.0f;
    } else {
        return center_l / 8.0f * 7.0f;
    }
}
