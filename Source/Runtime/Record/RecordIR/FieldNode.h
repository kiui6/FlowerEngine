#pragma once

#include "FieldNodeType.h"
#include "FieldNodeOp.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    FieldNodeType type;
    FieldNodeOp op;
    uint16_t size;
    uint16_t elementNumber;
    uint16_t pad;
    union {
        bool     boolValue;
        uint32_t  integerValue;
        float    floatValue;
        double   doubleValue;
        const char*    stringValue;
        FieldNode* children;
    } data;
};

static_assert(sizeof(FieldNode) == 16, "FieldNode size must be 16 bytes");