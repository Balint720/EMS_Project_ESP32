#include <Arduino.h>

#include "app/http_reqs.h"
#include "hal/hal.h"
#include "app/app.h"

#include "sys/sys.h"

void setup() {
  
  pinMode(21, OUTPUT);

  analogWrite(21, 128);

  pinMode(2, INPUT);

  
  // Serial
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello world!");
  debug_print("Debug print mode on!\n");

  HAL_Setup();
  APP_Setup();

  /*
  // SPI Flash test code
  funcGenRecipe_st testRecipe = {
    2,
    {
      {
        SQUARE,
        10,
        10,
        2.5
      },
      {
        TRIANGLE,
        22,
        23,
        5.2
      }
    }
  };
  debug_print("Attempting to save recipe to flash memory...\n");
  HAL_SaveRecipeToFlash(&testRecipe, 0);

  debug_print("Attempting to read recipe from flash memory...\n");
  funcGenRecipe_st out;
  HAL_LoadRecipeFromFlash(0, &out);
  */
}

void loop() {
  HAL_Loop();
  APP_Loop();
}