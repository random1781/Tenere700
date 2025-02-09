// encoders.cpp
#include "encoders.h"
//#include "globals.h" // or hardware info
#include "hardware.h"
#include "screens.h"
#include "sensors.h"

// ========== Encoder button pins ========== 
#define NEOROTARY_ADDR  (0x49)
#define ENC0_SWITCH     12
#define ENC1_SWITCH     14
#define ENC2_SWITCH     17
#define ENC3_SWITCH     9

static int segBrightness = 15;
//extern int targetDistanceCountdown;

static const uint8_t ENC_SWITCHES[4] = {ENC0_SWITCH, ENC1_SWITCH,
                                        ENC2_SWITCH, ENC3_SWITCH};
// ========== End encoder button pins ==========

static const unsigned long DOUBLE_PRESS_THRESHOLD = 200;
static const unsigned long LONG_PRESS_THRESHOLD   = 500;

// ========== Encoder state ==========
static bool lastButtonState[4] = {false, false, false, false};
static bool waitingForDouble[4] = {false, false, false, false};
static unsigned long pressStart[4] = {0, 0, 0, 0};
static unsigned long lastPressTime[4] = {0, 0, 0, 0};
static bool longPressFired[4] = {false, false, false, false};

int32_t encPositions[4] = {0, 0, 0, 0};
// ========== End encoder state ==========

// ---------------------------------------------------------------------
// Forward declarations for our event handlers
// ---------------------------------------------------------------------
static void handleEncoderIncrement(int i, int32_t steps);
static void handleEncoderDecrement(int i, int32_t steps);
static void handleSinglePress(int i);
static void handleDoublePress(int i);
static void handleLongPress(int i);

bool initEncoders()
{
  if (quadEnc.begin(NEOROTARY_ADDR)) {
    Serial.println("Found NeoRotary!");

    quadEnc.pinMode(ENC0_SWITCH, INPUT_PULLUP);
    quadEnc.pinMode(ENC1_SWITCH, INPUT_PULLUP);
    quadEnc.pinMode(ENC2_SWITCH, INPUT_PULLUP);
    quadEnc.pinMode(ENC3_SWITCH, INPUT_PULLUP);

    quadEnc.setGPIOInterrupts(1UL << ENC0_SWITCH | 1UL << ENC1_SWITCH | 
                              1UL << ENC2_SWITCH | 1UL << ENC3_SWITCH, 1);

    for (int i = 0; i < 4; i++) {
      encPositions[i] = quadEnc.getEncoderPosition(i);
      quadEnc.enableEncoderInterrupt(i);
    }

    return true;
  }
  else
  {
    return false;
  }
}

// This function is called frequently in loop()
void updateEncoders()
{
  // ========== Button press ==========
  for (int i = 0; i < 4; i++)
  {
    bool pressed = ! quadEnc.digitalRead(ENC_SWITCHES[i]);

    // Detect PRESS START
    if (pressed && !lastButtonState[i]) {
      pressStart[i] = millis();
      longPressFired[i] = false;
    }

    // Detect RELEASE
    if (!pressed && lastButtonState[i]) {
      unsigned long pressDuration = millis() - pressStart[i];
      if (pressDuration >= LONG_PRESS_THRESHOLD) {
        Serial.println("first handlelongpress");
        //handleLongPress(i);
      } else {
        // short press
        if (waitingForDouble[i]) {
          // second short press => double
          handleDoublePress(i);
          waitingForDouble[i] = false;
        } else {
          // first short press => start waiting
          waitingForDouble[i] = true;
          lastPressTime[i]    = millis();
        }
      }
    }

    // Detect long press in progress
    if (pressed && !longPressFired[i]) {
      unsigned long pressDuration = millis() - pressStart[i];
      if (pressDuration >= LONG_PRESS_THRESHOLD) {
        Serial.println("2nd handlelongpress");
        handleLongPress(i);
        longPressFired[i] = true;
      }
    }

    // 4) If waitingForDouble, check the timer
    if (waitingForDouble[i] && (millis() - lastPressTime[i] > DOUBLE_PRESS_THRESHOLD)) {
      // time expired => single press
      handleSinglePress(i);
      waitingForDouble[i] = false;
    }

    // Update for next iteration
    lastButtonState[i] = pressed;

    // ========== Rotation ==========
    int32_t newEncPosition = quadEnc.getEncoderPosition(i);
    int32_t delta = newEncPosition - encPositions[i];
    // did we move around?
    if (encPositions[i] != newEncPosition) {
      //Serial.print("Encoder #");
      //Serial.print(i);
      if (delta > 0) {
        handleEncoderIncrement(i, delta);
        //Serial.print(" -> ");
      }
      else {
        handleEncoderDecrement(i, delta); 
        //Serial.print(" <- ");
      }
      //Serial.println(newEncPosition);      // display new position
      encPositions[i] = newEncPosition;      // and save for next round
    }
  }


  //delay(10);
}

