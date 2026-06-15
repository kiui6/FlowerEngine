#pragma once

#include <cstdint>

enum class FieldType : uint8_t
{
    None,
    Bool,
    Trivial,
    Record,
    List,
    Tuple,
    Pair,
    String,
    Text,
};