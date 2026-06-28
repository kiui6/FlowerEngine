#pragma once

#include <cstdint>

#include <Utility/Types.h>

struct SerialDependency
{
    u64 pluginOrderID;
    u64 sourceID;
};

static_assert(sizeof(SerialDependency) % alignof(SerialDependency) == 0);
static_assert(sizeof(SerialDependency) == 16);