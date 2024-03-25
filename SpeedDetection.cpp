#include "SpeedDetection.h"

RPMMovingAverage rpmMovingAverage;

// photointerrupter variables
unsigned long prevTime = 0;
unsigned long currTime = 0;
unsigned long T;
double rpm = 0;
double currentAvgRPM = 0;
double sumRpm = 0;
unsigned long debounceDelay = 5000;
volatile unsigned long timeOfLastInterrupt;
volatile unsigned long timeOfLastTrans;
volatile unsigned long diff;
volatile unsigned interruptCnt;
unsigned loopCnt;
int n_interrupts = 0;
volatile bool recordValue = true;
int dutyCycle = 0;
bool REVERSE = false;


void INT0_ISR() {
  unsigned long tmpTime = micros();
  if ((tmpTime - timeOfLastInterrupt) > debounceDelay) {
    diff = tmpTime - timeOfLastTrans;
    timeOfLastTrans = tmpTime;
    ++interruptCnt;
  }
  timeOfLastInterrupt = tmpTime;
}

void initializeSpeedDetection() {
  initializeRPMMovingAverage(rpmMovingAverage);
}

void updateRpm() {
  // check if there have been interrupts since the last time the void loop ran
  unsigned long currTime = micros();

  // valid interrupt has occurred
  if (interruptCnt > loopCnt) {
    if (recordValue) {
      rpm = (60000000UL) / (diff * PPR);
      currentAvgRPM = addToRPMMovingAverage(rpmMovingAverage, rpm);
    }
    loopCnt = interruptCnt;
    recordValue = !recordValue; // record every other valid rpm from interrupt

  } else if (currTime - timeOfLastTrans >= 500000) {
    currentAvgRPM = addToRPMMovingAverage(rpmMovingAverage, 0.0);
    timeOfLastTrans = currTime;
    currentAvgRPM = sumRpm / rpmMovingAverage.count;
  }
}