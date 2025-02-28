// screens.cpp
#include "screens.h"
#include "hardware.h"   // to check if found7SegLeft, found7SegRight, etc.
//#include "globals.h"    // for screen enums
#include "sensors.h"
#include "Arduino.h"

extern int distanceFromSoftReset;
extern int timeStopwatchStart;

// Default screen stataes
ScreenType currentScreenLeft = ScreenType::ODOMETER;
ScreenType currentScreenRight = ScreenType::COMPASS_HEADING;

// Arrays of screens for left and right displays
ScreenType mainScreensLeft[] = {
    ScreenType::ODOMETER
//    ScreenType::DISTANCE_COUNTDOWN
};

ScreenType mainScreensRight[] = {
    ScreenType::COMPASS_HEADING,
    ScreenType::SPEEDOMETER,
    ScreenType::GPSODOMETER
//    ScreenType::CURRENT_TIME,
//    ScreenType::COUNTDOWN_TIMER,
//    ScreenType::STOPWATCH
};

const int numMainScreensLeft = sizeof(mainScreensLeft) / sizeof(ScreenType);
const int numMainScreensRight = sizeof(mainScreensRight) / sizeof(ScreenType);

int countdownDistance = 5000; // Distance in meters
int countdownTimer = 300;     // Timer in seconds

static int lastDisplayedValueLeft = -1;
static int lastDisplayedValueRight = -1;

DisplayType getDisplayTypeForScreen(ScreenType screen) {
    switch (screen) {
        case ScreenType::ODOMETER:
          return DisplayType::DISTANCE_KM;
        case ScreenType::GPSODOMETER:
          return DisplayType::DISTANCE_KM;
        case ScreenType::SPEEDOMETER:
          return DisplayType::SPEED_KPH;
        case ScreenType::DISTANCE_COUNTDOWN:
          return DisplayType::DISTANCE_KM;
        case ScreenType::SET_COUNTDOWN:
          return DisplayType::DISTANCE_KM;
        case ScreenType::COMPASS_HEADING:
          return DisplayType::DEGREES;
        case ScreenType::CURRENT_TIME:
          return DisplayType::TIME_HHMM;
        case ScreenType::COUNTDOWN_TIMER:
          return DisplayType::TIME_HHMM;
        case ScreenType::STOPWATCH:
          return DisplayType::TIME_HHMM;
        case ScreenType::SET_TIMER:
          return DisplayType::TIME_MMSS;

        default:
            return DisplayType::DEGREES; // Fallback
    }
}

void formatAndDisplay(Adafruit_7segment &display, DisplayType type, int value) {
    display.clear();

/*
     1
   ------
32|      | 2
  |  64  |
   ------
16|      | 4
  |  8   |
   ------
*/
    switch (type) {
        // Accepts meters as input and converts to KM
        case DisplayType::DISTANCE_KM: {
            // Convert meters to kilometers
            float distanceKM = value * 0.001;
            /*Serial.print("value: ");
            Serial.print(value);
            Serial.print(" dkm: ");
            Serial.print(distanceKM);*/

            // Show two decimal places if it fits in the screen
            if (value <= 99999) {
              display.printFloat(distanceKM, 2);
            }
            // Otherwise show only one. I doubt the ODO will be over 999 km.
            else {
              display.printFloat(distanceKM, 1);
            }
            break;
        }

        case DisplayType::SPEED_KPH: {
          display.printFloat(value, 0);
        }

        case DisplayType::DEGREES: {
            if (value < 0 || value > 359) {
                // Print dashes for invalid value
                display.writeDigitRaw(0, 0x40);
                display.writeDigitRaw(1, 0x40);
                display.writeDigitRaw(3, 0x40);
                display.writeDigitRaw(4, 0x40);
            } else if (value == 0) {
                display.writeDigitNum(3, 0);
                display.writeDigitRaw(4, 0x63);
            }
            else {
                // Multiplying by 10 shifts everything left one, which allows space for...
                display.print(value * 10);
                // ...a degree symbol
                display.writeDigitRaw(4, 0x63);
            }
            break;
        }

        case DisplayType::TIME_HHMM: {
            int hours = value / 100;
            int minutes = value % 100;
            if (hours > 23 || minutes > 59) {
                // Print dashes for invalid value
                display.writeDigitRaw(0, 0x40);
                display.writeDigitRaw(1, 0x40);
                display.writeDigitRaw(3, 0x40);
                display.writeDigitRaw(4, 0x40);
                break;
            } else {
                display.writeDigitNum(0, hours / 10); // Tens place of hours
                display.writeDigitNum(1, hours % 10); // Ones place of hours

                // Display colon
                display.drawColon(true);

                // Display minutes (right 2 digits)
                display.writeDigitNum(3, minutes / 10); // Tens place of minutes
                display.writeDigitNum(4, minutes % 10); // Ones place of minutes
            }
            break;
        }

        case DisplayType::TIME_MMSS: {
            int minutes = value / 60;
            int seconds = value % 60;
            if (minutes > 99 || seconds > 59) {
                // Print dashes for invalid value
                display.writeDigitRaw(0, 0x40);
                display.writeDigitRaw(1, 0x40);
                display.writeDigitRaw(3, 0x40);
                display.writeDigitRaw(4, 0x40);
            } else {
                display.print(minutes * 100 + seconds);
                display.drawColon(true); // Enable colon
            }
            break;
        }
    }
    //Serial.println("WRITE");
    display.writeDisplay();
}

