#include "LoadCell.h"

HX711 scale;
LoadMovingAverage loadMovingAverage;
double currentLoad = 0;

void setupLoadCell() {
  setCpuFrequencyMhz(RTC_CPU_FREQ_80M);;
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION);
  scale.tare();
  initializeLoadMovingAverage(loadMovingAverage);
}

void updateForceReading() {
  // Read input from the load cell
  double currForceReading = scale.get_units(2);

  // clip the currForceReading if exceed range
  if (abs(currForceReading) > MAX_INPUT_FORCE){
    if (currForceReading < 0){
      currForceReading = -MAX_INPUT_FORCE;
    } else{
      currForceReading = MAX_INPUT_FORCE;
    }
  } 
  if (abs(currForceReading) < MIN_INPUT_FORCE){
    if (currForceReading < 0){
      currForceReading = -MIN_INPUT_FORCE;
    } else {
      currForceReading = MIN_INPUT_FORCE;
    }
  }

  currentLoad = addToLoadMovingAverage(loadMovingAverage, currForceReading);
}


