#include "sensors.h"
#include "hardware.h"
#include "Arduino.h"
#include <string>
#include <cmath>
#include <TimeLib.h>
//#include <EEPROM.h>

int distance = 0; // Total distance traveled (meters)
int heading = 0;
int speed = 0;

int distanceFromSoftReset = 0;
int distanceFromCountdown = 0;
int targetDistanceCountdown = 0;
int timeStopwatchStart = 0;
//EEPROM.get(0, distanceFromSoftReset);

//static int heading = 0;

void callbackODO(UBX_NAV_ODO_data_t *ubxODO) {
  //odometerData = ubxDataStruct;
  distance = ubxODO->distance; // Update total distance
}

void callbackPVT(UBX_NAV_PVT_data_t *ubxPVT) {
  //pvtData = ubxDataStruct;
  heading = ubxPVT->headMot;
  speed = ubxPVT->gSpeed;
}

void setBoardOdometer() {
  distanceFromSoftReset = getCurrentGPSOdometer();
  Serial.print("distancefromsoftreset after press: "); Serial.println(distanceFromSoftReset);
}

void stopwatchStart() {
  timeStopwatchStart = myGNSS.getUnixEpoch();
}

void stopwatchReset() {
  timeStopwatchStart = 0;
}

void resetGPSOdometer() {
  myGNSS.resetOdometer();
  distanceFromSoftReset = 0;
}

void resetDistanceCountdown() {
  targetDistanceCountdown = 0;
  distanceFromCountdown = 0;
}

void modifyDistanceFromSoftReset(int amount) {
  distanceFromSoftReset += amount;
  Serial.println(distanceFromSoftReset);
}

void modifyTargetDistanceCoutdown(int amount) {
  targetDistanceCountdown += amount;
  Serial.println(targetDistanceCountdown);
}
  /*Serial.print(ubxDataStruct->distance);
  Serial.print(" / totalDistance: ");
  Serial.println(ubxDataStruct->totalDistance);*/

// This uses the controller to compute the difference between the GPS's
// odometer and a point where the user marked.
int getCurrentOdometer() {
  return distance - distanceFromSoftReset;
}

// This simply fetches the GPS's odometer.
int getCurrentSpeedometer() {
  int speedKph = round(speed * 3.6 / 1000.0);
  return speedKph;
}

// This simply fetches the GPS's odometer.
int getCurrentGPSOdometer() {
  return distance;
}

int getCurrentDistanceCountdown() {
  // Placeholder for countdown logic
  //targetDistanceCountdown = 5000;
  /*Serial.print(distance);
  Serial.print("< distance  >");
  Serial.println(targetDistanceCountdown - (distance - distanceFromCountdown));*/
  return targetDistanceCountdown - (distance - distanceFromCountdown);
}

int getCurrentCompassHeading() {
  // Return a rounded integer in degrees
  heading = myGNSS.packetUBXNAVPVT->callbackData->headMot * 0.00001;
  return round(heading);
}

int getCurrentTime() {
  /*int gnssHour = myGNSS.getHour();
  int gnssMinute = myGNSS.getMinute();
  char buffer[5];
  sprintf(buffer, "%02d%02d", gnssHour, gnssMinute);
  String timeString = String(buffer);
  /*Serial.print("GNSS: ");
  Serial.print(gnssHour);
  Serial.print(gnssMinute);
  Serial.print(" / timeString: ");
  Serial.println(timeString);*/
  int epoch = myGNSS.getUnixEpoch();
  return epoch;
  //return timeString.toInt();
}

unsigned long getCountdownTimer() {
  uint32_t us;
  uint32_t epoch = myGNSS.getUnixEpoch();
  epoch = myGNSS.getUnixEpoch(us);
  // Placeholder for countdown timer logic
  return 3333; // Example: 5 minutes remaining
}

unsigned long getStopwatch() {
  int currentTime = myGNSS.getUnixEpoch();
  int elapsedTime = currentTime - timeStopwatchStart;
  /*Serial.print(currentTime);
  Serial.print("<current elapsed>");
  Serial.println(elapsedTime);*/
  if (currentTime == elapsedTime) {
    return 0;
  }
  else {
    return elapsedTime;
  }
}
