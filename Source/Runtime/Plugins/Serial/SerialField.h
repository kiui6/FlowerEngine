#pragma once

#include <Utility/ID.h>

#include <Utility/Types.h>

#include <Record/RecordIR/FieldNodeType.h>

struct SerialField
{
    u32 id;
    FieldNodeType type;
};

static_assert(sizeof(SerialField) % alignof(SerialField) == 0);
static_assert(sizeof(SerialField) == 8);