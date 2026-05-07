#pragma once

#include <cstdint>

using ID32 = uint32_t;

constexpr ID32 MakeID32(const char* str) {
    return (static_cast<ID32>(str[0]) << 0)  |
           (static_cast<ID32>(str[1]) << 8)  |
           (static_cast<ID32>(str[2]) << 16) |
           (static_cast<ID32>(str[3]) << 24);    
}