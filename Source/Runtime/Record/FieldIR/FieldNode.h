#pragma once

#include "FieldNodeType.h"
#include <cstdint>
#include <vector>

struct FieldNode {
    FieldNodeType type;
};

struct FieldNodeTrivial : FieldNode 
{
    uint32_t size;
    void* data;
};

struct FieldNodeList : FieldNode {
    FieldNode** elements;
    uint32_t count;
    FieldNodeType elementType;
};

struct FieldNodePair : FieldNode {
    FieldNode* first;
    FieldNode* second;
};

struct FieldNodeTuple : FieldNode {
    uint8_t count;
    FieldNode** elements;
};