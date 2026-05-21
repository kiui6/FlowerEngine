#pragma once

#include <Utility/ID.h>

#include <cstddef>

struct SerialField
{
    ID32 id;
    uint32_t dataSize;
    std::byte* data;
};