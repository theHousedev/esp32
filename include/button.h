#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  private:
    int pin;
    int state;
    int prevState;
    bool pressHandled;
    unsigned long updatedTime;
    const int DEBOUNCE_DELAY = 50;

  public:
    Button(int pin);
    void monitorButton();
    bool pressed();
};

#endif