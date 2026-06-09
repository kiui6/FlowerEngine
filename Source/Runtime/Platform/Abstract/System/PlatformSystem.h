#pragma once

#include <string_view>

class PlatformSystem
{
public:
    PlatformSystem() = default;
    virtual ~PlatformSystem() = default;

    virtual void DebugPrint(const char* string) = 0;
    virtual const std::string_view PlatformName() = 0;
};