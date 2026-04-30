#include "app/function_gen_recipe.h"

#include "hal/hal.h"

#ifndef MAX_RECIPE_STEP_COUNT
    #warning MAX_RECIPE_STEP_COUNT is undefined: Default count of 20 is used
    #define MAX_RECIPE_STEP_COUNT 20
#endif

static struct funcGenRecipe_st *currRecipe;

void APP_SetupGlobalRecipe() {
    currRecipe = new funcGenRecipe_st();
}

funcGenRecipe_st getCurrentRecipe() {
    return *currRecipe;
}

int setCurrentRecipe(const funcGenRecipe_st *newRecipe) {
    if (newRecipe->stepCount > MAX_RECIPE_STEP_COUNT) {
        return -1;
    }
    for (int i = 0; i < newRecipe->stepCount; i++) {
        currRecipe->recipe[i] = newRecipe->recipe[i];
    }
    currRecipe->stepCount = newRecipe->stepCount;

    return 0;
}

int saveCurrentRecipe() {
    return HAL_SaveRecipeToFlash(currRecipe);
}

int loadCurrentRecipe(int index) {
    return HAL_LoadRecipeFromFlash(index, currRecipe);
}