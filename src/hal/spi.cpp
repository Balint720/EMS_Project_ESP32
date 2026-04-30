#include "hal/spi.h"

#include "sys/sys.h"
#include <Arduino.h>
#include "app/function_gen_recipe.h"

#include <limits.h>

#include <SPI.h>

#define SPI_DATA_MAX_CHUNK_SIZE 1024

int HAL_SetupSPI() {
    setupSPI_A();
    setupSPI_Flash();

    return 0;
}

/*
*   SPI for AD9833 and Resistor
*/

SPIClass *SPI_A = nullptr;

void setupSPI_A() {
    SPI_A = new SPIClass();
    SPI_A->begin(
        PIN_SPI_CLK_1,
        -1,
        PIN_SPI_D_1,
        PIN_SPI_CS_1
    );
}

void endSPI1() {
    SPI_A->end();
}

/*
*   SPI Flash code
*/

static int headerCheck(int recipeIndex);

#ifndef MAX_RECIPE_STEP_COUNT
    #warning MAX_RECIPE_STEP_COUNT is undefined: Default count of 20 is used
    #define MAX_RECIPE_STEP_COUNT 60
#endif
#ifndef MAX_RECIPE_COUNT
    #warning MAX_RECIPE_COUNT is undefined: Default count of 20 is used
    #define MAX_RECIPE_COUNT 20
#endif
#if MAX_RECIPE_COUNT > 20
    #warning Recipe count over 20: multiple recipes are stored in one partition \
                In the current state, recipes will be erased in bulk
#endif

static bool* storedRecipe;

// Partition values
#define RECIPES_PARTITION      \
    ESP_PARTITION_TYPE_DATA,    \
    ESP_PARTITION_SUBTYPE_ANY, \
    "recipes"


// Partition of stored recipes
static const esp_partition_t *recip_part = nullptr;

void dumpPartitions() {
    Serial.println("=== Partition Table ===");

    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

    while (it != NULL) {
        const esp_partition_t *p = esp_partition_get(it);
        Serial.printf("Name: %-16s Type: 0x%02X Sub: 0x%02X Addr: 0x%06X Size: 0x%06X\n",
                      p->label, p->type, p->subtype, p->address, p->size);
        it = esp_partition_next(it);
    }

    Serial.println("========================");
}

int setupSPI_Flash() {
    dumpPartitions();
    recip_part = esp_partition_find_first(RECIPES_PARTITION);
    vTaskDelay(1);
    int ret = esp_flash_init(esp_flash_default_chip);
    vTaskDelay(1);

    if (ret == ESP_OK) {
        storedRecipe = new bool[MAX_RECIPE_COUNT];

        for (int i = 0; i < MAX_RECIPE_COUNT; i++) {
            if (headerCheck(i) != 0) break;
            else storedRecipe[i] = true;
        }

        return 0;
    }
    else {
        return -1;
    }
}

typedef uint32_t s_header_t;
static constexpr s_header_t SECTOR_HEADER = 0x20011026;
static constexpr size_t SECTOR_HEADER_SIZE = sizeof(s_header_t);

#if MAX_RECIPE_COUNT <= 20
    static size_t recipeSizeBytes() {   
        return 0x1000;                  
    }
#else
    static size_t recipeSizeBytes() {
        return sizeof(funcGenRecipe_st);
    }
#endif

static size_t recipeOffset(int recipeIndex) {
    return recipeIndex * recipeSizeBytes();
}

static int headerCheck(int recipeIndex) {
    if (recipeIndex >= MAX_RECIPE_COUNT) {
        debug_print("Recipe index exceeds max recipe count\n");
        return -2;
    }

    size_t offset = recipeOffset(recipeIndex);

    s_header_t headerCheck;
    esp_partition_read(recip_part, offset, &headerCheck, SECTOR_HEADER_SIZE);
    debug_print("Header value: %x\n", headerCheck);
    if (headerCheck != SECTOR_HEADER) {
        debug_print("Sector does not have header\n");
        return -1;
    }

    return 0;
}

