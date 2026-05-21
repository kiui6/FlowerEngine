#pragma once

#include <cstdint>

struct SerialDependency
{
    uint16_t prefixIndex;
    uint64_t dependencyID;
};