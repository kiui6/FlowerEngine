#pragma once

#include <cstdint>

constexpr uint32_t MakeRecordType(const char* str) {
    return (static_cast<uint32_t>(str[0]) << 0)  |
           (static_cast<uint32_t>(str[1]) << 8)  |
           (static_cast<uint32_t>(str[2]) << 16) |
           (static_cast<uint32_t>(str[3]) << 24);    
}

consteval uint32_t CompMakeRecordType(const char* str) {
    return (static_cast<uint32_t>(str[0]) << 0)  |
           (static_cast<uint32_t>(str[1]) << 8)  |
           (static_cast<uint32_t>(str[2]) << 16) |
           (static_cast<uint32_t>(str[3]) << 24);    
}

#define FIELDID(id) CompMakeRecordType( #id )