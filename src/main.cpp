#include <Arduino.h>
#include "config.h"
#include <test_lib.h>
#define SERIAL_DELAY 1000

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {

  Serial.println("Testing Serial Communication");
  
  delay(SERIAL_DELAY);

  Serial.println("Testing Custom Library");

  print_testpin();
}

