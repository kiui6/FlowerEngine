#pragma once

#include <Utility/ID.h>

#include <Utility/Types.h>

#include <Record/RecordFieldMemory.h>

struct SerialField
{
    u32 id;
    FieldType type;
};

static_assert(sizeof(SerialField) % alignof(SerialField) == 0);
static_assert(sizeof(SerialField) == 8);