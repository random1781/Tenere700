// screens.h
#pragma once

#include "hardware.h"

// Enum for all screens (main and sub-screens)
enum class ScreenType {
    ODOMETER,
    GPSODOMETER,
    SPEEDOMETER,
    DISTANCE_COUNTDOWN,
    COMPASS_HEADING,
    CURRENT_TIME,
    COUNTDOWN_TIMER,
    STOPWATCH,
    SET_COUNTDOWN,   // Sub-screen for setting countdown distance
    SET_TIMER        // Sub-screen for setting timer
};

// Screen state for each display
extern ScreenType currentScreenLeft;
extern ScreenType currentScreenRight;

// Display types for formatting
enum class DisplayType {
  TIME_HHMM,      // Hours and minutes (24-hour format)
  TIME_MMSS,      // Minutes and seconds (countdown/countup)
  DISTANCE_KM,     // Distance in kilometers
  SPEED_KPH,
  DEGREES         // Compass heading in degrees
};

enum class DisplaySide {
    LEFT,
    RIGHT
};

// Enum for button press types
enum class ButtonPressType {
    NONE,
    SINGLE_PRESS,
    DOUBLE_PRESS,
    LONG_PRESS
};

//void formatAndDisplay(DisplaySide side, DisplayType type, int value);
void formatAndDisplay(Adafruit_7segment &display, DisplayType type, int value);

void updateDisplay(DisplaySide side);
void handleScreenInput(DisplaySide side, int encoderIndex, int encoderValue, ButtonPressType buttonPressed);
void cycleScreen(DisplaySide side, int direction);