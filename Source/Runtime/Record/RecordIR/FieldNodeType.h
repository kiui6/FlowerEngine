#pragma once

#include <cstdint>

enum class FieldNodeType : uint8_t
{
    Integer     = 0,
    Unsigned    = 1,
    Float       = 2,
    Double      = 3,
    Bool        = 4,
    Record      = 5,
    List        = 6,
    Map         = 7,
    Tuple       = 8,
    Pair        = 9,
    String      = 10,
    Text        = 11,
    Trivial     = 12,

    MAX         = 0xFF
};