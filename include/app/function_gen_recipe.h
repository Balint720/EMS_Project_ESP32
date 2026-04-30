#pragma once

#include "function_gen_vars.h"

struct funcGenRecipe_st {
    unsigned int stepCount;
    funcGenVars_t recipe[MAX_RECIPE_STEP_COUNT];
};

void APP_SetupGlobalRecipe();

struct funcGenVars_t* getCurrentRecipeSteps(unsigned int *);
int setCurrentRecipe(struct funcGenRecipe_st);
int saveCurrentRecipe();
int loadCurrentRecipe(int);
