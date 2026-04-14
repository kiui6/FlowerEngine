#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct PluginFileHeader
{
    char signature[4] = {'P', 'L', 'U', 'G'};
    uint16_t versionMajor = 0;
    uint16_t versionMinor = 0;

    uint64_t pluginID = 0;
    uint64_t pluginVersion = 0;
    uint64_t dependenciesOffset = 0;
    uint64_t dependenciesCount = 0;

    uint64_t recordsOffset = 0;
    uint64_t recordsLutOffset = 0;
    uint64_t recordsLutCount = 0;
    
    uint64_t worldsOffset = 0;
    uint64_t worldsLutOffset = 0;
    uint64_t worldsLutCount = 0;

    uint64_t blackboardOffset = 0;
    uint64_t blackboardLutOffset = 0;
    uint64_t blackboardLutCount = 0;
};
#pragma pack(pop)

static_assert(sizeof(PluginFileHeader) == 112 && "Unexpected plugin file header size");