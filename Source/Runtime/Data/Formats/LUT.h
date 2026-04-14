#pragma once

#include <cstdint>

#pragma pack(push, 1)
template <typename Key, typename Value>
struct LookupTableEntry {
    Key key;
    Value value;
};
#pragma pack(pop)