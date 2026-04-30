#pragma once

#include "app/function_gen_recipe.h"

class SPIClass;

int HAL_SetupSPI();

// SPI pins connected to AD9833 and resistor
#define PIN_SPI_D_1        (uint8_t)23
#define PIN_SPI_CLK_1      (uint8_t)18
#define PIN_SPI_CS_1       (uint8_t)5

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