#ifndef COUNTER_H
#define COUNTER_H

#include <Arduino.h>

class Counter {
  private:
    uint8_t value;
    unsigned long lastCountTime;
    int runInterval;
    
    void print0b(uint8_t value);

  public:
    bool paused;
    
    Counter();
    void run();
    void reset(String msg = "");
    void increment(String msg = "");
    void togglePause();
    uint8_t getValue();
};

#endif