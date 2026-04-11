#pragma once

#include <cstdint>
#include <variant>

enum class BlackboardValueType : uint8_t
{
    Bool,
    Int,
    Float,
    ID
};

using BlackboardValue = std::variant<bool, int, float, uint32_t>;