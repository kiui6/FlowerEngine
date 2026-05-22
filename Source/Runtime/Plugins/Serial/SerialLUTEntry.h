#pragma once

#include <Record/RecordID.h>

#include <Utility/Types.h>

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