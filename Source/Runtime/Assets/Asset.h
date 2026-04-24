#pragma once

#include <cstdint>

class Asset
{
public:
    virtual ~Asset(){}

    static uint64_t GetStaticType() {return 0;}
};