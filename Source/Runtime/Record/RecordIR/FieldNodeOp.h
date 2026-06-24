#pragma once

#include <cstdint>

enum class FieldNodeOp : uint8_t
{
    Emplace,
    Insert,
    Remove,
};