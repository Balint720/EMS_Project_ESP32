#pragma once

#include "hal/bluetooth_uuid.h"

#include "app/function_gen_recipe.h"

#include <string>

/*
*   Base Hardware Abstraction Layer functions
*/
void HAL_Setup();
void HAL_Loop();
void HAL_ReactToValueChange(Char_UUID_Index);

/*
*   Bluetooth HAL functions
*/
std::string HAL_BT_GetValue(Service_UUID_Index, Char_UUID_Index);

void HAL_BT_SetValue(Service_UUID_Index, Char_UUID_Index, const char*);
void HAL_BT_SetValue(Service_UUID_Index, Char_UUID_Index, std::string);

/*
*   SPI HAL functions
*/

int HAL_SaveRecipeToFlash(const funcGenRecipe_st *recipe, int index = -1);
int HAL_LoadRecipeFromFlash(int, funcGenRecipe_st* out);