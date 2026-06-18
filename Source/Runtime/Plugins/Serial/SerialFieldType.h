#pragma once

#include <cstdint>

// WARN: Chaning existing order will break backwards compatibility

enum class SerialFieldType : uint32_t
{
    Bool = 0,
    Record = 1,
    List = 2,
    Map = 3,
    Tuple = 4,
    Pair = 5,
    String = 6,
    Text = 7,
    Trivial = 8,
    Trivial8 = 9,
    Trivial16 = 10,
    Trivial32 = 11,
    Trivial64 = 12,
};