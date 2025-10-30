#include <Arduino.h>
#include "button.h"
#include "counter.h"

const int PAUSE_PIN = 15;
const int RESET_PIN = 16;
const int INCR_PIN = 17;
const int DATA_PIN = 18;
const int CLOCK_PIN = 19;
const int LATCH_PIN = 22;
const int POT_PIN = 34;

Counter counter{};

Button pauseBtn{PAUSE_PIN};
Button resetBtn{RESET_PIN};
Button incrBtn{INCR_PIN};

void writeByteToShiftRegister(uint8_t value) {
  digitalWrite(LATCH_PIN, LOW);
  
  for (int bit = 7; bit >= 0; bit--) {
    bool bitValue = (value >> bit) & 0x01;
    digitalWrite(DATA_PIN, bitValue);
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
  }
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(PAUSE_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(INCR_PIN, INPUT_PULLUP);

  Serial.println("Controllable Binary Counter");
}

void loop() {
  pauseBtn.monitor();
  resetBtn.monitor();
  incrBtn.monitor();

  counter.run();

  if (pauseBtn.pressed()) { counter.togglePause(); }
  if (counter.paused) {
    if (resetBtn.pressed()) { counter.reset("[Reset]"); }
    if (incrBtn.pressed()) { counter.increment("[Increment]"); }
  }
}