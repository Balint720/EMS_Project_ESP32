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

struct funcGenVars_st* getCurrentRecipeStep();
struct funcGenVars_st* getCurrentRecipeStepIndexed(unsigned int);
struct funcGenRecipe_st* getCurrentRecipe();
time_t getCurrentStepTime();
time_t getCurrentStepTimeIndexed(unsigned int index);

int setCurrentRecipe(const struct funcGenRecipe_st&);
int setCurrentStepTime(time_t newTime);
int saveCurrentRecipe();
int loadCurrentRecipe(int);

int getRecipeSelectionNumber();
int getRecipeStepNumber();
bool setRecipeSelectionNumber(unsigned int);
bool setRecipeStepNumber(unsigned int);

bool actionOnRecipe(uint8_t command, uint8_t value);

template <typename T>
int updateCurrentRecipe(unsigned int step, RecipeVars valType, T value) {
    funcGenVars_st* currStep = getCurrentRecipeStep();
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
