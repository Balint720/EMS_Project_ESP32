#pragma once

#include "app/function_gen_recipe.h"

class SPIClass;

int     HAL_SetupSPI();
int     HAL_SaveRecipeToFlash(const funcGenRecipe_st *recipe, int index = -1);
int     HAL_LoadRecipeFromFlash(int, funcGenRecipe_st* out);

/*
*   SPI for AD9833 and Resistor
*/
extern SPIClass *SPI_A;
void setupSPI_A();

/*
*   SPI Flash code
*/

int setupSPI_Flash();

int readRecipe(int recipeIndex, funcGenRecipe_st *outRecipe);
int writeRecipe(int recipeIndex, const funcGenRecipe_st *recipe);