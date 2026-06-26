#pragma once

#include <cstdint>

enum class FieldNodeOp : uint8_t
{
    Insert = 0,
    Remove = 1,
    Move = 2,
    Modify = 3,
};