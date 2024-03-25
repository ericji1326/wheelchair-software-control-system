#ifndef SPEED_DETECTION_H
#define SPEED_DETECTION_H

#define PPR 84.0       // number of windows on encoder wheel * 2 (2 interrupts occur for every rising edge)

#include <Arduino.h>
#include "MovingAverage.h"

// photointerrupter variables
extern unsigned long prevTime;
extern unsigned long currTime;
extern unsigned long T;
extern double rpm;
extern double currentAvgRPM;
extern double sumRpm;
extern unsigned long debounceDelay;
extern volatile unsigned long timeOfLastInterrupt;
extern volatile unsigned long timeOfLastTrans;
extern volatile unsigned long diff;
extern volatile unsigned interruptCnt;
extern unsigned loopCnt;
extern int n_interrupts;
extern volatile bool recordValue;
extern int dutyCycle;
extern bool REVERSE;

extern RPMMovingAverage rpmMovingAverage;

void initializeSpeedDetection();
void INT0_ISR();
void updateRpm();
#endif