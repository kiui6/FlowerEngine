#pragma once

#include <cstddef>

struct RecordFieldMemory {
    size_t size = 0;
    std::byte* memory;
};