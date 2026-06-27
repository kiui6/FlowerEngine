#pragma once

#include <Record/RecordID.h>

#include <Utility/Types.h>

enum class SerialRecordFlags : u16 {
    Deleted = 1 << 0,
    Delta = 1 << 1,
};

inline bool operator&(SerialRecordFlags a, SerialRecordFlags b) {
    return (static_cast<u16>(a) & static_cast<u16>(b)) != 0;
}

struct SerialLUTEntry
{
    u64 id;
    u64 offset;
    u32 type;
    SerialRecordFlags flags;
    u16 fieldsCount;
};

static_assert(sizeof(SerialLUTEntry) % alignof(SerialLUTEntry) == 0);
static_assert(sizeof(SerialLUTEntry) == 24);