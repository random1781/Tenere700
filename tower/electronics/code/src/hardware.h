#pragma once

// Include the Arduino core and I2C
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <QMC5883L.h>

// ========== 7-Segment (Adafruit LED Backpack) ==========
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// ========== IMU (Adafruit ISM330DHCX) ==========
#include <Adafruit_ISM330DHCX.h>

// ========== GPS (SparkFun Ublox GNSS v3) ==========
// Make sure you've installed the SparkFun_u-blox_GNSS_v3 library:
#include <SparkFun_u-blox_GNSS_v3.h>

// ========== NeoRotary (Adafruit NeoRotary 4) ==========
#include <Adafruit_seesaw.h>

// ========== Flags: whether hardware is found ==========
extern bool found7SegLeft;
extern bool found7SegRight;
extern bool foundIMU;
extern bool foundGPS;
extern bool foundQuadEnc;
extern bool foundCompass;

// ========== Global objects (one per device) ==========
// We will define these objects in hardware.cpp
extern Adafruit_7segment segLeft;
extern Adafruit_7segment segRight;
extern Adafruit_ISM330DHCX imu;
extern SFE_UBLOX_GNSS myGNSS;
extern Adafruit_seesaw quadEnc;
//extern QMC5883L compass;

// ========== Setup & check functions ==========
void initAllHardware();
void checkAllHardware(); // Optional periodic check