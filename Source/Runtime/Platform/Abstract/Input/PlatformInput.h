#pragma once

#include "RawInputDevice.h"
#include <Platform/Abstract/Window/Window.h>

#include <memory>

class PlatformInput
{
public:
    PlatformInput() = default;
    virtual ~PlatformInput() = default;

    std::unique_ptr<RawInputDevice> CreateRawInputDevice(Window& window);
};