#include <Arduino.h>

#include "app/http_reqs.h"
#include "hal/hal.h"
#include "app/app.h"

void setup() {
  
  pinMode(21, OUTPUT);

  analogWrite(21, 128);
  
  // Serial
  Serial.begin(9600);


  HAL_Setup();
  APP_Setup();

}

void loop() {
  HAL_Loop();
  APP_Loop();
  
}