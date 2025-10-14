#include <Arduino.h>

// interface buttons
const int PAUSE_PIN = 15;
const int RESET_PIN = 16;
const int INCR_PIN = 17;

// shift register
const int DATA_PIN = 18;
const int CLOCK_PIN = 19;
const int LATCH_PIN = 22;
const int POT_PIN = 34;

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

Button pause{PAUSE_PIN};
Button reset{RESET_PIN};
Button incr{INCR_PIN};

Counter counter{};

void setup() {
  Serial.begin(115200);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  pinMode(PAUSE_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(INCR_PIN, INPUT_PULLUP);

  Serial.println("74HC595 Binary Counter Test");
}

void loop() {
  counter.run();

  pause.monitor();
  reset.monitor();
  incr.monitor();

  if (pause.pressed()) {
    counter.togglePause();

    if (counter.paused) {
      Serial.println("[Counter paused]");
    } else {
      Serial.println("[Counter resumed]");
    }
  }

  if (counter.paused) {
    if (reset.pressed()) {
      counter.reset("[Reset]");
    }

    if (incr.pressed()) {
      counter.increment("[Increment]");
    }
  }
}