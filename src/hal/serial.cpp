#include <Arduino.h>

#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include "app/function_gen_vars.h"
#include "hal/hal.h"
#include "hal/pot.h"
#include "hal/funcgen_chip.h"

namespace SerialCommands {
    namespace Commands {
        static constexpr const char* kSet = "set";
        static constexpr const char* kReset = "reset";
        static constexpr const char* kOn = "on";
        static constexpr const char* kOff = "off";
        static constexpr const char* kRead = "read";
    };
    namespace ValueNames {
        static constexpr const char* kFrequency = "frequency";
        static constexpr const char* kPulsewidth = "pulsewidth";
        static constexpr const char* kAmplitude = "amplitude";
        static constexpr const char* kWaveform = "waveform";
        static constexpr const char* kResistance = "resistance";
    };
};

static void readSerialCommands();

static void printFuncGenVars() {
    Serial.printf("\n");
    Serial.printf("Amplitude: %f\n", getAmp());
    Serial.printf("Pulse width: %d\n", getPulseWidth());
    Serial.printf("Frequency: %f\n", getFreq());
    Serial.printf("Waveform: %d\n", getWaveform());
}

static void printFuncGenRecipeVars() {
    Serial.printf("\nCurrent Selected Recipe: %d\n", getRecipeSelectionNumber());
    Serial.printf("Current Selected step: %d\n\n", getRecipeStepNumber());
    const funcGenVars_st *helper = getCurrentRecipeStep();
    if (helper == nullptr) {
        Serial.printf("Recipe step doesn't exist.\n");
    }
    else {
        Serial.printf("Amplitude: %f\n", helper->amp);
        Serial.printf("Pulse width: %d\n", helper->pulseWidth);
        Serial.printf("Frequency: %f\n", helper->freq);
        Serial.printf("Waveform: %d\n", helper->waveform);
        Serial.printf("Time: %lld\n", getCurrentStepTime());
    }
}

static void printPotMeterRes() {
    Serial.printf("\n");
    Serial.printf("Resistance: %f\n", getValueOfResistance());
}

static bool parseNumber(const char* text, double* value) {
    char* end = nullptr;
    errno = 0;
    const double parsed = strtod(text, &end);
    if (end == text || *end != '\0' || errno == ERANGE || !std::isfinite(parsed)) {
        return false;
    }

    *value = parsed;
    return true;
}

static Errors setSerialValue(const char* name, const char* text, Char_UUID_Index* changedValue) {
    if (strcmp(name, SerialCommands::ValueNames::kFrequency) == 0) {
        double value;
        if (!parseNumber(text, &value)) return INVALID_INPUT;
        *changedValue = FREQUENCY;
        return setFreq(value);
    }

    if (strcmp(name, SerialCommands::ValueNames::kPulsewidth) == 0) {
        if (*text == '-') return INVALID_INPUT;
        char* end = nullptr;
        errno = 0;
        const unsigned long value = strtoul(text, &end, 10);
        if (end == text || *end != '\0' || errno == ERANGE || value > UINT_MAX) {
            return INVALID_INPUT;
        }
        *changedValue = PULSEWIDTH;
        return setPulseWidth(static_cast<unsigned int>(value));
    }

    if (strcmp(name, SerialCommands::ValueNames::kAmplitude) == 0) {
        double value;
        if (!parseNumber(text, &value)) return INVALID_INPUT;
        *changedValue = AMPLITUDE;
        return setAmp(value);
    }

    if (strcmp(name, SerialCommands::ValueNames::kWaveform) == 0) {
        Waveform waveform;
        if (strcmp(text, "sinus") == 0) waveform = SINUS;
        else if (strcmp(text, "square") == 0) waveform = SQUARE;
        else if (strcmp(text, "triangle") == 0) waveform = TRIANGLE;
        else if (strcmp(text, "sawtooth") == 0) waveform = SAWTOOTH;
        else return INVALID_INPUT;

        *changedValue = WAVEFORM;
        return setWaveform(waveform);
    }

    if (strcmp(name, SerialCommands::ValueNames::kResistance) == 0) {
        double value;
        if (!parseNumber(text, &value)) return INVALID_INPUT;
        *changedValue = RESISTANCE;
        return setValueOfResistanceScaled(value);
    }

    return INVALID_INPUT;
}

static void resetFunctionGenerator() {
    resetAD9833();
    /*turnOff();
    setWaveform(SQUARE);
    setPulseWidth(0);
    setFreq(1);
    setAmp(5.0);

    HAL_ReactToValueChange(FG_STATE);
    HAL_ReactToValueChange(WAVEFORM);
    HAL_ReactToValueChange(PULSEWIDTH);
    HAL_ReactToValueChange(FREQUENCY);
    HAL_ReactToValueChange(AMPLITUDE);
    */
}

void HAL_LoopSerialRead() {
    readSerialCommands();
}

static void readSerialCommands() {
    if (Serial.available() <= 0) return;

    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 0) return;
    if (input.length() >= 128) {
        Serial.println("Command is too long.");
        return;
    }

    char line[128];
    input.toCharArray(line, sizeof(line));

    char* saveptr = nullptr;
    char* command = strtok_r(line, " \t", &saveptr);
    if (command == nullptr) return;
    char* valuename = strtok_r(nullptr, " \t", &saveptr);
    char* value = strtok_r(nullptr, " \t", &saveptr);
    char* extra = strtok_r(nullptr, " \t", &saveptr);

    if (strcmp(command, SerialCommands::Commands::kOn) == 0 ||
        strcmp(command, SerialCommands::Commands::kOff) == 0) {
        if (valuename != nullptr) {
            Serial.println("The on and off commands do not take arguments.");
            return;
        }

        const bool turnOnRequested = strcmp(command, SerialCommands::Commands::kOn) == 0;
        const Errors result = turnOnRequested ? turnOn() : turnOff();
        if (result == NO_ERROR) {
            HAL_ReactToValueChange(FG_STATE);
            Serial.println(turnOnRequested ? "Turned on successfully." : "Turned off successfully.");
        } else {
            Serial.println("Error changing output state.");
        }
        return;
    }

    if (strcmp(command, SerialCommands::Commands::kRead) == 0) {
        printFuncGenVars();
        printPotMeterRes();
        return;
    }

    if (strcmp(command, SerialCommands::Commands::kReset) == 0) {
        if (valuename != nullptr) {
            Serial.println("The reset command does not take arguments.");
            return;
        }
        resetFunctionGenerator();
        Serial.println("Function generator reset to startup defaults.");
        printFuncGenVars();
        return;
    }

    if (strcmp(command, SerialCommands::Commands::kSet) != 0) {
        Serial.println("Unknown command. Use set, reset, on, or off.");
        return;
    }
    if (valuename == nullptr || value == nullptr || extra != nullptr) {
        Serial.println("Usage: set <frequency|pulsewidth|amplitude|waveform|resistance> <value>");
        return;
    }

    Char_UUID_Index changedValue;
    const Errors result = setSerialValue(valuename, value, &changedValue);
    if (result != NO_ERROR) {
        Serial.println("Invalid value or value is outside the allowed range.");
        return;
    }

    HAL_ReactToValueChange(changedValue);
    Serial.println("Value updated successfully.");
    printFuncGenVars();
    printPotMeterRes();
}
