// main.ino
#include "hardware.h"
#include "encoders.h"
#include "screens.h"
//#include "globals.h"

// Timers to handle periodic tasks
unsigned long lastHardwareCheck = 0;
const unsigned long HARDWARE_CHECK_INTERVAL = 1000; // 1 second for example
//const unsigned long LONG_PRESS_INTERVAL = 500; // Half second for example

void setup() {
  Serial.begin(115200);
  // while(!Serial);
  delay(1000);
  initAllHardware();
  initEncoders();
}

void loop() {
  // 1) Update encoders
  if (foundQuadEnc == true) {
    updateEncoders();
  }
  
  myGNSS.checkUblox(); // Check for the arrival of new data and process it.

  // 2) Periodically re-check hardware
  unsigned long now = millis();
  if(now - lastHardwareCheck > HARDWARE_CHECK_INTERVAL) {
    lastHardwareCheck = now;
    checkAllHardware();
  }

  updateDisplay(DisplaySide::LEFT);
  updateDisplay(DisplaySide::RIGHT);

  myGNSS.checkCallbacks();
}