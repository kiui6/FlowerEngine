#pragma once

#include <Utility/ID.h>

#include <Utility/Types.h>

constexpr u32 CURRENT_PLUGIN_FILE_VERSION = 0x00;

struct SerialHeader
{
    u32 magic = 0xEDEDEDED;
    u32 version = CURRENT_PLUGIN_FILE_VERSION;
    u64 pluginID = 0;

    u64 recordsLutCount = 0;
    u64 recordsLutOffset = 0;

    u64 recordsBlobSize = 0;
    u64 recordsBlobOffset = 0;

    u64 dependenciesOffset = 0;
    u16 dependencyCount = 0;

    u16 infoSectionOffset = 0;

    u32 checksum = 0;
};

static_assert(sizeof(SerialHeader) % alignof(SerialHeader) == 0);
static_assert(sizeof(SerialHeader) == 64);