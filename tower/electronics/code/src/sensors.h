#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include "SparkFun_u-blox_GNSS_v3.h"

//int distanceFromSoftReset;
//extern int distanceFromCountdown;
//extern int targetDistanceCountdown;
//extern int timeFromStopwatch;

void initSensors();
void callbackODO(UBX_NAV_ODO_data_t *ubxDataStruct);
void callbackPVT(UBX_NAV_PVT_data_t *ubxDataStruct);
void setBoardOdometer();
void stopwatchStart();
void stopwatchReset();
void resetGPSOdometer();
void resetDistanceCountdown();
void modifyDistanceFromSoftReset(int amount);
void modifyTargetDistanceCoutdown(int amount);
int getCurrentOdometer();        // Return odometer reading
int getCurrentSpeedometer();        // Return speedometer reading
int getCurrentGPSOdometer();        // Return odometer reading
int getCurrentDistanceCountdown(); // Return remaining distance
int getCurrentCompassHeading();  // Return compass heading
int getCurrentTime();    // Return current time in seconds
unsigned long getCountdownTimer(); // Return countdown timer in seconds
unsigned long getStopwatch();      // Return stopwatch value in seconds

#endif