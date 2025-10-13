#include <Arduino.h>

// interface
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

class Button {
  private:
    int pin;
    int lastState;
    int currState;
    int prevState;
    unsigned long lastDebounceTime;
    const int DEBOUNCE_DELAY = 50;

  public:
    Button(int pin) {
      this->pin = pin;
      lastState = HIGH;
      currState = HIGH;
      prevState = HIGH;
      lastDebounceTime = 0;
    }

    void updateState() {
      int reading = digitalRead(pin);
      if (reading != lastState) {
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != currState) {
          prevState = currState;
          currState = reading;
        }
      }
      lastState = reading;
    }
    
    bool wasPressed() {
      if (prevState == HIGH && currState == LOW) {
        prevState = LOW;
        return true;
      }

      if (prevState == LOW && currState == HIGH) {
        prevState = HIGH;
      }
      return false;
    }

    bool isPressed() {
      return currState == LOW;
    }
};

class Counter {
  private:
    uint8_t value;
    bool paused;
    unsigned long lastCountTime;
    int interval;

    void print0b(uint8_t value) {
      for (int bit = 7; bit >= 0; bit--) {
        Serial.print((value >> bit) & 0x01 ? "1" : "0");
      }
    }

  public:
    Counter(bool isPaused) {
      value = 0;
      this->paused = isPaused;
      lastCountTime = 0;
      interval = 0;
    }

    void run() {
      if (!paused) {
        int potValue = analogRead(POT_PIN);
        interval = map(potValue, 10, 4085, 15, 1000);
        this->interval = interval;

        if (millis() - lastCountTime >= interval) {
          increment();
          lastCountTime = millis();
        }
      }
    }

    void reset(String msg = "") {
      value = 0;
      writeByteToShiftRegister(value);
      if (msg.length() > 0) {
        Serial.print(msg);
        Serial.print(" ");
      }
      Serial.print("0b");
      print0b(value);
      Serial.printf(" (%d)\n", value);
      
    }
    
    void increment(String msg = "") {
      value++;
      writeByteToShiftRegister(value);
      if (msg.length() > 0) {
        Serial.print(msg);
        Serial.print(" ");
      } else {
        Serial.print("Auto-increment [");
        Serial.print(this->interval);
        Serial.print("ms] ");
      }
      Serial.print("0b");
      print0b(value);
      Serial.printf(" (%d)\n", value);
    }

    uint8_t getValue() { return value; }

    bool isPaused() { return paused; }
    void togglePause() { paused = !paused; }
};

bool startPaused = true;
Button pauseButton(PAUSE_PIN);
Button resetButton(RESET_PIN);
Button incrButton(INCR_PIN);
Counter counter(startPaused);

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
  pauseButton.updateState();
  resetButton.updateState();
  incrButton.updateState();

  counter.run();

  if (pauseButton.wasPressed()) { 
    counter.togglePause();
    if (counter.isPaused()) {
      Serial.println("[Counter paused]");
    } else {
      Serial.println("[Counter resumed]");
    }
  }
  
  if (resetButton.wasPressed() && counter.isPaused()) {
    counter.reset("[Reset]");
  }

  if (incrButton.wasPressed() && counter.isPaused()) {
    counter.increment("[Increment]");
  }
}