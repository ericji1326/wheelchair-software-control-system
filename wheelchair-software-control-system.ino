#include <Arduino.h>
#include "LoadCell.h"
#include "Controller.h"
#include "SpeedDetection.h"

// photointerrupter variables
#define OPT_ENCODERPIN_1 33

// motor drive variables
#define MAX_RPM 36.0

// motor pwm vars
int motor1Pin1 = 26;
int motor1Pin2 = 27;
const int freq = 100;
const int resolution = 8;

// pid vars
double targetRPM = 0;

// variables for slowing down motor
double MAX_DIFF = MAX_INPUT_FORCE - MIN_INPUT_FORCE;
int MAX_INCREMENTS = 100;

void setup() {
  Serial.begin(115200);
  Serial.println("currentAvgLoad, currentAvgRPM, targetRPM, dutyCycle, newDutyCycle, error");

  // Interrupt Setup
  pinMode(OPT_ENCODERPIN_1, INPUT);
  attachInterrupt(digitalPinToInterrupt(OPT_ENCODERPIN_1), INT0_ISR, RISING);

  // pwm setup: sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  
  // configure LED PWM functionalities
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(motor1Pin1, 0);
  ledcAttachPin(motor1Pin2, 1);

  setupLoadCell();
}

void loop() {

  updateForceReading();
  updateRpm();

  // Serial.print("Current Load: ");
  Serial.print(currentLoad/25.0);
  Serial.print(", ");

  // Serial.print("Current RPM: ");
  Serial.print(currentAvgRPM);
  Serial.print(", ");

  // Serial.print("Current Duty Cycle: ");
  Serial.println(dutyCycle);

  const double speedWeight = 0.3;
  const double forceWeight =  1 - speedWeight;

  // diff between current speed and min speed
  double speedDiff = dutyCycle - MIN_PWM;
  // diff between current force and min force
  double forceDiff = abs(currentLoad) - MIN_INPUT_FORCE;
  // number of increments due to speed difference
  int nIncrementSpeed = map(speedDiff, 0, MAX_PWM-MIN_PWM, 1, MAX_INCREMENTS);
  int nIncrementForce = map(forceDiff, MAX_INPUT_FORCE-MIN_INPUT_FORCE, 0, 1, MAX_INCREMENTS);
  // total weighted number of increments
  int nIncrements = speedWeight*nIncrementSpeed + forceWeight*nIncrementForce;

  if (currentLoad < 0 && !REVERSE){ // moving average load is backwards and we are going forward
    slowDownMotor(nIncrements);
    REVERSE = true;
  }
  if (currentLoad > 0 && REVERSE) { // moving average load is forwards and we are going backwards
    slowDownMotor(nIncrements);
    REVERSE = false;
  }
  if (currentAvgRPM != 0 && abs(currentLoad) <= MIN_INPUT_FORCE){
    slowDownMotor(nIncrements);
  }

  double absCurrForceReading = abs(currentLoad);
  // Map to target RPM based on force reading
  if (absCurrForceReading > MAX_INPUT_FORCE) {
      // Serial.println("Force exceeds maximum.");
      targetRPM = MAX_RPM;
  } else if (absCurrForceReading < MIN_INPUT_FORCE) {
      // Serial.println("Force is below minimum.");
      targetRPM = 0.0;
  } else {
      // Serial.println("Force is within range.");
      targetRPM = map(absCurrForceReading, MIN_INPUT_FORCE, MAX_INPUT_FORCE, 0.0, MAX_RPM);
  }

  // Calculate motor speed using proportional controller
  // Get new dutycycle, overwrite the old duty cycle
  pdController(targetRPM, currentAvgRPM, dutyCycle);
  
  // drive the motor
  if (REVERSE){
    ledcWrite(0, 0);
    ledcWrite(1, dutyCycle);
  } else {
    ledcWrite(0, dutyCycle);
    ledcWrite(1, 0);
  }
  delay(10);
}

// ramps down motor when there is a change in direction
void slowDownMotor(int n_increments){
  double increment = ((double) dutyCycle - MIN_PWM)/(double) n_increments;
  for (int i=0; i < n_increments; i++){
    if (dutyCycle < PWM_SPEEDUP_THESH){
      increment = 15;
    }
    dutyCycle -= increment;
    if (dutyCycle < 0){
      break;
    }
    if (REVERSE){
      ledcWrite(0, 0);
      ledcWrite(1, dutyCycle);
    } else {
      ledcWrite(0, dutyCycle);
      ledcWrite(1, 0);
    }
    delay(10);
  }

  // reset the rpmArray
  for (int i = 0; i < RPM_QUEUE_SIZE; i++) {
    currentAvgRPM = addToRPMMovingAverage(rpmMovingAverage, 0);
  }
}
