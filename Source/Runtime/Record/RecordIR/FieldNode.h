#pragma once

#include "FieldNodeType.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    union {
        bool     boolValue;
        int32_t  integerValue;
        float    floatValue;
        double   doubleVaue;
        char*    stringValue;
        FieldNode* children;
    } data;
    uint32_t size;
    FieldNodeType type;
    uint8_t flags;
};

static_assert(sizeof(FieldNode) == 16, "FieldNode size must be 16 bytes");