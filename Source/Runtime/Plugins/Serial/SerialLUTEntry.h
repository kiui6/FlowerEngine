#pragma once

#include <Record/RecordID.h>

#include <Utility/Types.h>

constexpr uint16_t PLUGIN_SERIAL_RECORD_FLAG_DELETED = 1 << 0;

struct SerialLUTEntry
{
    u64 id;
    u64 offset;
    u32 type;
    u16 flags;
    u16 fieldsCount;
};

static_assert(sizeof(SerialLUTEntry) % alignof(SerialLUTEntry) == 0);
static_assert(sizeof(SerialLUTEntry) == 24);