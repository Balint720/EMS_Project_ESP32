#pragma once

enum Service_UUID_Index {
    FUNCTIONGEN
};

enum Char_UUID_Index {
    WAVEFORM,
    FREQUENCY,
    PULSEWIDTH,
    AMPLITUDE
};

constexpr const char *service_uuid_map[] = {
    "aba060d5-e5dd-42b6-862c-a97312ecb95b"
};

constexpr const char *char_uuid_map[] = {
    "c1ef4cb5-5c55-40a7-82b7-5df9443f6871",
    "ecf33cce-f166-4296-a8c0-7737dea4d5ba",
    "7329d245-fd1f-457a-832f-b1138b977b90",
    "b8dd73a1-29d1-4e25-b34d-e1e6eded3bbc"
};