void cycleScreen(DisplaySide side, int direction) {
    ScreenType &currentScreen = (side == DisplaySide::LEFT) ? currentScreenLeft : currentScreenRight;
    ScreenType *mainScreens = (side == DisplaySide::LEFT) ? mainScreensLeft : mainScreensRight;
    int numScreens = (side == DisplaySide::LEFT) ? numMainScreensLeft : numMainScreensRight;

    // Find the current screen index in the respective mainScreens array
    int currentIndex = -1;
    for (int i = 0; i < numScreens; i++) {
        if (mainScreens[i] == currentScreen) {
            currentIndex = i;
            break;
        }
    }

    // Update the index based on the direction
    if (currentIndex != -1) {
        currentIndex = (currentIndex + direction + numScreens) % numScreens;
        currentScreen = mainScreens[currentIndex];
    }
}

// ---------------------------------------------------------------------
// Update Display (Handles Left or Right Display)
// ---------------------------------------------------------------------
void updateDisplay(DisplaySide side) {
    ScreenType currentScreen = (side == DisplaySide::LEFT) ? currentScreenLeft : currentScreenRight;
    Adafruit_7segment *display = (side == DisplaySide::LEFT) ? &segLeft : &segRight;
    int *lastDisplayedValue = (side == DisplaySide::LEFT) ? &lastDisplayedValueLeft : &lastDisplayedValueRight;

    int newValue = -1; // Default invalid value

    switch (currentScreen) {
        case ScreenType::ODOMETER: {
            newValue = getCurrentOdometer();
            break;
        }
        case ScreenType::GPSODOMETER: {
            newValue = getCurrentGPSOdometer();
            break;
        }
        case ScreenType::SPEEDOMETER: {
            newValue = getCurrentSpeedometer();
            break;
        }
        case ScreenType::DISTANCE_COUNTDOWN: {
            newValue = getCurrentDistanceCountdown(); // Distance in meters
            break;
        }
        case ScreenType::COMPASS_HEADING: {
          unsigned long now = millis();
            newValue = getCurrentCompassHeading();
            break;
        }
        case ScreenType::CURRENT_TIME: {
            newValue = getCurrentTime();
            break;
        }
        case ScreenType::COUNTDOWN_TIMER: {
            newValue = countdownTimer; // Timer in seconds
            break;
        }
        case ScreenType::STOPWATCH: {
            newValue = getStopwatch();
            break;
        }
        case ScreenType::SET_COUNTDOWN: {
            newValue = 69; // Countdown distance in meters
            segLeft.blinkRate(1);
            break;
        }
        case ScreenType::SET_TIMER: {
            newValue = countdownTimer; // Timer in seconds
            break;
        }
        default: {
            display->clear();
            display->print(9999); // Default invalid state
            display->writeDisplay();
            return;
        }
    }

    // Check if the new value is different before updating the display
    if (newValue != *lastDisplayedValue) {
        // Get the appropriate DisplayType for the current screen
        DisplayType displayType = getDisplayTypeForScreen(currentScreen);

        // Update the display and save the new value
        //Serial.println(newValue);
        formatAndDisplay(*display, displayType, newValue);
        *lastDisplayedValue = newValue;
    }
}

