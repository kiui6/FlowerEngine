#pragma once

#include <Utility/ID.h>

/*
 * Provides API for compile time type identification
 */
template <ID32 id>
class StaticallyTyped
{
public:
    static ID32 StaticType() { return id; }
};