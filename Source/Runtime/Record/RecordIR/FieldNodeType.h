#pragma once

#include <cstdint>

enum class FieldNodeType : uint8_t
{
    None,
    Integer,
    Unsigned,
    Bool,
    Record,
    List,
    Map,
    Tuple,
    Pair,
    String,
    Text,
    Trivial,
};