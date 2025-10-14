#include <Arduino.h>
#include <Button.h>
#include "counter.h"

Counter::Counter()
    : value(0),
      paused(true),
      lastCountTime(0),
      runInterval(0) {}
  
void Counter::print0b(uint8_t value) {
        for (int bit = 7; bit >= 0; bit--) {
          Serial.print((value >> bit) & 0x01 ? "1" : "0");
        }
      }      
  
void Counter::run() {
        if (!paused) {
          int potValue = analogRead(BUTTON_PIN);
          runInterval = map(potValue, 10, 4085, 15, 1000);
  
          if (millis() - lastCountTime >= runInterval) {
            increment();
            lastCountTime = millis();
          }
        }
      }
  
void Counter::reset(String msg) {
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
      
void Counter::increment(String msg) {
        value++;
        writeByteToShiftRegister(value);
        if (msg.length() > 0) {
          Serial.print(msg);
          Serial.print(" ");
        } else {
          Serial.print("Auto-increment [");
          Serial.print(runInterval);
          Serial.print("ms] ");
        }
        Serial.print("0b");
        print0b(value);
        Serial.printf(" (%d)\n", value);
      }
  
uint8_t Counter::getValue() { return value; }

void Counter::togglePause() { paused = !paused; }