#include <Arduino.h>

#include "app/http_reqs.h"
#include "hal/hal.h"
#include "app/app.h"

#include "sys/sys.h"

void setup() {
  // Serial
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello world!");
  debug_print("Debug print mode on!\n");

  HAL::HAL_Setup();
  APP_Setup();
  SYS_Setup();
}

void loop() {
  HAL::HAL_Loop();
  APP_Loop();
  SYS_Loop();
}