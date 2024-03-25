#include <Arduino.h>

#define MAX_PWM 255.0
#define MIN_PWM 0.0
#define PWM_SPEEDUP_THESH 55.0

extern double prev_error;
extern double prev_time;
const double kd = 0.05;
const double kp =  0.10;

void pdController(double target, double current, int& oldDutyCycle);