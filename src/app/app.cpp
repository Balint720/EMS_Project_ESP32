#include "app/app.h"

#include "app/function_gen_vars.h"
#include "app/function_gen_recipe.h"

void APP_SetupGlobals() {
    APP_SetupGlobalFuncGenVars();
    APP_SetupGlobalRecipe();
}

void APP_Setup() {
    APP_SetupGlobals();
    APP_SetupFuncGen();
}

void APP_Loop() {
    APP_LoopFuncGen();
    APP_LoopFuncGenRecipe();
}