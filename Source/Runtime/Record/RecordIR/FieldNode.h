#pragma once

#include "FieldNodeType.h"
#include "FieldNodeOp.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    FieldNodeType type = FieldNodeType::None;
    FieldNodeOp op = FieldNodeOp::Insert;
    uint16_t size = 0;
    uint16_t targetPosition = 0;
    uint16_t sourcePosition = 0;
    union {
        bool     boolValue;
        uint64_t  unsignedValue;
        int64_t  integerValue;
        float    floatValue;
        double   doubleValue;
        const char*    stringValue;
        FieldNode* children;
    } data;
};

static_assert(sizeof(FieldNode) == 16, "FieldNode size must be 16 bytes");