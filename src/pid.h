/*
 *  This file defines methods for controlling the PID controller. 
 */

/**
 * Reads the offset from the sensor and returns the output of the PID controller.
 * 
 * @param offset A number from -3.5 to 3.5, the distance from the line.
 * @return The speed of the motors, if the value is postive, the robot will
 * substract the value from the left motor, if it is negative, the robot will
 * add the value from the right motor.
 */

float pid(uint16_t values[8]);
