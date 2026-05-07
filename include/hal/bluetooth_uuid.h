#pragma once

enum Service_UUID_Index {
    FUNCTIONGEN,
    RECIPE,
    SETTINGS
};

enum Char_UUID_Index {
    WAVEFORM,
    FREQUENCY,
    PULSEWIDTH,
    AMPLITUDE,
    FG_STATE,
    RECIPE_INFO,
    RECIPE_ACTIONS,
    RECIPE_WAVEFORM,
    RECIPE_FREQUENCY,
    RECIPE_PULSEWIDTH,
    RECIPE_AMPLITUDE,
    RECIPE_LENGTH,
    OUTPUT_ADCVALS
};


static const char * const service_uuid_map[] = {
    "e4a12522-729a-4300-8a90-84e5e756da13",
    "d3a60a09-c456-4f3e-8205-44d929c5e8a3",
    "d38f43f4-5a56-494f-aff5-f0dc391288a2"
};

static const char * const char_uuid_map[] = {
    "58f8114f-22a1-4a10-b409-a9b290e3a2ff",
    "3c815742-b9f4-4df0-a597-bd6cd227f1f0",
    "bd9b5e0c-b2ca-4d41-887a-c3c2eb653165",
    "cc9eaa34-b893-483a-b705-e0af7ac69b76",
    "e639db72-204e-4514-b177-eced50101fde",
    "cb8ff116-ff70-49d0-8a1b-958166011d4a",
    "0026a2cb-6903-41f9-99bf-07e184d200c7",
    "149df178-2413-40bf-b318-8954380c0463",
    "0e879c82-570f-4ef3-81c6-237b53703321",
    "e24530be-8ca0-4b79-b0a6-153d509b7983",
    "122b4fc8-16dc-47ab-bcb2-94eb1c91f6cc",
    "f15023f5-0aaf-4f82-887c-1ea4279fe873",
    "50571d79-8414-4911-917b-94f49710bec5"
};
