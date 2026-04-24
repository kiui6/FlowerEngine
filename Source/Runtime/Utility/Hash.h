#pragma once

#include <cstdint>

constexpr uint64_t FNV1a64(const char* str, uint64_t hash = 14695981039346656037ULL) {
    while (*str) {
        hash ^= static_cast<uint8_t>(*str++);
        hash *= 1099511628211ULL;
    }
    return hash;
}