static void printDebugError(esp_err_t err) {
        switch (err) {
        case ESP_OK:
            debug_print("Success\n");
            break;
        case ESP_ERR_INVALID_ARG:
            debug_print("Partition write failed: Offset exceeds partition size\n");
            break;
        case ESP_ERR_INVALID_STATE:
            debug_print("Partition write failed: Partition is not in the right state\n");
            break;
        case ESP_ERR_INVALID_SIZE:
            debug_print("Partition write failed: Partition size is not a multiple of the flash sector size\n");
            break;
        case ESP_ERR_NOT_FOUND:
            debug_print("Partition write failed: Partition not found\n");
            break;
        case ESP_ERR_NO_MEM:
            debug_print("Partition write failed: No memory\n");
            break;
        case ESP_FAIL:
            debug_print("Partition write failed: Unknown error\n");
            break;
        default:
            debug_print("Partition write failed: Unknown error\n");
            break;
    }
}

int writeRecipe(int recipeIndex, const funcGenRecipe_st *recipe) {
    if (recipeIndex >= MAX_RECIPE_COUNT) {
        debug_print("Recipe index exceeds max recipe count\n");
        return -2;
    }
    else if (recipeIndex == -1) {
        bool freeSpaceLeft = false;
        for (int i = 0; i < MAX_RECIPE_COUNT; i++) {
            if (storedRecipe[i] == false) {
                recipeIndex = i;
                freeSpaceLeft = true;
                break;
            }
        }
        if (!freeSpaceLeft) {
            debug_print("No more recipe space left and no index was given to overwrite\n");
            return -2;
        }
    }

    debug_print("Writing new recipe at index %d...\n", recipeIndex);

    funcGenRecipe_st buffer;

    // Copy user data
    buffer.stepCount = recipe->stepCount;
    for (int i = 0; i < MAX_RECIPE_STEP_COUNT; i++) {
        if (i < recipe->stepCount) buffer.recipe[i] = recipe->recipe[i];
        else buffer.recipe[i] = {SQUARE,0,0,0.0}; // pad unused entries
    }

    size_t offset = recipeOffset(recipeIndex);

    // Erase if recipe already exists
    if (storedRecipe[recipeIndex]) {
        debug_print("Recipe already stored in partition: erasing\n");
        esp_partition_erase_range(recip_part, offset, recipeSizeBytes());
        vTaskDelay(1);
    }

    // Write whole partition
    debug_print("Writing %x to offset %x\n", SECTOR_HEADER, offset);
    esp_err_t err = esp_partition_write(recip_part, offset, &SECTOR_HEADER, SECTOR_HEADER_SIZE);
    if (err != ESP_OK) {
        printDebugError(err);
        return -1;
    }
    vTaskDelay(1);
    debug_print("Writing recipe to offset %x\n", offset + SECTOR_HEADER_SIZE);

    offset += SECTOR_HEADER_SIZE;
    size_t offsetChunk = 0;
    size_t remainingBytes = sizeof(funcGenRecipe_st);
    while (remainingBytes > 0) {
        size_t chunk = remainingBytes > SPI_DATA_MAX_CHUNK_SIZE ? SPI_DATA_MAX_CHUNK_SIZE : remainingBytes;

        err = esp_partition_write(recip_part, offset + offsetChunk, (&buffer)+offsetChunk, chunk);

        if (err != ESP_OK) {
            printDebugError(err);
            return -1;
        }

        remainingBytes -= chunk;
        offsetChunk += chunk;

        // This is so other services are allowed to run between writes
        vTaskDelay(1);
    }

    storedRecipe[recipeIndex] = true;

    return 0;
}

int readRecipe(int recipeIndex, funcGenRecipe_st *outRecipe) {
    int x = headerCheck(recipeIndex);
    if (x == 0) {
        size_t offset = recipeOffset(recipeIndex) + SECTOR_HEADER_SIZE;
        size_t offsetChunk = 0;
        size_t remainingBytes = sizeof(funcGenRecipe_st);

        while (remainingBytes > 0) {
            size_t chunk = remainingBytes > SPI_DATA_MAX_CHUNK_SIZE ? SPI_DATA_MAX_CHUNK_SIZE : remainingBytes;

            esp_err_t err = esp_partition_read(recip_part, offset + offsetChunk, (outRecipe)+offsetChunk, chunk);

            if (err != ESP_OK) {
                printDebugError(err);
                return -1;
            }

            remainingBytes -= chunk;
            offsetChunk += chunk;

            // This is so other services are allowed to run between writes
            vTaskDelay(1);
        }

        return 0;
    }
    else return x;
}