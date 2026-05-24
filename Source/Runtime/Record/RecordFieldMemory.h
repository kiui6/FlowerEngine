#pragma once

#include <cstddef>
#include <vector>
#include <memory>

enum class FieldType : uint32_t {
    Integer = 0,
    Unsigned = 1,
    Float = 2,
    Boolean = 3,
    String = 4,
    List = 5,
    Map = 6,
};

struct RecordFieldMemory {
    uint32_t size;
    FieldType type;
    std::unique_ptr<std::byte[]> data;
};