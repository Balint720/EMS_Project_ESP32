#include "app/function_gen_recipe.h"

#include "hal/hal.h"
#include "sys/convert.h"

#ifndef MAX_RECIPE_STEP_COUNT
    #warning MAX_RECIPE_STEP_COUNT is undefined: Default count of 20 is used
    #define MAX_RECIPE_STEP_COUNT 20
#endif

static struct funcGenRecipe_st *currRecipe;
static int selectedStep = 0;
static int selectedRecipe = 0;

static unsigned long int lastCheck = millis();

/*
*   Saving values to bluetooth values
*/
static void UpdateBtValsToVars() {
    funcGenVars_st *helper = getCurrentRecipeStep();
    HAL_BT_SetValueDouble(Service_UUID_Index::RECIPE, Char_UUID_Index::RECIPE_AMPLITUDE, helper->amp);
    HAL_BT_SetValueInt(Service_UUID_Index::RECIPE, Char_UUID_Index::RECIPE_PULSEWIDTH, helper->pulseWidth);
    HAL_BT_SetValueDouble(Service_UUID_Index::RECIPE, Char_UUID_Index::RECIPE_FREQUENCY, helper->freq);
    HAL_BT_SetValueInt(Service_UUID_Index::RECIPE, Char_UUID_Index::RECIPE_WAVEFORM, (int)helper->waveform);
    HAL_BT_SetValueInt(Service_UUID_Index::RECIPE, Char_UUID_Index::RECIPE_TIME, getCurrentStepTime());
    HAL_BT_SetValueInt(Service_UUID_Index::SETTINGS, Char_UUID_Index::RECIPE_INFO, combine8to16(selectedRecipe, selectedStep));
}

void APP_SetupGlobalRecipe() {
    currRecipe = new funcGenRecipe_st();
    selectedStep = 0;
    selectedRecipe = 0;
    int x = HAL_LoadRecipeFromFlash(selectedRecipe, currRecipe);
    if (x == 0) {
        debug_print("Recipe found at index 0, loaded.\n");
    }

    UpdateBtValsToVars();
}

void APP_LoopFuncGenRecipe() {
    unsigned long t = millis();
    if (lastCheck + 10000 < t) {
        lastCheck = t;
        UpdateBtValsToVars();
#if defined(PRINT_DEBUG_SERIAL)
        debug_print("\nCurrent Selected Recipe: %d\n", selectedRecipe);
        debug_print("Current Selected step: %d\n\n", selectedStep);
        funcGenVars_st *helper = getCurrentRecipeStep();
        if (helper == nullptr) {
            debug_print("Recipe step doesn't exist.\n");
        }
        else {
            debug_print("Amplitude: %f\n", helper->amp);
            debug_print("Pulse width: %d\n", helper->pulseWidth);
            debug_print("Frequency: %f\n", helper->freq);
            debug_print("Waveform: %d\n", helper->waveform);
            debug_print("Time: %d\n", getCurrentStepTime());
        }
#endif
    }
}

funcGenRecipe_st* getCurrentRecipe() {
    return currRecipe;
}

struct funcGenVars_st* getCurrentRecipeStep() {
    return getCurrentRecipeStepIndexed(selectedStep);
}

struct funcGenVars_st* getCurrentRecipeStepIndexed(unsigned int s) {
    if (s > currRecipe->stepCount - 1) {
        if (s == currRecipe->stepCount || s < MAX_RECIPE_STEP_COUNT) {
            debug_print("New empty step added\n");
            currRecipe->stepCount++;
        }
        else {
            debug_print("Step asked for is too large\n");
            return nullptr;
        }   
    }

    return &(currRecipe->recipe[s]);
}

time_t getCurrentStepTime() {
    return currRecipe->time[selectedStep];
}

time_t getCurrentStepTimeIndexed(unsigned int index) {
    if (index < MAX_RECIPE_STEP_COUNT) {
        return currRecipe->time[index];
    }
    else return 0;
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

int setCurrentStepTime(time_t newTime) {
    currRecipe->time[selectedStep] = newTime;
    return 0;
}

int saveCurrentRecipe() {
    return HAL_SaveRecipeToFlash(currRecipe);
}

int loadCurrentRecipe(int index) {
    return HAL_LoadRecipeFromFlash(index, currRecipe);
}

int getRecipeSelectionNumber() {
    return selectedRecipe;
}

int getRecipeStepNumber() {
    return selectedStep;
}

bool setRecipeSelectionNumber(unsigned int num) {
    if (num >= MAX_RECIPE_COUNT) {
        return false;
    }
    if (selectedRecipe != num) {
        selectedRecipe = num;
        if (HAL_LoadRecipeFromFlash(selectedRecipe, currRecipe) != 0) {
            debug_print("Didn't find recipe at index %d, creating empty one\n", selectedRecipe);
            *currRecipe = {};
            currRecipe->stepCount = 1;
            currRecipe->recipe[0].amp = 5.0;
            currRecipe->recipe[0].freq = 1.0;
            currRecipe->recipe[0].pulseWidth = 0;
            currRecipe->recipe[0].waveform = SINUS;
            currRecipe->time[0] = 100;
        }
        else {
            debug_print("Recipe successfully loaded.\n");
        }
    }
    
    return true;
}
bool setRecipeStepNumber(unsigned int num) {
    debug_print("Trying to set step number to %d\n", num);
    if (num >= currRecipe->stepCount) {
        if (num == currRecipe->stepCount) {
            debug_print("Given number was 1 larger than the last index.\n");
            debug_print("Creating new step...\n");
            currRecipe->stepCount++;
            selectedStep = num;
            currRecipe->recipe[selectedStep] = {  };
            currRecipe->time[selectedStep] = 0;
            return true;
        }
        else {
            debug_print("Given number was larger than the maximum step number. Maximum is %d\n", currRecipe->stepCount);
            return false;
        }
    }
    selectedStep = num;
    debug_print("Step number set successful.");
    return true;
}


/*
*   Recipe commands
*/
#define SET_STEP_COUNT          0x10
#define START_RECIPE            0xA1
#define ABORT_RECIPE            0xAF
#define SAVE_RECIPE_TO_FLASH    0xCC

bool actionOnRecipe(uint8_t command, uint8_t value) {
    switch (command) {
        case SET_STEP_COUNT:
            if (value <= MAX_RECIPE_STEP_COUNT && value > 0) {
                currRecipe->stepCount = value;
                debug_print("Stepcount set to %d.\n", currRecipe->stepCount);
                return true;
            }
            else {
                debug_print("Stepcount unchanged. Value gotten: %d\n", value);
                return false;
            }
            return true;
        case START_RECIPE:
        {
            bool x = (startRecipe() == 0);
            if (x) {
                debug_print("Recipe started.\n");
            } 
            else {
                debug_print("Recipe start failed.\n");
            }
            return x;
        }
        case ABORT_RECIPE:
            return (stopRecipe() == 0);
        case SAVE_RECIPE_TO_FLASH:
            if (HAL_SaveRecipeToFlash(currRecipe, selectedRecipe) == 0) {
                debug_print("Recipe save to %d index successful.\n", selectedRecipe);
                return true;
            }
            else {
                debug_print("Recipe save to %d index failed.\n", selectedRecipe);
                return false;
            }
    }
    return false;
}