static void handleSinglePress(int i) {
  Serial.print("Encoder "); Serial.print(i); Serial.println(" SINGLE press");
  if (i == 3) {
    handleScreenInput(DisplaySide::RIGHT, i, 0, ButtonPressType::SINGLE_PRESS);
  }
}

static void handleDoublePress(int i) {
  Serial.print("Encoder "); Serial.print(i); Serial.println(" DOUBLE press");
}

static void handleLongPress(int i) {
  Serial.print("Encoder "); Serial.print(i); Serial.println(" LONG press");
  // Long press encoder 0
  if (i == 0) {
    handleScreenInput(DisplaySide::LEFT, i, 0, ButtonPressType::LONG_PRESS);
  }
  else if (i == 1) {
    handleScreenInput(DisplaySide::LEFT, i, 1, ButtonPressType::LONG_PRESS);
  }
  else if (i == 2) {
    handleScreenInput(DisplaySide::RIGHT, i, 2, ButtonPressType::LONG_PRESS);
  }
  else if (i == 3) {
    handleScreenInput(DisplaySide::RIGHT, i, 3, ButtonPressType::LONG_PRESS);
  }
}

static void handleEncoderIncrement(int i, int32_t steps)
{
  Serial.print("Encoder "); Serial.print(i);
  Serial.print(" increment by "); Serial.println(steps);

  if (i == 0) {  // Outer left encoder
    modifyDistanceFromSoftReset(-10);
  }
  if (i == 1) {
    /*cycleScreen(DisplaySide::LEFT, 1);
    Serial.println("Cycled left screen forward");*/
    modifyDistanceFromSoftReset(-1000);
  }
  if (i == 2) {
    if (segBrightness + steps < 15) {
      segBrightness += steps;
    }
    else {
      segBrightness = 15;
    }
    segLeft.setBrightness(segBrightness);
    segRight.setBrightness(segBrightness);
  }
  else if (i == 3) {
    cycleScreen(DisplaySide::RIGHT, 1);
    Serial.println("Cycled right screen forward");
  }
}

static void handleEncoderDecrement(int i, int32_t steps)
{
  Serial.print("Encoder "); Serial.print(i);
  Serial.print(" decrement by "); Serial.println(steps);

  if (i == 0) {  // Outer left encoder
    modifyDistanceFromSoftReset(10);
  }
  if (i == 1) {
    /*cycleScreen(DisplaySide::LEFT, -1);
    Serial.println("Cycled left screen backward");*/
    modifyDistanceFromSoftReset(1000);
  }
  if (i == 2) {
    Serial.println(segBrightness);
    if (segBrightness + steps > 0) {
      segBrightness += steps;
    }
    else {
      segBrightness = 0;
    }
    segLeft.setBrightness(segBrightness);
    segRight.setBrightness(segBrightness);
  }
  else if (i == 3) {
    cycleScreen(DisplaySide::RIGHT, -1);
    Serial.println("Cycled right screen backward");
  }
}