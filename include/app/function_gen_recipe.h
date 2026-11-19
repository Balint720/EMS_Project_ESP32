#pragma once

#include "function_gen_vars.h"
#include "sys/sys.h"

struct funcGenRecipe_st {
    unsigned int stepCount;
    funcGenVars_st recipe[MAX_RECIPE_STEP_COUNT];
    time_t time[MAX_RECIPE_STEP_COUNT];
};

enum RecipeVars {
    RV_WAVEFORM,
    RV_FREQUENCY,
    RV_AMPLTIUDE,
    RV_PULSEWIDTH,
    RV_TIME
};

void APP_SetupGlobalRecipe();
void APP_LoopFuncGenRecipe();

struct funcGenVars_st* APP_GetCurrentRecipeStep();
struct funcGenVars_st* APP_GetCurrentRecipeStepIndexed(unsigned int);
struct funcGenRecipe_st* APP_GetCurrentRecipe();
time_t APP_GetCurrentStepTime();
time_t APP_GetCurrentStepTimeIndexed(unsigned int index);

int APP_SetCurrentRecipe(const struct funcGenRecipe_st&);
int APP_SetCurrentStepTime(time_t newTime);
int APP_SaveCurrentRecipe();
int APP_LoadCurrentRecipe(int);

int APP_GetRecipeSelectionNumber();
int APP_GetRecipeStepNumber();
bool APP_SetRecipeSelectionNumber(unsigned int);
bool APP_SetRecipeStepNumber(unsigned int);

bool APP_ActionOnRecipe(uint8_t command, uint8_t value);

template <typename T>
int APP_UpdateCurrentRecipe(unsigned int step, RecipeVars valType, T value) {
    funcGenVars_st* currStep = APP_GetCurrentRecipeStep();
    if (currStep == nullptr) return -1;

    switch (valType) {
        case RV_WAVEFORM:
            currStep->waveform = value;
            break;
        case RV_FREQUENCY:
            currStep->freq = value;
        case RV_AMPLTIUDE:
            currStep->amp = value;
        case RV_PULSEWIDTH:
            currStep->pulseWidth = value;
        default:
            debug_print("Error: invalid variable type\n");
            return -1;
    }
}
