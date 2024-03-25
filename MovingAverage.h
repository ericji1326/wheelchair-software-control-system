#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <Arduino.h>

#define RPM_QUEUE_SIZE 6
#define LOAD_QUEUE_SIZE 32

struct RPMMovingAverage {
  double array[RPM_QUEUE_SIZE];
  int currentIndex;          
  int count;                 
  double sum;                
};

struct LoadMovingAverage {
  double array[LOAD_QUEUE_SIZE];
  int currentIndex;          
  int count;                
  double sum;                
};

void initializeRPMMovingAverage(RPMMovingAverage& ma);
void initializeLoadMovingAverage(LoadMovingAverage& ma);
double addToRPMMovingAverage(RPMMovingAverage& ma, double value);
double addToLoadMovingAverage(LoadMovingAverage& ma, double value);

#endif
