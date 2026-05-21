#pragma once

#include <Utility/ID.h>

#include <cstddef>

constexpr uint32_t CURRENT_PLUGIN_FILE_VERSION = 0x00;

struct SerialHeader
{
    uint32_t magic = 0xEDEDEDED;
    uint32_t version = CURRENT_PLUGIN_FILE_VERSION;
    uint64_t pluginID = 0;

    uint16_t dependencyCount = 0;
    uint64_t dependenciesOffset = 0;

    uint64_t recordsLutCount = 0;
    uint64_t recordsLutOffset = 0;

    uint64_t recordsBlobSize = 0;
    uint64_t recordsBlobOffset = 0;

    uint32_t checksum = 0;

    uint8_t padding[2];
};