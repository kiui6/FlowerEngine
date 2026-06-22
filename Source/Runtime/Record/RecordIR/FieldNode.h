#pragma once

#include "FieldNodeType.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    FieldNodeType type;
    uint8_t flags;
    uint8_t pad[2];
    uint32_t size;
    union {
        bool     boolValue;
        int32_t  integerValue;
        float    floatValue;
        double   doubleVaue;
        char*    stringValue;
        FieldNode* children;
    } data;
};

static_assert(sizeof(FieldNode) == 16, "FieldNode size must be 16 bytes");