// ---------------------------------------------------------------------
// Handle Screen Input (Handles Left or Right Display)
// ---------------------------------------------------------------------
void handleScreenInput(DisplaySide side, int encoderIndex, int encoderValue, ButtonPressType buttonPressType) {
    // Determine the current screen for the specified side
    ScreenType &currentScreen = (side == DisplaySide::LEFT) ? currentScreenLeft : currentScreenRight;

    switch (currentScreen) {
        case ScreenType::SET_COUNTDOWN: {
            // Adjust the countdown distance using the encoder
            countdownDistance += encoderValue * 100; // Adjust in 100m increments
            if (countdownDistance < 0) countdownDistance = 0;
            if (countdownDistance > 999900) countdownDistance = 999900; // Cap at 999.9 km

            // Handle button press types
            if (buttonPressType == ButtonPressType::SINGLE_PRESS) {
                // Exit sub-screen and return to the main screen
                currentScreen = ScreenType::DISTANCE_COUNTDOWN;
            } else if (buttonPressType == ButtonPressType::DOUBLE_PRESS) {
                // Reset countdown distance to default
                countdownDistance = 5000; // Example default value
            } else if (buttonPressType == ButtonPressType::LONG_PRESS) {
                // Set countdown distance to the maximum value
                countdownDistance = 999900;
            }
            break;
        }
        case ScreenType::SET_TIMER: {
            // Adjust the timer using the encoder
            countdownTimer += encoderValue * 10; // Adjust in 10-second increments
            if (countdownTimer < 0) countdownTimer = 0;
            if (countdownTimer > 35999) countdownTimer = 35999; // Cap at 9:59:59

            // Handle button press types
            if (buttonPressType == ButtonPressType::SINGLE_PRESS) {
                // Exit sub-screen and return to the main screen
                currentScreen = ScreenType::COUNTDOWN_TIMER;
            } else if (buttonPressType == ButtonPressType::DOUBLE_PRESS) {
                // Reset timer to default
                countdownTimer = 300; // Example default value
            } else if (buttonPressType == ButtonPressType::LONG_PRESS) {
                // Set timer to maximum value
                countdownTimer = 35999; // Max timer value
            }
            break;
        }
        case ScreenType::ODOMETER: {
          Serial.println(encoderIndex);
          if (encoderIndex == 0 && buttonPressType == ButtonPressType::LONG_PRESS) {
            //distanceFromSoftReset = getCurrentOdometer();
            setBoardOdometer();
          }
          break;
        }
        case ScreenType::DISTANCE_COUNTDOWN: {
          Serial.println(encoderIndex);
          if (encoderIndex == 1 && buttonPressType == ButtonPressType::SINGLE_PRESS) {
            resetDistanceCountdown();
          }
          if (encoderIndex == 1 && buttonPressType == ButtonPressType::LONG_PRESS) {
            setBoardOdometer();
          }
          break;
        }
        case ScreenType::GPSODOMETER: {
          if (encoderIndex == 3 && buttonPressType == ButtonPressType::LONG_PRESS) {
            resetGPSOdometer();
          }
          break;
        }
        case ScreenType::STOPWATCH: {
          if (encoderIndex == 2 && buttonPressType == ButtonPressType::LONG_PRESS) {
            //stopwatchReset();
          }
          if (encoderIndex == 3 && buttonPressType == ButtonPressType::SINGLE_PRESS) {
            Serial.println("STOPWATCHGO");
            //stopwatchStart();
          }
          break;
        }
        default:
            // No input handling for other screens
            break;
    }
}
