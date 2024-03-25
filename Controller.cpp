#include "Controller.h"

double prev_error = 0;
double prev_time = 0;

// PD controller function, returns an incremental rpm in the correct direction to reach target
void pdController(double targetRpm, double currentRpm, int& oldDutyCycle) {
  double error = targetRpm - currentRpm;
  unsigned long currTime = micros();
  double dt = (currTime - prev_time)/1000000;
  double derivative = (error - prev_error) / dt;
  prev_time = currTime;
  prev_error = error;

  double prop_error = kp * error;
  double deriv_error = kd * derivative;
  
  // add proportional and deriv components to old duty cycle
  double deltaDutyCycle = prop_error + deriv_error;
  int newDutyCycle = oldDutyCycle + deltaDutyCycle;

  // clip duty cycle to limits of pwm
  if (newDutyCycle > MAX_PWM){
    newDutyCycle = MAX_PWM;
  }
  // duty cycle is set to the pwm that makes rpm 0, actual pwm set to 0 to prevent stalling
  else if (newDutyCycle <= MIN_PWM){
    newDutyCycle = MIN_PWM;
    // ledcWrite(0, 0);
    // ledcWrite(1, 0);
  }
  
  oldDutyCycle = newDutyCycle;
}
  