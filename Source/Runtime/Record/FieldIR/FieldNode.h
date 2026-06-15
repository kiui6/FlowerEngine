#pragma once

#include "FieldType.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    FieldType type;
};

struct FieldNodeTrivial : FieldNode 
{
    uint32_t size;
    void* data;
};

struct FieldNodeList : FieldNode {
    FieldNode** elements;
    uint32_t count;
    FieldType elementType;
};

struct FieldNodePair : FieldNode {
    FieldNode* first;
    FieldNode* second;
};

struct FieldNodeTuple : FieldNode {
    uint8_t count;
    FieldNode** elements;
};