const float Kp = 0.1;
const float Ki = 0;
const float Kd = 0;

float pid(float offset) {
    // Static variables to keep track of the integral and derivative
    static float integral = 0;
    static float prev_error = 0;

    // Calculate the integral and derivative
    float derivative = offset - prev_error;
    // The integral is the sum of all the errors
    integral += offset;
    // Update the previous error
    prev_error = offset;
    // Return the output of the PID controller
    return Kp * offset + Ki * integral + Kd * derivative;
}