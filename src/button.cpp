#include "button.h"

Button::Button(int pin) 
  : pin(pin),
    state(HIGH),
    prevState(HIGH),
    pressHandled(false),
    updatedTime(0) {}

void Button::monitor() {
  int pinRead = digitalRead(pin);
  
  if (pinRead != prevState) {
    updatedTime = millis();
  }
  
  if ((millis() - updatedTime) > DEBOUNCE_DELAY) {
    if (pinRead != state) {
      state = pinRead;
      pressHandled = false;
    }
  }
  
  prevState = pinRead;
}

bool Button::pressed() {
  if (state == LOW && !pressHandled) {
    pressHandled = true;
    return true;
  }
  return false;
}