// encoders.h
#pragma once

#include <Adafruit_seesaw.h>

enum class EncoderEventType {
  NONE,
  INCREMENT,
  DECREMENT,
  BUTTON_SINGLE_PRESS,
  BUTTON_DOUBLE_PRESS,
  BUTTON_LONG_PRESS,
};

struct EncoderState {
  int32_t lastPosition = 0;
  bool buttonPressed   = false;
  // Use these to handle multi-press detection, timing, etc.
};

extern Adafruit_seesaw encoders[4];
extern EncoderState encoderStates[4];

bool initEncoders();
void updateEncoders();