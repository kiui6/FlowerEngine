#pragma once

#include <Utility/ID.h>

#include <Utility/Types.h>

#include <Record/RecordIR/FieldNodeType.h>

struct SerialField
{
    u32 id;
FieldNodeType type; // Prioritize IR conventions and types if possible
};

static_assert(sizeof(SerialField) % alignof(SerialField) == 0);
static_assert(sizeof(SerialField) == 8);