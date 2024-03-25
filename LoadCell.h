#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"
#include "MovingAverage.h"

#define MAX_INPUT_FORCE 500.0 // Should corresponds to 110N?? Change after physical testing
#define MIN_INPUT_FORCE 20.0 // Change this after physical testing

extern HX711 scale;
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

// calibration for the load cell
const float CALIBRATION = 2095/23; // load cell 1
// const float CALIBRATION = -2080/23; // load cell 2

extern LoadMovingAverage loadMovingAverage;
extern double currentLoad;

void setupLoadCell();
void updateForceReading();

#endif