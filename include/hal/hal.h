#pragma once

#include "hal/bluetooth_uuid.h"

#include "app/function_gen_recipe.h"

#include <string>

/*
*   Base Hardware Abstraction Layer functions
*/
void    HAL_Setup();
void    HAL_Loop();
void    HAL_ReactToValueChange(Char_UUID_Index);

/*
*   Bluetooth HAL functions
*/
int     HAL_BT_GetValueInt(Service_UUID_Index, Char_UUID_Index);
float   HAL_BT_GetValueFloat(Service_UUID_Index, Char_UUID_Index);
double  HAL_BT_GetValueDouble(Service_UUID_Index, Char_UUID_Index);

void    HAL_BT_SetValueInt(Service_UUID_Index, Char_UUID_Index, int);
void    HAL_BT_SetValueFloat(Service_UUID_Index, Char_UUID_Index, float);
void    HAL_BT_SetValueDouble(Service_UUID_Index, Char_UUID_Index, double);

/*
*   SPI HAL functions
*/

int     HAL_SaveRecipeToFlash(const funcGenRecipe_st *recipe, int index = -1);
int     HAL_LoadRecipeFromFlash(int, funcGenRecipe_st* out);