#pragma once

#include "function_gen_vars.h"
#include "sys/sys.h"

struct funcGenRecipe_st {
    unsigned int stepCount;
    funcGenVars_t recipe[MAX_RECIPE_STEP_COUNT];
};

enum RecipeVars {
    RV_WAVEFORM,
    RV_FREQUENCY,
    RV_AMPLTIUDE,
    RV_PULSEWIDTH
};

void APP_SetupGlobalRecipe();

struct funcGenVars_t* getCurrentRecipeSteps(unsigned int *);
struct funcGenRecipe_st* getCurrentRecipe();
int setCurrentRecipe(struct funcGenRecipe_st);
int saveCurrentRecipe();
int loadCurrentRecipe(int);

template <typename T>
int updateCurrentRecipe(unsigned int step, RecipeVars valType, T value) {
    funcGenRecipe_st* cr = getCurrentRecipe();
    if (step > cr->stepCount - 1) {
        if (step == cr->stepCount) cr->stepCount++;
        else {
            debug_print("Error: step number out of bounds\n");
            return -1;
        }
    }

    switch (valType) {
        case RV_WAVEFORM:
            cr->recipe[step].waveform = value;
            break;
        case RV_FREQUENCY:
            cr->recipe[step].freq = value;
        case RV_AMPLTIUDE:
            cr->recipe[step].amp = value;
        case RV_PULSEWIDTH:
            cr->recipe[step].pulseWidth = value;
        default:
            debug_print("Error: invalid variable type\n");
            return -1;
    }
}
