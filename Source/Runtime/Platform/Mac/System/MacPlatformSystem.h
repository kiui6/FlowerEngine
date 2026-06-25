#pragma once

#include <Platform/Abstract/System/PlatformSystem.h>

class MacPlatformSystem : public PlatformSystem
{
public:

    virtual void DebugPrint(const char* string) override;
    virtual const std::string_view PlatformName() override {return "Mac";}
};