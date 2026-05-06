#pragma once

enum Service_UUID_Index {
    FUNCTIONGEN,
    RECIPE,
    OUTPUTVALS
};

enum Char_UUID_Index {
    WAVEFORM,
    FREQUENCY,
    PULSEWIDTH,
    AMPLITUDE,
    FG_STATE,
    RECIPE_STEPCOUNT,
    RECIPE_WAVEFORM,
    RECIPE_FREQUENCY,
    RECIPE_PULSEWIDTH,
    RECIPE_AMPLITUDE,
    RECIPE_LENGTH,
    OUTPUT_ADCVALS
};


static const char * const service_uuid_map[] = {
    "aba060d5-e5dd-42b6-862c-a97312ecb95b",
    "104dad85-0de8-4284-8522-fe5d34fd3cec",
    "a2d089c9-c2d9-4eb4-8c67-92c7f6b50cbc"
};

static const char * const char_uuid_map[] = {
    "c1ef4cb5-5c55-40a7-82b7-5df9443f6871",
    "ecf33cce-f166-4296-a8c0-7737dea4d5ba",
    "7329d245-fd1f-457a-832f-b1138b977b90",
    "b8dd73a1-29d1-4e25-b34d-e1e6eded3bbc",
    "a119ebb7-da3b-4936-ac95-41ba18e51f8b",
    "3b60fc24-92af-44e1-b896-a35a83e647cc",
    "d17fac03-cbe4-41ef-8b3f-152d811f7317",
    "4f672f51-203d-4624-8052-bd16d6ed6081",
    "0183cc3c-c9db-4b8b-84c1-66e95b9931f6",
    "c1440e4c-de92-4968-86e0-9c007ef9840e",
    "39ee2507-93c0-4cb3-a831-e7a353c878ca",
    "247ad10e-d8b9-4f8b-9f89-c81299adfee2"
};
