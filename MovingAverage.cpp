#include "MovingAverage.h"

void initializeLoadMovingAverage(LoadMovingAverage &ma) {
  ma.currentIndex = 0;
  ma.count = 0;
  ma.sum = 0.0;
  for (int i = 0; i < LOAD_QUEUE_SIZE; ++i) {
    ma.array[i] = 0.0;
  }
}

void initializeRPMMovingAverage(RPMMovingAverage &ma) {
  ma.currentIndex = 0;
  ma.count = 0;
  ma.sum = 0.0;
  for (int i = 0; i < RPM_QUEUE_SIZE; ++i) {
    ma.array[i] = 0.0; 
  }
}

double addToLoadMovingAverage(LoadMovingAverage& ma, double value) {
  if (ma.count >= LOAD_QUEUE_SIZE) {
    ma.sum -= ma.array[ma.currentIndex];
  } else {
    ma.count++;
  }

  ma.sum += value;

  ma.array[ma.currentIndex] = value;
  ma.currentIndex = (ma.currentIndex + 1) % LOAD_QUEUE_SIZE;

  return ma.sum / ma.count;
}

double addToRPMMovingAverage(RPMMovingAverage& ma, double value) {
  if (ma.count >= RPM_QUEUE_SIZE) {
    ma.sum -= ma.array[ma.currentIndex];
  } else {
    ma.count++;
  }

  ma.sum += value;

  ma.array[ma.currentIndex] = value;
  ma.currentIndex = (ma.currentIndex + 1) % RPM_QUEUE_SIZE;

  return ma.sum / ma.count;
}